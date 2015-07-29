/*
MobileRobots Advanced Robotics Interface for Applications (ARIA)
Copyright (C) 2004, 2005 ActivMedia Robotics LLC
Copyright (C) 2006, 2007, 2008, 2009 MobileRobots Inc.

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

If you wish to redistribute ARIA under different terms, contact 
MobileRobots for information about a commercial version of ARIA at 
robots@mobilerobots.com or 
MobileRobots Inc, 10 Columbia Drive, Amherst, NH 03031; 800-639-9481
*/
#include "ArExport.h"
#include "ariaOSDef.h"
#include "ArUrg.h"
#include "ArRobot.h"
#include "ArSerialConnection.h"
#include "ariaInternal.h"

AREXPORT ArUrg::ArUrg(int laserNumber, const char *name) :
  ArLaser(laserNumber, name, 4095),
  mySensorInterpTask(this, &ArUrg::sensorInterp),
  myAriaExitCB(this, &ArUrg::disconnect)
{
  clear();
  myRawReadings = NULL;

  Aria::addExitCallback(&myAriaExitCB, -10);

  laserSetName(getName());

  setParams();
  setSensorPosition(0, 0, 0);     

  laserAllowSetDegrees(-135, -135, 135, // start degrees
		  135, -135, 135); // end degrees

  laserAllowSetIncrement(1, 0, 135);
  
  std::list<std::string> baudChoices;
  baudChoices.push_back("019200");
  baudChoices.push_back("057600");
  baudChoices.push_back("115200");
  baudChoices.push_back("250000");
  baudChoices.push_back("500000");
  baudChoices.push_back("750000");
  laserAllowStartingBaudChoices("019200", baudChoices);

  laserAllowAutoBaudChoices("057600", baudChoices);


  setMinDistBetweenCurrent(50);
  setMaxDistToKeepCumulative(4000);
  setMinDistBetweenCumulative(200);
  setMaxSecondsToKeepCumulative(30);
  setMaxInsertDistCumulative(3000);

  setCumulativeCleanDist(75);
  setCumulativeCleanInterval(1000);
  setCumulativeCleanOffset(600);

  resetLastCumulativeCleanTime();

  setCurrentDrawingData(
	  new ArDrawingData("polyDots", 
			    ArColor(0, 0xaa, 0xaa), 
			    70,  // mm diameter of dots
			    77), true);
  setCumulativeDrawingData(
	  new ArDrawingData("polyDots", 
			    ArColor(0, 0x55, 0x55), 
			    100,  // mm diameter of dots
			    62), true);

  myLogMore = false;
}

AREXPORT ArUrg::~ArUrg()
{
  Aria::remExitCallback(&myAriaExitCB);
  if (myRobot != NULL)
  {
    myRobot->remRangeDevice(this);
    myRobot->remLaser(this);
    myRobot->remSensorInterpTask(&mySensorInterpTask);
  }
  if (myRawReadings != NULL)
  {
    ArUtil::deleteSet(myRawReadings->begin(), myRawReadings->end());
    myRawReadings->clear(); 
    delete myRawReadings;
    myRawReadings = NULL;
  }
  lockDevice();
  if (isConnected())
    disconnect();
  unlockDevice();
}

AREXPORT void ArUrg::laserSetName(const char *name)
{
  myName = name;
  
  myConnMutex.setLogNameVar("%s::myConnMutex", getName());
  myReadingMutex.setLogNameVar("%s::myReadingMutex", getName());
  myDataMutex.setLogNameVar("%s::myDataMutex", getName());
  myAriaExitCB.setNameVar("%s::exitCallback", getName());
  
  ArLaser::laserSetName(getName());
}

