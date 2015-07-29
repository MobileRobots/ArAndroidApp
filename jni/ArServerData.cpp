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
#include "Aria.h"
#include "ArExport.h"
#include "ArServerData.h"
#include "ArServerClient.h"
#include "ArNetPacket.h"

AREXPORT ArServerData::ArServerData(
	const char *name, const char *description, unsigned int command,
	ArFunctor2<ArServerClient *, ArNetPacket *> *functor,
	const char *argumentDescription, const char *returnDescription,
	const char *commandGroup, const char *dataFlags,
	ArRetFunctor1<long, unsigned int> *getFrequencyFunctor,
	ArFunctor2<long, unsigned int> *requestChangedFunctor,
	ArFunctor2<ArServerClient *, ArNetPacket *> 
	*requestOnceFunctor) : 
  myDataFlagsBuilder(512, '|')
{ 
  myDataMutex.setLogName("ArServerData::myDataMutex");
  myName = name; 
  myDescription = description; 
  myReturnDescription = returnDescription;
  myArgumentDescription = argumentDescription;
  myCommand = command;
  myFunctor = functor; 
  if (commandGroup != NULL)
    myCommandGroup = commandGroup;
  else
    myCommandGroup = "";
  myGetFrequencyFunctor = getFrequencyFunctor;
  myRequestChangedFunctor = requestChangedFunctor;
  myRequestOnceFunctor = requestOnceFunctor;
  if (dataFlags != NULL)
  {
    myDataFlagsBuilder.add(dataFlags);
  }

  mySlowPacket = hasDataFlag("SLOW_PACKET");
  myIdlePacket = hasDataFlag("IDLE_PACKET");
}

AREXPORT ArServerData::~ArServerData()
{

}

AREXPORT bool ArServerData::hasDataFlag(const char *dataFlag)
{
  myDataMutex.lock();
  size_t i;
  for (i = 0; i < myDataFlagsBuilder.getArgc(); i++)
  {
    if (strcmp(myDataFlagsBuilder.getArg(i), dataFlag) == 0)
    {
      myDataMutex.unlock();
      return true;
    }
  }

  myDataMutex.unlock();
  return false;
}

AREXPORT bool ArServerData::addDataFlags(const char *dataFlags)
{
  myDataMutex.lock();
  myDataFlagsBuilder.add(dataFlags);
  myDataMutex.unlock();
  return true;
}

AREXPORT bool ArServerData::remDataFlag(const char *dataFlag)
{
  myDataMutex.lock();
  size_t i;
  for (i = myDataFlagsBuilder.getArgc(); i < myDataFlagsBuilder.getArgc(); i++)
  {
    if (strcmp(myDataFlagsBuilder.getArg(i), dataFlag) == 0)
    {
      myDataFlagsBuilder.removeArg(i);
      myDataMutex.unlock();
      return true;
    }
  }

  myDataMutex.unlock();
  return false;
}

AREXPORT void ArServerData::callRequestChangedFunctor(void)
{ 
  if (myGetFrequencyFunctor == NULL || myRequestChangedFunctor == NULL)
    return;
  myRequestChangedFunctor->invoke(
	  myGetFrequencyFunctor->invokeR(myCommand), myCommand);
}
