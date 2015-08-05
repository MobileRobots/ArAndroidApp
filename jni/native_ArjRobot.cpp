
#include "native_ArjRobot.h"
#include "Aria.h"
#include "ArNetworking.h"
#include <android/log.h>

/******************************************************************************
 * UTILITY FUNCTIONS
 ******************************************************************************/
 
/**
 Print debug statement to logCat in eclipse
*/
void debugPrint(const char *ln) {
	__android_log_print(ANDROID_LOG_DEBUG, "ArAndroid", "ArjRobot:DEBUG: %s", ln);
}
void ariaLogDebugPrint(const char *ln) {
  __android_log_print(ANDROID_LOG_DEBUG, "ArAndroid", "ARIA:DEBUG: %s", ln);
}
void debugPrintInt(int i) {
	__android_log_print(ANDROID_LOG_DEBUG, "ArAndroid", "ArjRobot:DEBUG: %d", i);
}
void debugPrintS(const char *s1, const char *s2) {
	__android_log_print(ANDROID_LOG_DEBUG, "ArAndroid", "ArjRobot:DEBUG: %s %s", s1, s2);
}
void debugPrintSI(const char *s, int i) {
	__android_log_print(ANDROID_LOG_DEBUG, "ArAndroid", "ArjRobot:DEBUG: %s %d", s, i);
}
void debugPrintDDD(double d1, double d2, double d3) {
	__android_log_print(ANDROID_LOG_DEBUG, "ArAndroid", "ArjRobot:DEBUG: %f %f %f", d1, d2, d3);
}

/**
 Create a JNI object array from the string array
*/
jobjectArray createStringArray(JNIEnv *env, char argv[][256], int argc)
{
	jclass stringClass = NULL;
	jobjectArray strArray = NULL;
	jobjectArray result = NULL;
	int i;
	
	stringClass = env->FindClass("java/lang/String");
	if (env->ExceptionCheck()) {
		goto bail;
	}
	//assert(stringClass != NULL);
	strArray = env->NewObjectArray(argc, stringClass, NULL);
	if (env->ExceptionCheck()) {
		goto bail;
	}
	//assert(strArray != NULL);
	
	for (i = 0; i < argc; i++) {
		jstring argStr;
		argStr = env->NewStringUTF(argv[i]);
		if (env->ExceptionCheck()) {
			goto bail;
		}
		//assert(argStr != NULL);
		env->SetObjectArrayElement(strArray, i, argStr);
		env->DeleteLocalRef(argStr);
	}
	
	/* return the array */
	result = strArray;
	strArray = NULL;
	
bail:
	env->DeleteLocalRef(stringClass);
	env->DeleteLocalRef(strArray);
	return result;	
}

/******************************************************************************
 * OUTPUTHANDLER CLASS
 ******************************************************************************/
 
/**
 OUTPUTHANDLER::This class handles all output data from the robot
*/
class OutputHandler 
{
public:
	OutputHandler(ArClientBase *client);
	virtual ~OutputHandler(void);
	void handleServerStatus(ArNetPacket *packet);
	void handleGoalList(ArNetPacket *packet);
	void handleMap(ArNetPacket *packet);
	void setGoal(const char *currentGoal);
	jobjectArray getGoals(JNIEnv *env);
	int getServerStatus();
	void getRobotStatus(double *status);
	void getMapPoints(double *points);
	int getMapPointCount();
	ArMap getMap();
  void lock()
  {
    myMutex.lock();
  }
  void unlock()
  {
    myMutex.unlock();
  }
protected:
	ArClientBase *gClient;
	ArMap myMap;
	JNIEnv *myEnv;
	const char *myCurrentGoal;
	char myGoals[256][256];
	int myGoalsCount;
	int myServerStatus;
	int myGoalCount;
	double myStatus[8];
	double myPoints[128000];
	int myPointCount;
  ArMutex myMutex;

	bool myGotMapHeader;
	ArFunctor1C<OutputHandler, ArNetPacket *> myHandleServerStatusCB;	
	ArFunctor1C<OutputHandler, ArNetPacket *> myHandleGoalListCB;
	ArFunctor1C<OutputHandler, ArNetPacket *> myHandleMapCB;
};