AREXPORT bool ArUrg::setParams(
	double startingDegrees, double endingDegrees,
	double incrementDegrees, bool flipped)
{
  if (startingDegrees < -135.0 || startingDegrees > 135.0 || 
      endingDegrees < -135.0 || startingDegrees > 135.0 || 
      incrementDegrees < 0.0)
  {
    ArLog::log(ArLog::Normal, 
	       "%s::setParams: Bad params (starting %g ending %g clusterCount %g)",
	       getName(), startingDegrees, endingDegrees, incrementDegrees);
    return false;
  }
  double startingStepRaw;
  double endingStepRaw;
  int startingStep;
  int endingStep;
  int clusterCount;
  
  startingStepRaw = -(startingDegrees - 135.0) / .3515625;
  endingStepRaw = -(endingDegrees - 135.0) / .3515625;
  startingStep = (int)ceil(ArUtil::findMin(startingStepRaw, endingStepRaw));
  endingStep = (int)floor(ArUtil::findMax(startingStepRaw, endingStepRaw));
  clusterCount = ArMath::roundInt(incrementDegrees / .3515625);
  
  ArLog::log(ArLog::Verbose, "%s: starting deg %.1f raw %.1f step %d ending deg %.1f raw %.1f step %d, cluster deg %f count %d flipped %d",
	     getName(), startingDegrees, startingStepRaw, startingStep, endingDegrees, endingStepRaw, endingStep, incrementDegrees, clusterCount, flipped);
  
  return setParamsBySteps(startingStep, endingStep, clusterCount, flipped);
}

AREXPORT bool ArUrg::setParamsBySteps(int startingStep, int endingStep, 
				      int clusterCount, bool flipped)
{
  if (startingStep < 0 || startingStep > 768 || 
      endingStep < 0 || endingStep > 768 || 
      startingStep > endingStep || 
      clusterCount < 1)
  {
    ArLog::log(ArLog::Normal, 
	       "%s::setParamsBySteps: Bad params (starting %d ending %d clusterCount %d)",
	       getName(), startingStep, endingStep, clusterCount);
    return false;
  }
  
  myDataMutex.lock();
  myStartingStep = startingStep;
  myEndingStep = endingStep;
  myClusterCount = clusterCount;
  myFlipped = flipped;
  sprintf(myRequestString, "G%03d%03d%02d\n", myStartingStep, endingStep,
	  clusterCount);
  myClusterMiddleAngle = 0;
  if (myClusterCount > 1)
    myClusterMiddleAngle = myClusterCount * 0.3515625 / 2.0;

  if (myRawReadings != NULL)
  {
    ArUtil::deleteSet(myRawReadings->begin(), myRawReadings->end());
    myRawReadings->clear();
    delete myRawReadings;
    myRawReadings = NULL;
  }

  myRawReadings = new std::list<ArSensorReading *>;
  

  ArSensorReading *reading;
  int onStep;
  double angle;

  for (onStep = myStartingStep; 
       onStep < myEndingStep; 
       onStep += myClusterCount)
  {
    /// FLIPPED
    if (!myFlipped)
      angle = ArMath::subAngle(ArMath::subAngle(135, onStep * 0.3515625),
			       myClusterMiddleAngle);
    else
      angle = ArMath::addAngle(ArMath::addAngle(-135, onStep * 0.3515625), 
			       myClusterMiddleAngle);
			       
    reading = new ArSensorReading;
    reading->resetSensorPosition(ArMath::roundInt(mySensorPose.getX()),
				 ArMath::roundInt(mySensorPose.getY()),
				 ArMath::addAngle(angle, 
						  mySensorPose.getTh()));
    myRawReadings->push_back(reading);
    //printf("%.1f ", reading->getSensorTh());
  }


  myDataMutex.unlock();
  return true;
}

void ArUrg::clear(void)
{
  myIsConnected = false;
  myTryingToConnect = false;
  myStartConnect = false;

  myVendor = "";
  myProduct = "";
  myFirmwareVersion = "";
  myProtocolVersion = "";
  mySerialNumber = "";
  myStat = "";
}

AREXPORT void ArUrg::log(void)
{
  ArLog::log(ArLog::Normal, "Vendor information: %s", myVendor.c_str());
  ArLog::log(ArLog::Normal, "Product information: %s", myProduct.c_str());
  ArLog::log(ArLog::Normal, "Firmware version: %s", myFirmwareVersion.c_str());
  ArLog::log(ArLog::Normal, "Protocol Version: %s", myProtocolVersion.c_str());
  ArLog::log(ArLog::Normal, "Serial number: %s", mySerialNumber.c_str());
  ArLog::log(ArLog::Normal, "Stat: %s", myStat.c_str());
}

AREXPORT void ArUrg::setRobot(ArRobot *robot)
{
  myRobot = robot;
  if (myRobot != NULL)
    myRobot->addSensorInterpTask("urg", 90, &mySensorInterpTask);
  ArRangeDevice::setRobot(robot);
}

bool ArUrg::writeString(const char *str)
{
  if (myConn == NULL)
  {
    ArLog::log(ArLog::Terse, 
	       "%s: Attempt to write string to null connection", getName());
    return false;
  }

  myConnMutex.lock();
  myConn->write(str, strlen(str));
  myConnMutex.unlock();
  return true;
}

