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
#include "ArLog.h"
#include "ArConfig.h"
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include "ariaInternal.h"


#ifdef _ATL_VER
#include <atlbase.h>
#define HAVEATL 1
#endif // ifdef _ATL_VER

ArMutex ArLog::ourMutex;
ArLog::LogType ArLog::ourType=StdOut;
ArLog::LogLevel ArLog::ourLevel=ArLog::Normal;
FILE * ArLog::ourFP=0;
int ArLog::ourCharsLogged = 0;
std::string ArLog::ourFileName;
int ArLog::ourColbertStream = -1;
bool ArLog::ourLoggingTime = false;
bool ArLog::ourAlsoPrint = false;
AREXPORT void (* ArLog::colbertPrint)(int i, const char *str);

ArLog::LogType ArLog::ourConfigLogType = ArLog::StdOut;
ArLog::LogLevel ArLog::ourConfigLogLevel = ArLog::Normal;
char ArLog::ourConfigFileName[1024] = "log.txt";
bool ArLog::ourConfigLogTime = false;
bool ArLog::ourConfigAlsoPrint = false;
ArGlobalRetFunctor<bool> ArLog::ourConfigProcessFileCB(&ArLog::processFile);

char ArLog::ourAramConfigLogLevel[1024] = "Normal";
double ArLog::ourAramConfigLogSize = 10;
bool ArLog::ourUseAramBehavior = false;
double ArLog::ourAramLogSize = 0;
ArGlobalRetFunctor<bool> ArLog::ourAramConfigProcessFileCB(
	&ArLog::aramProcessFile);
std::string ArLog::ourAramPrefix = "";

AREXPORT void ArLog::logPlain(LogLevel level, const char *str)
{
  log(level, str);
}

/**
   This function is used like printf(). If the supplied level is less than
   or equal to the set level, it will be printed.
   @param level level of logging
   @param str printf() like formating string
*/
AREXPORT void ArLog::log(LogLevel level, const char *str, ...)
{
  if (level > ourLevel)
    return;

  //printf("logging %s\n", str);

  char buf[10000];
  char *bufPtr;
  char *timeStr;
  int timeLen = 0; // this is a value based on the standard length of
                       // ctime return
  time_t now;


  ourMutex.lock();
  // put our time in if we want it
  if (ourLoggingTime)
  {
    now = time(NULL);
    timeStr = ctime(&now);
    timeLen = 20;
    // get take just the portion of the time we want
    strncpy(buf, timeStr, timeLen);
    buf[timeLen] = '\0';
    bufPtr = &buf[timeLen];
  }
  else
    bufPtr = buf;
  va_list ptr;
  va_start(ptr, str);
  
  vsnprintf(bufPtr, sizeof(buf) - timeLen - 2, str, ptr);
  bufPtr[sizeof(buf) - timeLen - 1] = '\0';
  //vsprintf(bufPtr, str, ptr);
  // can do whatever you want with the buf now
  if (ourType == Colbert)
  {
    if (colbertPrint)		// check if we have a print routine
      (*colbertPrint)(ourColbertStream, buf);
  }
  else if (ourFP)
  {
    int written;
    if ((written = fprintf(ourFP, "%s\n", buf)) > 0)
      ourCharsLogged += written;
    fflush(ourFP);
  }
  else if (ourType != None)
  {
    printf("%s\n", buf);
    fflush(stdout);
  }
  if (ourAlsoPrint)
    printf("%s\n", buf);

  // check this down here instead of up in the if ourFP so that the log filled shows up after the printf
  if (ourUseAramBehavior && ourFP && ourAramLogSize > 0 && 
      ourCharsLogged > ourAramLogSize)
  {
    filledAramLog();
  }
  
// Also send it to the VC++ debug output window...
#ifdef HAVEATL
  ATLTRACE2("%s\n", buf);
#endif
  
  va_end(ptr);
  ourMutex.unlock();
}