/** 
 OUTPUTHANDLER::Constructor 
*/
OutputHandler::OutputHandler(ArClientBase *client) :
	gClient(client),
	myHandleServerStatusCB(this, &OutputHandler::handleServerStatus),
	myHandleGoalListCB(this, &OutputHandler::handleGoalList),
	myHandleMapCB(this, &OutputHandler::handleMap)
{
	// Add handlers and start requests
  gClient->addHandler("update", &myHandleServerStatusCB);
  gClient->addHandler("getGoals", &myHandleGoalListCB);
  gClient->addHandler("getMapBinary", &myHandleMapCB);
  gClient->requestOnce("getGoals");
  gClient->request("update", 33);
  
  // Initialize member vars
  myMutex.lock();
  myServerStatus = 1;
  myCurrentGoal = "none";
  myGotMapHeader = false;
  myPointCount = 0;
  myMutex.unlock();
}

/** 
 OUTPUTHANDLER::Destructor 
*/
OutputHandler::~OutputHandler(void)
{
	gClient->requestStop("update");
}

/** 
 OUTPUTHANDLER::Callback to handle server status updates 
*/
void OutputHandler::handleServerStatus(ArNetPacket *packet) 
{
  myMutex.lock();
  char status[64];
  char mode[64];
  packet->bufToStr(status, 63);
  packet->bufToStr(mode, 63);
  myStatus[0] = ( (double) packet->bufToByte2() )/10.0;	// voltage
  myStatus[1] = (double) packet->bufToByte4();			    // X 		    (mm)
  myStatus[2] = (double) packet->bufToByte4();			    // Y 		    (mm)
  myStatus[3] = (double) packet->bufToByte2();			    // Theta 	  (deg)
  myStatus[4] = (double) packet->bufToByte2();			    // Vel 		  (mm/s)
  myStatus[5] = (double) packet->bufToByte2();			    // Rot Vel 	(deg/s)
  myStatus[6] = (double) packet->bufToByte2();			    // Lat Vel 	(mm/s)
  myStatus[7] = (double) packet->bufToByte();			      // Temp		  (F?)
  
  // Set server status
  char base[128] = "Arrived at ";
  char failbase[128] = "Failed to get to ";
  char *done = strcat(base, myCurrentGoal);
  char *fail = strcat(failbase, myCurrentGoal);
  if (strstr(status, fail) != NULL) {
    myServerStatus = -10; /* Failed to get to goal */
  } else if (strstr(status, done) == NULL) {
    myServerStatus = 2;		/* Moving to goal */
  } else {
    myServerStatus = 1;		/* Arrived at goal */
  }

  myMutex.unlock();
}

/**
 OUTPUTHANDLER::Callback to handle goal list
*/
void OutputHandler::handleGoalList(ArNetPacket *packet)
{
  myMutex.lock();
	myGoalsCount = 0;
  for(int i = 0; packet->getReadLength() < packet->getLength(); i++) {
    packet->bufToStr(myGoals[i], 255);
    if(strlen(myGoals[i]) == 0)
    {
        myMutex.unlock();
        return;
    }
    myGoalCount++;
  }
  myMutex.unlock();
}

/**
 OUTPUTHANDLER::Callback to handle map
*/
void OutputHandler::handleMap(ArNetPacket *packet)
{
  myMutex.lock();
	int skip = 0;
	int numToSkip = 8; // filter the points, we don't need a lot on a small screen.
	if (!myGotMapHeader) {
		char buffer[256];
		packet->bufToStr(buffer, sizeof(buffer));
		if (buffer[0] == '\0')
			myGotMapHeader = true;
	}
	if (myGotMapHeader) {
		double cnt = (double)packet->bufToByte4();
		if (cnt < 1001) {
			for (int i = 0; i < (int)cnt; i++) {
				double x = (double)packet->bufToByte4();
				double y = (double)packet->bufToByte4();
				if (skip > numToSkip) {
					myPoints[myPointCount++] = x;
					myPoints[myPointCount++] = y;
					skip = 0;
				} else {
					skip++;
				}
			}
		} 
	}
  myMutex.unlock();
}


/** 
 OUTPUTHANDLER::Sets the goal we are going to 
*/
void OutputHandler::setGoal(const char *currentGoal)
{
	myServerStatus = 3;
	myCurrentGoal = currentGoal;
}

/** 
 OUTPUTHANDLER::Gets the server status 
*/
int OutputHandler::getServerStatus() 
{
	return myServerStatus;
}

