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
#ifndef NLSERVERCOMMANDS_H
#define NLSERVERCOMMANDS_H

/**
   The commands from the server to the client
**/

class ArServerCommands
{
public:
  enum ServerCommands {
    SHUTDOWN = 1, ///< Closes the connection
    INTRODUCTION = 2, ///< Introduces the server to the client
    UDP_INTRODUCTION = 3, ///< Udp introduction of the server to the client
    UDP_CONFIRMATION = 4, ///< Confirmation Udp was received from client
    CONNECTED = 5, ///< Server accepts clients connection
    REJECTED = 6, ///< Server rejects clients connection, has a byte2, then a string.... these reasons (1 = bad username password, string then is empty, 2 = rejecting connection because using central server, string then is central server IP)
    TCP_ONLY = 7, ///< Server tells client to only send TCP
    LIST = 129, ///< Map of the string names for a type to a number along with a long description of the data type
    LISTSINGLE = 130, ///< Map of a single type to a number (for late additions to server) along with its description
    LISTARGRET = 131, ///< Map of the number to their arguments and returns descriptions
    LISTARGRETSINGLE = 132, ///< Map of a single type to a number (for late additions to server) along with its argument and return descriptions
    LISTGROUPANDFLAGS = 133, ///< Map of the number to their command groups and data flags
    LISTGROUPANDFLAGSSINGLE = 134 ///< Map of a single type to a number (for late additions to server) along with its command group and data flags
  };
};

#endif // NLSERVERCOMMANDS_H
