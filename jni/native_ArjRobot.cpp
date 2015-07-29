
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
protected:
	ArClientBase *myClient;
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

	bool gotHeader;
	ArFunctor1C<OutputHandler, ArNetPacket *> myHandleServerStatusCB;	
	ArFunctor1C<OutputHandler, ArNetPacket *> myHandleGoalListCB;
	ArFunctor1C<OutputHandler, ArNetPacket *> myHandleMapCB;
};

/** 
 OUTPUTHANDLER::Constructor 
*/
OutputHandler::OutputHandler(ArClientBase *client) :
	myClient(client),
	myHandleServerStatusCB(this, &OutputHandler::handleServerStatus),
	myHandleGoalListCB(this, &OutputHandler::handleGoalList),
	myHandleMapCB(this, &OutputHandler::handleMap)
{
	// Add handlers and start requests
  myClient->addHandler("update", &myHandleServerStatusCB);
  myClient->addHandler("getGoals", &myHandleGoalListCB);
  myClient->addHandler("getMapBinary", &myHandleMapCB);
  myClient->requestOnce("getGoals");
  myClient->request("update", 33);
  
  // Initialize member vars
  myServerStatus = 1;
  myCurrentGoal = "none";
  gotHeader = false;
  myPointCount = 0;
}

/** 
 OUTPUTHANDLER::Destructor 
*/
OutputHandler::~OutputHandler(void)
{
	myClient->requestStop("update");
}

/** 
 OUTPUTHANDLER::Callback to handle server status updates 
*/
void OutputHandler::handleServerStatus(ArNetPacket *packet) 
{
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
}

/**
 OUTPUTHANDLER::Callback to handle goal list
*/
void OutputHandler::handleGoalList(ArNetPacket *packet)
{
	myGoalsCount = 0;
  for(int i = 0; packet->getReadLength() < packet->getLength(); i++) {
    packet->bufToStr(myGoals[i], 255);
    if(strlen(myGoals[i]) == 0)
        return;
    myGoalCount++;
  }
}

/**
 OUTPUTHANDLER::Callback to handle map
*/
void OutputHandler::handleMap(ArNetPacket *packet)
{
	int skip = 0;
	int numToSkip = 8; // filter the points, we don't need a lot on a small screen.
	if (!gotHeader) {
		char buffer[256];
		packet->bufToStr(buffer, sizeof(buffer));
		if (buffer[0] == '\0')
			gotHeader = true;
	}
	if (gotHeader) {
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
	ArClientBase *myClient;
};

/* 
 INPUTHANDLER::Constructor 
*/
InputHandler::InputHandler(ArClientBase *client) :
	myClient(client)
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
 
static ArClientBase myClient;
static InputHandler *myInputHandler;
static OutputHandler *myOutputHandler;
static int myServerStatus;
static double myRobotStatus[8];
static bool myShutdown;

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
	if (!myClient.blockingConnect(cHost, port, true, NULL, NULL, NULL)) {
		debugPrint("Error connecting");
		return -1;
	}
	debugPrint("Connected");
	
	// Run the client connection in a different thread  
	myClient.runAsync();
	
	// Download the map, maybe move this to it's own function later
	debugPrint("getting map");
	myClient.requestOnce("getMapBinary");
	
	// Create the OutputHandler
	myOutputHandler = new OutputHandler(&myClient);
	
	/* Block until the connection is closed.  We will use this time to get
	 * status update information at a regular rate. */
	myShutdown = false;
	while (myClient.getRunningWithLock() && !myShutdown) {
    ArUtil::sleep(100);
    myServerStatus = myOutputHandler->getServerStatus();
    myOutputHandler->getRobotStatus(myRobotStatus);
  }
  debugPrint("Shutting down Aria");
  myClient.disconnect();
  Aria::shutdown();
  return 0;
}

/** 
 Shutdown 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_shutdown
  (JNIEnv *, jobject)
{
	myShutdown = true;
}

/** 
 Go to a named goal 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_gotoGoal
  (JNIEnv *env, jobject obj, jstring goal)
{
	const char *cGoal = env->GetStringUTFChars(goal, 0);
	debugPrintS("Sending to goal ", cGoal);
	myOutputHandler->setGoal(cGoal);
	ArUtil::sleep(100);
	myClient.requestOnceWithString("gotoGoal", cGoal);
	debugPrint("On its way");
	//env->ReleaseStringUTFChars(goal, cGoal);
}

/** 
 Stop the robot 
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_stop
  (JNIEnv *env, jobject obj)
{
	debugPrint("Stopping robot");
	myClient.requestOnce("stop");
	return 1;
}

/** 
 Block until goal complete or timeout occurs
*/
JNIEXPORT jint JNICALL Java_com_adept_arandroid_ArjRobot_waitGoalDone
  (JNIEnv *env, jobject obj, jint timeout)
{
	int cnt = 0;
	do {
		ArUtil::sleep(100);
		if (cnt > (timeout/100)) {
			debugPrint("timeout moving robot");
			return -2;
		}
		cnt++;
	} while (myServerStatus > 1);
	return myServerStatus;
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
		fill[i] = myRobotStatus[i];
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
	jobjectArray result = myOutputHandler->getGoals(env);
	return result;
}

/** 
 Get the map
*/
JNIEXPORT jdoubleArray JNICALL Java_com_adept_arandroid_ArjRobot_getMap
  (JNIEnv *env, jobject obj)
{
	jdoubleArray out;
	int cnt = myOutputHandler->getMapPointCount();
	out = env->NewDoubleArray(cnt);
	if (out == NULL) {
		return NULL;
	}
	double points[cnt];
	myOutputHandler->getMapPoints(points);
	int i;
	jdouble fill[cnt];
	for (i = 0; i < cnt; i++) {
		fill[i] = points[i];
	}
	env->SetDoubleArrayRegion(out, 0, cnt, fill);
	return out;
}