/**
 OUTPUTHANDLER::Gets the list of goals
*/
jobjectArray OutputHandler::getGoals(JNIEnv *env)
{
	debugPrintInt(myGoalCount);
	jobjectArray result = createStringArray(env, myGoals, myGoalCount);
	return result;
}

/** 
 OUTPUTHANDLER::Gets the robot status 
*/
void OutputHandler::getRobotStatus(double *status)
{
	int i;
	for (i = 0; i < 8; i++) {
		status[i] = myStatus[i];
	}
}

/**
 OUTPUTHANDLER::Gets the map points as a double array
*/
void OutputHandler::getMapPoints(double *points)
{
	int i;
	for (i = 0; i < myPointCount; i++) {
		points[i] = myPoints[i];
	}
}

/**
 OUTPUTHANDLER::Gets the number of points in the map
*/
int OutputHandler::getMapPointCount() 
{
	return myPointCount;
}

/******************************************************************************
 * INPUTHANDLER CLASS
 ******************************************************************************/
 
/**
 INPUTHANDLER::This class handles all input data to the robot
*/
class InputHandler
{
public:
	InputHandler(ArClientBase *client);
	virtual ~InputHandler(void);
protected:
	ArClientBase *gClient;
};

/* 
 INPUTHANDLER::Constructor 
*/
InputHandler::InputHandler(ArClientBase *client) :
	gClient(client)
{

}

/* 
 INPUTHANDLER::Destructor 
*/
InputHandler::~InputHandler(void)
{

}

/******************************************************************************
 * NATIVE FUNCTIONS
 ******************************************************************************/
 
static ArClientBase gClient;
//static InputHandler *gInputHandler; // unused
static OutputHandler *gOutputHandler;
static ArMutex gMutex; // Locked during access to global variables below:
static int gGoalStatus;
static double gRobotStatus[8];
static bool gShutdown;

/** 
 Initialize the robot connection 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_initialize
  (JNIEnv *env, jobject obj, jstring host, jint port)
{
	// Get the host ip address
	const char *cHost = env->GetStringUTFChars(host, 0);
	
	// Try to connect
	Aria::init();
  ArLog::setFunctor(new ArGlobalFunctor1<const char*>(ariaLogDebugPrint));
	debugPrintS("Connecting to ", cHost);
	if (!gClient.blockingConnect(cHost, port, true, NULL, NULL, NULL)) {
		debugPrint("Error connecting");
		return -1;
	}
	debugPrint("Connected");
	
	// Run the client connection in a different thread  
	gClient.runAsync();
	
	// Download the map, maybe move this to it's own function later

	debugPrint("getting map");
	gClient.requestOnce("getMapBinary");
	
	// Create the OutputHandler and InputHandler
	gOutputHandler = new OutputHandler(&gClient);
	
	/* Block until the connection is closed.  We will use this time to get
	 * status update information at a regular rate. */
  gMutex.lock();
	gShutdown = false;
  gMutex.unlock();

	while (gClient.getRunningWithLock()) {
    ArUtil::sleep(100);
    gMutex.lock();
    if (gShutdown) {
      gMutex.unlock();
      break;
    }
    gOutputHandler->lock();
    gGoalStatus = gOutputHandler->getServerStatus();
    gOutputHandler->getRobotStatus(gRobotStatus);
    gOutputHandler->unlock();
    gMutex.unlock();
  }
  debugPrint("Shutting down Aria");
  gClient.disconnect();
  Aria::shutdown();
  return 0;
}