/**
   Initialize the logging utility by supplying the type of logging and the
   level of logging. If the type is File, the fileName needs to be supplied.
   @param type type of Logging
   @param level level of logging
   @param fileName the name of the file for File type of logging
   @param logTime if this is true then the time a message is given will be logged
   @param alsoPrint if this is true then in addition to whatever other logging (to a file for instance) the results will also be printed
   @param printThisCall if this is true the new settings will be printed otherwise they won't
*/
AREXPORT bool ArLog::init(LogType type, LogLevel level, const char *fileName,
			  bool logTime, bool alsoPrint, bool printThisCall)
{
  ourMutex.setLogName("ArLog::ourMutex");
  ourMutex.lock();
  
  // if we weren't or won't be doing a file then close any old file
  if (ourType != File || type != File)
  {
    close();
  }
  
  if (type == StdOut)
    ourFP=stdout;
  else if (type == StdErr)
    ourFP=stderr;
  else if (type == File)
  {
    if (fileName != NULL)
    {
      if (strcmp(ourFileName.c_str(), fileName) == 0)
      {
	ArLog::logNoLock(ArLog::Terse, "ArLog::init: Continuing to log to the same file.");
      }
      else
      {
	close();
	if ((ourFP = ArUtil::fopen(fileName, "w")) == NULL)
	{
	  ArLog::logNoLock(ArLog::Terse, "ArLog::init: Could not open file %s for logging.", fileName);
	  ourMutex.unlock();
	  return(false);
	}
	ourFileName=fileName;
      }
    }
  }
  else if (type == Colbert)
  {
    colbertPrint = NULL;
    ourColbertStream = -1;	// use default stream
    if (fileName)
    {  // need to translate fileName to integer index
    }
  }
  else if (type == None)
  {

  }
  ourType=type;
  ourLevel=level;

  // environment variable overrides level
  {
    char* lev = getenv("ARLOG_LEVEL");
    if(lev)
    {
      switch(toupper(lev[0]))
      {
        case 'N':
          ourLevel = Normal;
          break;
        case 'T':
          ourLevel = Terse;
          break;
        case 'V':
          ourLevel = Verbose;
          break;
       }
    }
  }

  ourLoggingTime = logTime;
  ourAlsoPrint = alsoPrint;

  if (printThisCall)
  {
    printf("ArLog::init: ");
    
    if (ourType == StdOut)
      printf(" StdOut\t");
    else if (ourType == StdErr)
      printf(" StdErr\t");
    else if (ourType == File)
      printf(" File(%s)\t", ourFileName.c_str());
    else if (ourType == Colbert)
      printf(" Colbert\t");
    else if (ourType == None)
      printf(" None\t");
    else
      printf(" BadType\t");
    
    if (ourLevel == Terse)
      printf(" Terse\t");
    else if (ourLevel == Normal)
      printf(" Normal\t");
    else if (ourLevel == Verbose)
      printf(" Verbose\t");
    else
      printf(" BadLevel\t");
    
    if (ourLoggingTime)
      printf(" Logging Time\t");
    else
      printf(" Not logging time\t");
    
    if (ourAlsoPrint)
      printf(" Also printing\n");
    else
      printf(" Not also printing\n");
  }
  ourMutex.unlock();
  return(true);
}

AREXPORT void ArLog::close()
{
  if (ourFP && (ourType == File))
  {
    fclose(ourFP);
    ourFP=0;
    ourFileName="";
  }
}

AREXPORT void ArLog::logNoLock(LogLevel level, const char *str, ...)
{
  if (level > ourLevel)
    return;

  char buf[2048];
  char *bufPtr;
  char *timeStr;
  int timeLen = 20; // this is a value based on the standard length of
                       // ctime return
  time_t now;

  
  // put our time in if we want it
  if (ourLoggingTime)
  {
    now = time(NULL);
    timeStr = ctime(&now);
    // get take just the portion of the time we want
    strncpy(buf, timeStr, timeLen);
    buf[timeLen] = '\0';
    bufPtr = &buf[timeLen];
  }
  else
    bufPtr = buf;
  va_list ptr;
  va_start(ptr, str);
  //vsnprintf(bufPtr, sizeof(buf) - timeLen - 1, str, ptr);
  vsprintf(bufPtr, str, ptr);
  // can do whatever you want with the buf now
  if (ourType == Colbert)
  {
    if (colbertPrint)		// check if we have a print routine
      (*colbertPrint)(ourColbertStream, buf);
  }
  else if (ourFP)
  {
    int written;
    if ((written = fprintf(ourFP, "%s\n", buf)) > 0)
      ourCharsLogged += written;
    fflush(ourFP);
  }
  else if (ourType != None)
    printf("%s\n", buf);
  if (ourAlsoPrint)
    printf("%s\n", buf);
  va_end(ptr);
}


AREXPORT void ArLog::addToConfig(ArConfig *config)
{
  std::string section = "LogConfig";
  config->addParam(
	  ArConfigArg("LogType", (int *)&ourConfigLogType,
		      "The type of log we'll be using, 0 for StdOut, 1 for StdErr, 2 for File (and give it a file name), 3 for colbert (don't use that), and 4 for None", 
		      ArLog::StdOut, ArLog::None), 
	  section.c_str(), ArPriority::TRIVIAL);
  config->addParam(
	  ArConfigArg("LogLevel", (int *)&ourConfigLogLevel,
		      "The level of logging to do, 0 for Terse, 1 for Normal, and 2 for Verbose", 
		      ArLog::Terse, ArLog::Verbose), 
	  section.c_str(), ArPriority::TRIVIAL);
  config->addParam(
	  ArConfigArg("LogFileName", ourConfigFileName,
		      "File to log to", sizeof(ourConfigFileName)),
	  section.c_str(), ArPriority::TRIVIAL);
  config->addParam(
	  ArConfigArg("LogTime", &ourConfigLogTime,
		      "True to prefix log messages with time and date, false not to"),
	  section.c_str(), ArPriority::TRIVIAL);
  config->addParam(
	  ArConfigArg("LogAlsoPrint", &ourConfigAlsoPrint,
		      "True to also printf the message, false not to"),	  
	  section.c_str(), ArPriority::TRIVIAL);
  ourConfigProcessFileCB.setName("ArLog");
  config->addProcessFileCB(&ourConfigProcessFileCB, 200);
}

