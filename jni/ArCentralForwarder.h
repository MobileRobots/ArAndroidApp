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
#ifndef ARCENTRALFORWARDER_H
#define ARCENTRALFORWARDER_H

#include "Aria.h"
#include "ArServerBase.h"
#include "ArClientBase.h"

/**
   Class 
**/
class ArCentralForwarder 
{
public:
  AREXPORT ArCentralForwarder(
	  ArServerBase *mainServer, ArSocket *socket,
	  const char *robotName, int startingPort, std::set<int> *usedPorts,
	  ArFunctor2<ArCentralForwarder *,
	  ArServerClient *> *notifyServerClientRemovedCB);
  AREXPORT ~ArCentralForwarder();
  ArServerBase *getServer(void) { return myServer; }
  ArClientBase *getClient(void) { return myClient; }
  int getPort(void) { return myPort; }
  const char *getRobotName(void) { return myRobotName.c_str(); }
  AREXPORT void netCentralHeartbeat(ArNetPacket *packet);
  AREXPORT bool callOnce(
	  double heartbeatTimeout, double udpHeartbeatTimeout,
	  double robotBackupTimeout, double clientBackupTimeout);
  AREXPORT bool isConnected(void) { return myState == STATE_CONNECTED; }
protected:
  void robotServerClientRemoved(ArServerClient *client);
  void clientServerClientRemoved(ArServerClient *client);
  void receiveData(ArNetPacket *packet);
  void requestChanged(long interval, unsigned int command);
  void requestOnce(ArServerClient *client, ArNetPacket *packet);

  AREXPORT bool startingCallOnce(
	  double heartbeatTimeout, double udpHeartbeatTimeout,
	  double robotBackupTimeout, double clientBackupTimeout);
  AREXPORT bool connectingCallOnce(
	  double heartbeatTimeout, double udpHeartbeatTimeout,
	  double robotBackupTimeout, double clientBackupTimeout);
  AREXPORT bool gatheringCallOnce(
	  double heartbeatTimeout, double udpHeartbeatTimeout,
	  double robotBackupTimeout, double clientBackupTimeout);
  AREXPORT bool connectedCallOnce(
	  double heartbeatTimeout, double udpHeartbeatTimeout,
	  double robotBackupTimeout, double clientBackupTimeout);

  ArServerBase *myMainServer;
  ArSocket *mySocket;
  std::string myRobotName;
  std::string myPrefix;
  int myStartingPort;
  std::set<int> *myUsedPorts;
  ArFunctor2<ArCentralForwarder *,
	     ArServerClient *> *myForwarderServerClientRemovedCB;

  enum State
  {
    STATE_STARTING,
    STATE_CONNECTING,
    STATE_GATHERING,
    STATE_CONNECTED
  };

  ArServerBase *myServer;
  ArClientBase *myClient;
  State myState;
  int myPort;
  ArServerBase *server;
  ArClientBase *client;

  bool myRobotHasCentralServerHeartbeat;
  ArTime myLastSentCentralServerHeartbeat;

  enum ReturnType
  {
    RETURN_NONE,
    RETURN_SINGLE,
    RETURN_VIDEO,
    RETURN_UNTIL_EMPTY,
    RETURN_COMPLEX,
    RETURN_VIDEO_OPTIM,
  };

  std::map<unsigned int, ReturnType> myReturnTypes;
  std::map<unsigned int, std::list<ArServerClient *> *> myRequestOnces;
  std::map<unsigned int, ArTime *> myLastRequest;
  std::map<unsigned int, ArTime *> myLastBroadcast;

  ArTime myLastTcpHeartbeat;
  ArTime myLastUdpHeartbeat;

  ArFunctor1C<ArCentralForwarder, ArNetPacket *> myReceiveDataFunctor;
  ArFunctor2C<ArCentralForwarder, 
	      long, unsigned int> myRequestChangedFunctor;
  ArFunctor2C<ArCentralForwarder, 
	      ArServerClient *, ArNetPacket *> myRequestOnceFunctor;
  ArFunctor1C<ArCentralForwarder, 
      ArServerClient *> myRobotServerClientRemovedCB;
  ArFunctor1C<ArCentralForwarder, 
      ArNetPacket *> myNetCentralHeartbeatCB;
  ArFunctor1C<ArCentralForwarder, 
	      ArServerClient *> myClientServerClientRemovedCB;
  
};


#endif // ARSERVERSWITCHFORWARDER