/** 
 Shutdown 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_shutdown
  (JNIEnv *, jobject)
{
  gMutex.lock();
	gShutdown = true;
  gMutex.unlock();
  return 1;
}

/** 
 Go to a named goal 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_gotoGoal
  (JNIEnv *env, jobject obj, jstring goal)
{
	const char *cGoal = env->GetStringUTFChars(goal, 0);
  gOutputHandler->lock();
	debugPrintS("Sending to goal ", cGoal);
	gOutputHandler->setGoal(cGoal);
  gOutputHandler->unlock();
	ArUtil::sleep(100);
	gClient.requestOnceWithString("gotoGoal", cGoal);
	debugPrint("On its way");
	//env->ReleaseStringUTFChars(goal, cGoal);
  return 1;
}

/** 
 Stop the robot 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_stop
  (JNIEnv *env, jobject obj)
{
	debugPrint("Stopping robot");
	gClient.requestOnce("stop");
	return 1;
}

/** 
 Send the robot to dock
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_dock
  (JNIEnv *env, jobject obj)
{
	debugPrint("Send the robot to dock");
	gClient.requestOnce("dock");
	return 1;
}

void sendInput(double myTransRatio, double myRotRatio, double myThrottle, double myLatRatio)
{
  /* This method is called by the main function to send a ratioDrive
   * request with our current velocity values. If the server does 
   * not support the ratioDrive request, then we abort now: */
  //if(!gClient->dataExists("ratioDrive")) return;

  /* Construct a ratioDrive request packet.  It consists
   * of three doubles: translation ratio, rotation ratio, and an overall scaling
   * factor. */
  ArNetPacket packet;
  packet.doubleToBuf(myTransRatio);
  packet.doubleToBuf(myRotRatio);
  packet.doubleToBuf(myThrottle); // this is an additional amount (percentage) that is applied to each of the trans,rot,lat velocities. 
  packet.doubleToBuf(myLatRatio);
//  if (myPrinting) printf("ArClientRatioDrive: Sending ratioDrive request\n");
  gClient.requestOnce("ratioDrive", &packet);
}


JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_forward
  (JNIEnv *env, jobject obj)
{
	debugPrint("Move the robot forward");
	sendInput(50, 0, 100, 0);
	return 1;
}

JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_spin_clockwise
  (JNIEnv *env, jobject obj)
{
	debugPrint("Rotate the robot clockwise");
	sendInput(0, 50, 100, 0);
	return 1;
}

JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_backward
  (JNIEnv *env, jobject obj)
{
	debugPrint("Move the robot backward");
	sendInput(-50, 0, 100, 0);
	return 1;
}

JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_spin_counterclockwise
  (JNIEnv *env, jobject obj)
{
	debugPrint("Rotate the robot counterclockwise");
	sendInput(0, -50, 100, 0);
	return 1;
}

/** 
 Block until goal complete or timeout occurs
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_waitGoalDone
  (JNIEnv *env, jobject obj, jint timeout)
{
	int cnt = 0;
	while(true) {
		ArUtil::sleep(100);
		if (cnt > (timeout/100)) {
			debugPrint("timeout moving robot");
			return -2;
		}
		cnt++;
    gMutex.lock();
    int status = gGoalStatus;
    gMutex.unlock();
    if(status > 1)
      return status;
	} 
	return -2;
}

/** 
 Get robot status 
*/
JNIEXPORT jdoubleArray JNICALL Java_com_adept_arandroid_ArjRobot_getRobotStatus
  (JNIEnv *env, jobject obj)
{
	jdoubleArray status;
	status = env->NewDoubleArray(8);
	if (status == NULL) {
		return NULL; /* out of memory exception thrown */
	}
	int i;
	jdouble fill[8];
	for (i = 0; i < 8; i++) {
		fill[i] = gRobotStatus[i];
	}
	env->SetDoubleArrayRegion(status, 0, 8, fill);
	return status;
}

/**
 Get this robot's goals
*/
JNIEXPORT jobjectArray JNICALL Java_com_adept_arandroid_ArjRobot_getGoals
  (JNIEnv *env, jobject obj)
{
	debugPrint("getting goals");
  gOutputHandler->lock();
	jobjectArray result = gOutputHandler->getGoals(env);
  gOutputHandler->unlock();
	return result;
}

/** 
 Get the map
*/
JNIEXPORT jdoubleArray JNICALL Java_com_adept_arandroid_ArjRobot_getMap
  (JNIEnv *env, jobject obj)
{
	jdoubleArray out;
  gOutputHandler->lock();
	int cnt = gOutputHandler->getMapPointCount();
	out = env->NewDoubleArray(cnt);
	if (out == NULL) {
    gOutputHandler->unlock();
		return NULL;
	}
	double points[cnt];
	gOutputHandler->getMapPoints(points);
  gOutputHandler->unlock();
	int i;
	jdouble fill[cnt];
	for (i = 0; i < cnt; i++) {
		fill[i] = points[i];
	}
	env->SetDoubleArrayRegion(out, 0, cnt, fill);
	return out;
}

