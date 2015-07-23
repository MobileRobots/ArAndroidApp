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
#ifndef ARCENTRALMANAGER
#define ARCENTRALMANAGER

#include "Aria.h"
#include "ArServerBase.h"
#include "ArCentralForwarder.h"

class ArCentralManager : public ArASyncTask
{
public:
  /// Constructor
  AREXPORT ArCentralManager(ArServerBase *robotServer, ArServerBase *clientServer);
  /// Destructor
  AREXPORT virtual ~ArCentralManager();
  /// Logs all the connection information
  void logConnections(void);
  /// Adds a callback for when a new forwarder is added
  AREXPORT void addForwarderAddedCallback(
	  ArFunctor1<ArCentralForwarder *> *functor, int priority = 0);
  /// Removes a callback for when a new forwarder is added
  AREXPORT void remForwarderAddedCallback(
	  ArFunctor1<ArCentralForwarder *> *functor);
  /// Adds a callback for when a new forwarder is destroyed
  AREXPORT void addForwarderRemovedCallback(
	  ArFunctor1<ArCentralForwarder *> *functor, int priority = 0);
  /// Removes a callback for when a new forwarder is destroyed
  AREXPORT void remForwarderRemovedCallback(
	  ArFunctor1<ArCentralForwarder *> *functor);	  
  /// Networking command to get the list of clients
  AREXPORT void netClientList(ArServerClient *client, ArNetPacket *packet);
  /// A callback so we can tell the main connection happened when a
  /// client is removed
  AREXPORT void forwarderServerClientRemovedCallback(
	  ArCentralForwarder *forwarder, ArServerClient *client);  
  /// A callback so we can close down other connetions when a main
  /// client loses connection
  AREXPORT void mainServerClientRemovedCallback(ArServerClient *client);  
  /// Networking command to switch the direction of a connection
  AREXPORT void netServerSwitch(ArServerClient *client, ArNetPacket *packet);
  AREXPORT virtual void *runThread(void *arg);
protected:
  void close(void);
  bool processFile(void);

  ArServerBase *myRobotServer;
  ArServerBase *myClientServer;
  double myHeartbeatTimeout;
  double myUdpHeartbeatTimeout;
  double myRobotBackupTimeout;
  double myClientBackupTimeout;

  int myMostForwarders;
  int myMostClients;

  ArTypes::UByte4 myClosingConnectionID;
  std::list<ArSocket *> myClientSockets;
  std::list<std::string> myClientNames;
  std::list<ArCentralForwarder *> myForwarders;
  std::set<int> myUsedPorts;
  ArMutex myCallbackMutex;
  std::multimap<int, 
      ArFunctor1<ArCentralForwarder *> *> myForwarderAddedCBList;
  std::multimap<int, 
      ArFunctor1<ArCentralForwarder *> *> myForwarderRemovedCBList;
  ArMutex myDataMutex;
  int myOnSocket;
  ArFunctor2C<ArCentralManager, ArServerClient *, 
      ArNetPacket *> myNetSwitchCB;
  ArFunctor2C<ArCentralManager, ArServerClient *, 
      ArNetPacket *> myNetClientListCB;
  ArFunctorC<ArCentralManager> myAriaExitCB;
  ArRetFunctorC<bool, ArCentralManager> myProcessFileCB;
  ArFunctor2C<ArCentralManager, ArCentralForwarder *, 
	      ArServerClient *> myForwarderServerClientRemovedCB;
  ArFunctor1C<ArCentralManager, ArServerClient *> myMainServerClientRemovedCB;
};


#endif // ARSERVERSWITCHMANAGER