AREXPORT bool ArLog::processFile(void)
{
  if (ourConfigLogType != ourType || ourConfigLogLevel != ourLevel ||
      strcmp(ourConfigFileName, ourFileName.c_str()) != 0 || 
      ourConfigLogTime != ourLoggingTime || ourConfigAlsoPrint != ourAlsoPrint)
  {
    ArLog::logNoLock(ArLog::Normal, "Initializing log from config");
    return ArLog::init(ourConfigLogType, ourConfigLogLevel, ourConfigFileName, 
		       ourConfigLogTime, ourConfigAlsoPrint, true);
  }
  return true;
}


AREXPORT void ArLog::aramInit(const char *prefix, ArLog::LogLevel defaultLevel,
			      double defaultSize)
{
  if (prefix == NULL || prefix[0] == '\0')
    ourAramPrefix = "";
  else
  {
    ourAramPrefix = prefix;
    if (prefix[strlen(prefix) - 1] != '/')
      ourAramPrefix += "/";
  }
  
  std::string section = "Log Config";
  Aria::getConfig()->addParam(
	  ArConfigArg("Level", ourAramConfigLogLevel,
		      "The level of logging type of log we'll be using", sizeof(ourAramConfigLogLevel)),
	  section.c_str(), ArPriority::TRIVIAL, 
	  "Choices:Terse;;Normal;;Verbose");
  Aria::getConfig()->addParam(
	  ArConfigArg("LogFileSize", &ourAramConfigLogSize,
		      "The maximum size of the log files (6 files are rotated through), 0 means no maximum", 0, 20000),
	  section.c_str(), ArPriority::TRIVIAL);

  ourUseAramBehavior = true;
  ourAramConfigProcessFileCB.setName("ArLogAram");
  Aria::getConfig()->addProcessFileCB(&ourAramConfigProcessFileCB, 210);

  if (defaultLevel == ArLog::Terse)
    sprintf(ourAramConfigLogLevel, "Terse");
  else if (defaultLevel == ArLog::Normal)
    sprintf(ourAramConfigLogLevel, "Normal");
  if (defaultLevel == ArLog::Verbose)
    sprintf(ourAramConfigLogLevel, "Verbose");

  char buf[2048];
  snprintf(buf, sizeof(buf), "%slog1.txt", ourAramPrefix.c_str());
  ArLog::init(ArLog::File, defaultLevel, buf, true, true, true);

  ourAramConfigLogSize  = defaultSize;  // even megabytes
  ourAramLogSize = ArMath::roundInt(ourAramConfigLogSize * 1000000);  // even megabytes
}

AREXPORT bool ArLog::aramProcessFile(void)
{
  ourMutex.lock();
  ourAramLogSize = ArMath::roundInt(ourAramConfigLogSize * 1000000);  // even megabytes
  if (strcasecmp(ourAramConfigLogLevel, "Terse") == 0)
    ourLevel = Terse;
  else if (strcasecmp(ourAramConfigLogLevel, "Verbose") == 0)
    ourLevel = Verbose;
  else if (strcasecmp(ourAramConfigLogLevel, "Normal") == 0)
    ourLevel = Normal;
  else 
  {
    ArLog::logNoLock(ArLog::Normal, 
	       "ArLog: Bad log level '%s' defaulting to Normal", 
	       ourAramConfigLogLevel);
    ourLevel = Normal;
  }
  ourMutex.unlock();
  return true;
}

AREXPORT void ArLog::filledAramLog(void)
{
  ArLog::logNoLock(ArLog::Normal, "ArLog: Log filled, starting new file");

  fclose(ourFP);

  char buf[2048];
  int i;
  for (i = 5; i > 0; i--)
  {
    snprintf(buf, sizeof(buf), "mv %slog%d.txt %slog%d.txt", 
	     ourAramPrefix.c_str(), i, ourAramPrefix.c_str(), i+1);
    system(buf);
  }

  snprintf(buf, sizeof(buf), "%slog1.txt", ourAramPrefix.c_str());
  if ((ourFP = ArUtil::fopen(ourFileName.c_str(), "w")) == NULL)
  {
    ourType = StdOut;
    ArLog::logNoLock(ArLog::Normal, 
	       "ArLog: Couldn't reopen file '%s', failing back to stdout",
	       ourFileName.c_str());
  }
  ourCharsLogged = 0;
}