bool ArUrg::readLine(char *buf, unsigned int size, 
			      unsigned int msWait)
{
  if (myConn == NULL)
  {
    ArLog::log(ArLog::Terse, 
	       "%s: Attempt to read line from null connection", getName());
    return false;
  }

  ArTime started;
  started.setToNow();

  buf[0] = '\0';
  unsigned int onChar = 0;
  int ret;

  myConnMutex.lock();
  while ((msWait == 0 || started.mSecSince() < (int)msWait) && 
	 onChar < size)
  {
    if ((ret = myConn->read(&buf[onChar], 1, 0)) > 0)
    {
      if (buf[onChar] == '\n' || 
	  buf[onChar] == '\r')
      {
	buf[onChar+1] = '\0';
	if (myLogMore)
	  ArLog::log(ArLog::Normal, "%s: %s", getName(), buf);
	myConnMutex.unlock();
	return true;
      }
      onChar++;
    }
    if (ret < 0)
    {
      myConnMutex.unlock();
      return false;
    }
    if (ret == 0)
      ArUtil::sleep(1);
  }
  myConnMutex.unlock();
  return false;
}

AREXPORT bool ArUrg::blockingConnect(void)
{
  char buf[1024];

  if (!getRunning())
    runAsync();

  myConnMutex.lock();
  if (myConn == NULL)
  {
    ArLog::log(ArLog::Terse, 
	       "%s: Could not connect because there is no connection defined", 
	       getName());
    myConnMutex.unlock();
    failedToConnect();
    return false;
  }

  if (myConn->getStatus() != ArDeviceConnection::STATUS_OPEN && 
      !myConn->openSimple())
  {
    ArLog::log(ArLog::Terse, 
	       "%s: Could not connect because the connection was not open and could not open it", getName());
    myConnMutex.unlock();
    failedToConnect();
    return false;
  }
  myConnMutex.unlock();

  lockDevice();
  myTryingToConnect = true;
  unlockDevice();

  laserPullUnsetParamsFromRobot();
  laserCheckParams();
  
  setParams(getStartDegrees(), getEndDegrees(), getIncrement(), getFlipped());

  ArSerialConnection *serConn = NULL;
  serConn = dynamic_cast<ArSerialConnection *>(myConn);

  if (serConn != NULL)
    serConn->setBaud(atoi(getStartingBaudChoice()));
  ArUtil::sleep(1000);
  
  // send for the version info
  if (!writeString("V\n"))
  {
    failedToConnect();
    return false;
  }

  // send for the version info
  if (!readLine(buf, sizeof(buf), 10000) || 
      strncasecmp(buf, "V", strlen("V")) != 0)
  {
    // if we didn't get it, try it at what the autobaud rate is
    if (serConn != NULL)
    {
      serConn->setBaud(atoi(getAutoBaudChoice()));
      ArUtil::sleep(1000);
      // send for the version info again at the new baud rate
      if (!writeString("V\n"))
      {
	failedToConnect();
	return false;
      }
      // if we still didn't get it, just give up
      if (!readLine(buf, sizeof(buf), 10000) || 
	  strncasecmp(buf, "V", strlen("V")) != 0)
      {
	ArLog::log(ArLog::Normal, "%s: '%s'\n", getName(), buf);
	ArLog::log(ArLog::Normal, "%s::blockingConnect: Did not get back version response (even after switching to autobaudchoice)", getName());
	failedToConnect();
	return false;
      }
    }
    // if we don't have a serial port, then we can't change the baud,
    // so just fail
    else
    {
      ArLog::log(ArLog::Normal, "%s: '%s'\n", getName(), buf);
      ArLog::log(ArLog::Normal, 
		 "%s::blockingConnect: Did not get back version response",
		 getName());
      failedToConnect();
      return false;
    }
  }

  // get the status from that request
  if (!readLine(buf, sizeof(buf), 10000) || 
      strncasecmp(buf, "0", strlen("0")) != 0)		  
  {
    ArLog::log(ArLog::Normal, 
	       "%s::blockingConnect: Bad status on version response",
	       getName());
    failedToConnect();
    return false;
  }

  
  if (serConn != NULL)
  {
    // now change the baud...

    //if (!writeString("S0576007654321\n"))

    sprintf(buf, "S%06d7654321\n", atoi(getAutoBaudChoice()));
    if (!writeString(buf))
    {
      failedToConnect();
      return false;
    }    
    ArUtil::sleep(1000);

    //serConn->setBaud(115200);
    serConn->setBaud(atoi(getAutoBaudChoice()));
    // wait a second for the baud to change...
    ArUtil::sleep(1000);
	
    // send for the version info
    if (!writeString("V\n"))
    {
      failedToConnect();
      return false;
    }
    
    // send for the version info
    if (!readLine(buf, sizeof(buf), 10000) || 
	strncasecmp(buf, "V", strlen("V")) != 0)
    {
      ArLog::log(ArLog::Normal, "%s: '%s'\n", getName(), buf);
      ArLog::log(ArLog::Normal, "%s::blockingConnect: Did not get back version response after baud change", 
		 getName());
      failedToConnect();
      return false;
    }

  }
  


  while (readLine(buf, sizeof(buf), 10000))
  {
    if (buf[0] == '\n' || buf[0] == '\r')
      break;

    if (strlen(buf) == 0)
    {
      ArLog::log(ArLog::Normal, 
		 "%s::blockingConnect: Bad information in version response",
		 getName());
      failedToConnect();
      return false;
    }
    // chop off the \n or \r
    buf[strlen(buf) - 1] = '\0';
    if (strncasecmp(buf, "VEND:", strlen("VEND:")) == 0)
      myVendor = &buf[5];
    else if (strncasecmp(buf, "PROD:", strlen("PROD:")) == 0)
      myProduct = &buf[5];
    else if (strncasecmp(buf, "FIRM:", strlen("FIRM:")) == 0)
      myFirmwareVersion = &buf[5];
    else if (strncasecmp(buf, "PROT:", strlen("PROT:")) == 0)
      myProtocolVersion = &buf[5];
    else if (strncasecmp(buf, "SERI:", strlen("SERI:")) == 0)
      mySerialNumber = &buf[5];
    else if (strncasecmp(buf, "STAT:", strlen("STAT:")) == 0)
      myStat = &buf[5];
  }

  if (myVendor.empty() || myProduct.empty() || myFirmwareVersion.empty() || 
      myProtocolVersion.empty() || mySerialNumber.empty())
  {
    ArLog::log(ArLog::Normal, 
	       "%s::blockingConnect: Missing information in version response",
	       getName());
    failedToConnect();
    return false;
  }

  
  // send for the laser illumination
  if (!writeString("L1\n"))
  {
    failedToConnect();
    return false;
  }

  // send for the version info
  if (!readLine(buf, sizeof(buf), 10000) || 
      strncasecmp(buf, "L1", strlen("L1")) != 0)
  {
    ArLog::log(ArLog::Normal, 
	       "%s::blockingConnect: Did not get laser illumination control", 
	       getName());
    failedToConnect();
    return false;
  }

  if (!readLine(buf, sizeof(buf), 10000) || 
      strncasecmp(buf, "0", strlen("0")) != 0)		  
  {
    ArLog::log(ArLog::Normal, 
       "%s::blockingConnect: Bad status on laser illumination response",
	       getName());
    failedToConnect();
    return false;
  }

  if (!readLine(buf, sizeof(buf), 10000) || 
      (buf[0] != '\n' && buf[0] != '\r'))
  {
    ArLog::log(ArLog::Normal, 
	     "%s::blockingConnect: Extra stuff on laser illumination response",
	       getName());
    failedToConnect();
    return false;
  }

  lockDevice();
  myIsConnected = true;
  myTryingToConnect = false;
  unlockDevice();
  ArLog::log(ArLog::Normal, "%s: Connected to laser", getName());
  laserConnect();
  return true;
}

AREXPORT bool ArUrg::asyncConnect(void)
{
  myStartConnect = true;
  if (!getRunning())
    runAsync();
  return true;
}

AREXPORT bool ArUrg::disconnect(void)
{
  if (!isConnected())
    return true;

  ArLog::log(ArLog::Normal, "%s: Disconnecting", getName());

  char buf[2048];
  sprintf(buf, "S%06d7654321\n", atoi(getStartingBaudChoice()));
  laserDisconnectNormally();
  if (writeString(buf))
    return true;
  else
    return false;
}

void ArUrg::sensorInterp(void)
{
  ArTime readingRequested;
  std::string reading;
  myReadingMutex.lock();
  if (myReading.empty())
  {
    myReadingMutex.unlock();
    return;
  }

  readingRequested = myReadingRequested;
  reading = myReading;
  myReading = "";
  myReadingMutex.unlock();

  ArTime time = readingRequested;
  ArPose pose;
  int ret;
  int retEncoder;
  ArPose encoderPose;

  //time.addMSec(-13);
  if (myRobot == NULL || !myRobot->isConnected())
  {
    pose.setPose(0, 0, 0);
    encoderPose.setPose(0, 0, 0);
  } 
  else if ((ret = myRobot->getPoseInterpPosition(time, &pose)) < 0 ||
	   (retEncoder = 
	    myRobot->getEncoderPoseInterpPosition(time, &encoderPose)) < 0)
  {
    ArLog::log(ArLog::Normal, "%s: reading too old to process", getName());
    return;
  }

  ArTransform transform;
  transform.setTransform(pose);

  unsigned int counter = 0; 
  if (myRobot != NULL)
    counter = myRobot->getCounter();

  lockDevice();
  myDataMutex.lock();

  //double angle;
  int i;
  int len = reading.size();

  int range;
  int big; 
  int little;
  //int onStep;

  std::list<ArSensorReading *>::reverse_iterator it;
  ArSensorReading *sReading;

  bool ignore;
  for (it = myRawReadings->rbegin(), i = 0; 
       it != myRawReadings->rend() && i < len - 1; 
       it++, i += 2)
  {
    ignore = false;
    big = reading[i] - 0x30;
    little = reading[i+1] - 0x30;
    range = (big << 6 | little);
    if (range < 20)
    {
      range = 4096;
    }
    sReading = (*it);
    sReading->newData(range, pose, encoderPose, transform, counter, 
		      time, ignore, 0);
  }

  myDataMutex.unlock();

  laserProcessReadings();
  unlockDevice();
}

AREXPORT void * ArUrg::runThread(void *arg)
{
  char buf[1024];
  ArTime readingRequested;
  std::string reading;

  while (getRunning())
  {
    lockDevice();
    if (myStartConnect)
    {
      myStartConnect = false;
      myTryingToConnect = true;
      unlockDevice();

      blockingConnect();

      lockDevice();
      myTryingToConnect = false;
      unlockDevice();
      continue;
    }
    unlockDevice();
    
    if (!myIsConnected)
    {
      ArUtil::sleep(100);
      continue;
    }

    // if we have a robot but it isn't running yet then don't have a
    // connection failure
    if (laserCheckLostConnection())
    {
      ArLog::log(ArLog::Terse, 
		 "%s:  Lost connection to the laser because of error.  Nothing received for %g seconds (greater than the timeout of %g).", getName(), 
		 myLastReading.mSecSince()/1000.0, 
		 getConnectionTimeoutSeconds());
      myIsConnected = false;
      laserDisconnectOnError();
      continue;
    }

    reading = "";
    readingRequested.setToNow();
    if (!writeString(myRequestString))
    {
      ArLog::log(ArLog::Terse, "Could not send request distance reading to urg");
      continue;
    }

    if (!readLine(buf, sizeof(buf), 10000) || 
	strncasecmp(buf, "G", strlen("G")) != 0)
    {
      ArLog::log(ArLog::Normal, "%s: Did not get distance reading response",
		 getName());
      continue;
    }

    if (!readLine(buf, sizeof(buf), 10000) || 
	strncasecmp(buf, "0", strlen("0")) != 0)		  
    {
      ArLog::log(ArLog::Normal, "%s::blockingConnect: Bad status on distance reading response",
		 getName());
      continue;
    }

    while (readLine(buf, sizeof(buf), 10000))
    {
      if (buf[0] == '\n' || buf[0] == '\r')
      {
	myReadingMutex.lock();
	myReadingRequested = readingRequested;
	myReading = reading;
	myReadingMutex.unlock();	
	if (myRobot == NULL)
	  sensorInterp();
	break;
      }

      // chop off the \n or \r
      if (buf[0] != '\0')
      {
	buf[strlen(buf) - 1] = '\0';
	reading += buf;
      }
    }
    //printf("\n\n");

    ArUtil::sleep(1);

  }
  return NULL;
}

void ArUrg::failedToConnect(void)
{
  lockDevice();
  myTryingToConnect = true;
  unlockDevice();
  laserFailedConnect();
}

bool ArUrg::laserCheckParams(void)
{
  laserSetAbsoluteMaxRange(4095);
  return true;
}
