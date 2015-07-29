# Copyright (C) 2011-2015 Adept Technology


# TODO move this out of this example and build ARIA as a 
# "prebuilt library" (see
# http://developer.android.com/ndk/guides/prebuilts.html)

LOCAL_PATH:= $(call my-dir)

ifndef ARIA
ARIA=/home/pengtang/Downloads/ARIA-src
endif

include $(CLEAR_VARS)

LOCAL_MODULE    := libAria_android
LOCAL_C_INCLUDES:=$(ARIA)/include
LOCAL_EXPORT_C_INCLUDES:=$(ARIA)/include
LOCAL_CPPFLAGS:=-fno-exceptions -Wall -fPIC
LOCAL_LDLIBS:=-ldl -lstdc++ # need -lpthread? does android have librt? or similar
LOCAL_CPP_FEATURES+=rtti
LOCAL_SRC_FILES := \
	$(ARIA)/src/ArActionStop.cpp \
	$(ARIA)/src/ArActionBumpers.cpp \
	$(ARIA)/src/ArLaserConnector.cpp \
	$(ARIA)/src/ArNMEAParser.cpp \
	$(ARIA)/src/ArThread.cpp \
	$(ARIA)/src/ArP2Arm.cpp \
	$(ARIA)/src/ArModuleLoader.cpp \
	$(ARIA)/src/ArConfigGroup.cpp \
	$(ARIA)/src/ArActionRobotJoydrive.cpp \
	$(ARIA)/src/ArPriorityResolver.cpp \
	$(ARIA)/src/ArMapComponents.cpp \
	$(ARIA)/src/ArActionKeydrive.cpp \
	$(ARIA)/src/ArArgumentBuilder.cpp \
	$(ARIA)/src/ArActionGotoStraight.cpp \
	$(ARIA)/src/ArActionGroups.cpp \
	$(ARIA)/src/ArActionInput.cpp \
	$(ARIA)/src/ArRobotConfig.cpp \
	$(ARIA)/src/ArRangeDeviceThreaded.cpp \
	$(ARIA)/src/ArLMS2xxPacket.cpp \
	$(ARIA)/src/ArActionLimiterTableSensor.cpp \
	$(ARIA)/src/ArFunctorASyncTask.cpp \
	$(ARIA)/src/ArRobotJoyHandler.cpp \
	$(ARIA)/src/ArRobotPacketReceiver.cpp \
	$(ARIA)/src/ArActionAvoidFront.cpp \
	$(ARIA)/src/ArTCM2.cpp \
	$(ARIA)/src/ArActionGroup.cpp \
	$(ARIA)/src/ArSystemStatus.cpp \
	$(ARIA)/src/ArLogFileConnection.cpp \
	$(ARIA)/src/ArDataLogger.cpp \
	$(ARIA)/src/ArSZSeries.cpp \
	$(ARIA)/src/ArRobotPacketSender.cpp \
	$(ARIA)/src/ArIrrfDevice.cpp \
	$(ARIA)/src/ArLog.cpp \
	$(ARIA)/src/ArSonarConnector.cpp \
	$(ARIA)/src/ArMode.cpp \
	$(ARIA)/src/ArLMS2xxPacketReceiver.cpp \
	$(ARIA)/src/ArPTZConnector.cpp \
	$(ARIA)/src/ArRVisionPTZ.cpp \
	$(ARIA)/src/ArActionDeceleratingLimiter.cpp \
	$(ARIA)/src/ArFileParser.cpp \
	$(ARIA)/src/ArStringInfoGroup.cpp \
	$(ARIA)/src/ArRobotPacket.cpp \
	$(ARIA)/src/ArTCMCompassRobot.cpp \
	$(ARIA)/src/ArTcpConnection.cpp \
	$(ARIA)/src/ArActionTriangleDriveTo.cpp \
	$(ARIA)/src/ArModes.cpp \
	$(ARIA)/src/ArMapObject.cpp \
	$(ARIA)/src/ArCameraCollection.cpp \
	$(ARIA)/src/ArConfig.cpp \
	$(ARIA)/src/ArRobotPacketReaderThread.cpp \
	$(ARIA)/src/ArRatioInputRobotJoydrive.cpp \
	$(ARIA)/src/ArActionLimiterRot.cpp \
	$(ARIA)/src/ArTrimbleGPS.cpp \
	$(ARIA)/src/ArRangeBuffer.cpp \
	$(ARIA)/src/ArIRs.cpp \
	$(ARIA)/src/ArMD5Calculator.cpp \
	$(ARIA)/src/ArRobotTypes.cpp \
	$(ARIA)/src/ArAMPTU.cpp \
	$(ARIA)/src/ArThread_LIN.cpp \
	$(ARIA)/src/ArRobot.cpp \
	$(ARIA)/src/ArCameraCommands.cpp \
	$(ARIA)/src/ArSick.cpp \
	$(ARIA)/src/ArRangeDevice.cpp \
	$(ARIA)/src/ArActionDesired.cpp \
	$(ARIA)/src/ArTCMCompassDirect.cpp \
	$(ARIA)/src/ArRobotBatteryPacketReader.cpp \
	$(ARIA)/src/ArGPSCoords.cpp \
	$(ARIA)/src/ArASyncTask.cpp \
	$(ARIA)/src/ariaUtil.cpp \
	$(ARIA)/src/ArRobotConnector.cpp \
	$(ARIA)/src/ArActionDriveDistance.cpp \
	$(ARIA)/src/ArLCDMTX.cpp \
	$(ARIA)/src/ArRecurrentTask.cpp \
	$(ARIA)/src/ArLineFinder.cpp \
	$(ARIA)/src/ArActionMovementParameters.cpp \
	$(ARIA)/src/ArVersalogicIO.cpp \
	$(ARIA)/src/ArBumpers.cpp \
	$(ARIA)/src/ArSpeech.cpp \
	$(ARIA)/src/ArMutex.cpp \
	$(ARIA)/src/ArForbiddenRangeDevice.cpp \
	$(ARIA)/src/ArLaserLogger.cpp \
	$(ARIA)/src/ArSyncLoop.cpp \
	$(ARIA)/src/ArDPPTU.cpp \
	$(ARIA)/src/ArMap.cpp \
	$(ARIA)/src/ArBatteryConnector.cpp \
	$(ARIA)/src/ArGPS.cpp \
	$(ARIA)/src/ArGripper.cpp \
	$(ARIA)/src/ArJoyHandler.cpp \
	$(ARIA)/src/ArArg.cpp \
	$(ARIA)/src/ArRobotConfigPacketReader.cpp \
	$(ARIA)/src/ArCondition_LIN.cpp \
	$(ARIA)/src/ArLaserReflectorDevice.cpp \
	$(ARIA)/src/ArActionGoto.cpp \
	$(ARIA)/src/ArSonarMTX.cpp \
	$(ARIA)/src/ArSerialConnection_LIN.cpp \
	$(ARIA)/src/ArSensorReading.cpp \
	$(ARIA)/src/ArUrg.cpp \
	$(ARIA)/src/ArModule.cpp \
	$(ARIA)/src/ArUrg_2_0.cpp \
	$(ARIA)/src/Aria.cpp \
	$(ARIA)/src/ArActionColorFollow.cpp \
	$(ARIA)/src/ArActionLimiterForwards.cpp \
	$(ARIA)/src/ArTransform.cpp \
	$(ARIA)/src/ArSocket.cpp \
	$(ARIA)/src/ArLaser.cpp \
	$(ARIA)/src/ArDeviceConnection.cpp \
	$(ARIA)/src/ArSonarDevice.cpp \
	$(ARIA)/src/ArMutex_LIN.cpp \
	$(ARIA)/src/ArSonyPTZ.cpp \
	$(ARIA)/src/ArActionTurn.cpp \
	$(ARIA)/src/ArMTXIO.cpp \
	$(ARIA)/src/ArActionLimiterBackwards.cpp \
	$(ARIA)/src/ArBatteryMTX.cpp \
	$(ARIA)/src/ArACTS.cpp \
	$(ARIA)/src/md5.cpp \
	$(ARIA)/src/ArSignalHandler_LIN.cpp \
	$(ARIA)/src/ArNovatelGPS.cpp \
	$(ARIA)/src/ArRatioInputJoydrive.cpp \
	$(ARIA)/src/ArBasePacket.cpp \
	$(ARIA)/src/ArInterpolation.cpp \
	$(ARIA)/src/ArRobotParams.cpp \
	$(ARIA)/src/ArSimulatedLaser.cpp \
	$(ARIA)/src/ArActionMovementParametersDebugging.cpp \
	$(ARIA)/src/ArLaserFilter.cpp \
	$(ARIA)/src/ArKeyHandler.cpp \
	$(ARIA)/src/ArConfigArg.cpp \
	$(ARIA)/src/ArActionConstantVelocity.cpp \
	$(ARIA)/src/ArAnalogGyro.cpp \
	$(ARIA)/src/ArSonarAutoDisabler.cpp \
	$(ARIA)/src/ArLMS1XX.cpp \
	$(ARIA)/src/ArSyncTask.cpp \
	$(ARIA)/src/ArLMS2xx.cpp \
	$(ARIA)/src/ArMapInterface.cpp \
	$(ARIA)/src/ArSocket_LIN.cpp \
	$(ARIA)/src/ArAction.cpp \
	$(ARIA)/src/ArPTZ.cpp \
	$(ARIA)/src/ArVCC4.cpp \
	$(ARIA)/src/ArJoyHandler_LIN.cpp \
	$(ARIA)/src/ArActionIRs.cpp \
	$(ARIA)/src/ArActionRatioInput.cpp \
	$(ARIA)/src/ArMapUtils.cpp \
	$(ARIA)/src/ArLCDConnector.cpp \
	$(ARIA)/src/ArActionJoydrive.cpp \
	$(ARIA)/src/ArRatioInputKeydrive.cpp \
	$(ARIA)/src/ArActionStallRecover.cpp \
	$(ARIA)/src/ArNetServer.cpp \
	$(ARIA)/src/ArArgumentParser.cpp \
	$(ARIA)/src/ArS3Series.cpp \
	$(ARIA)/src/ArGPSConnector.cpp \
	$(ARIA)/src/ArActionAvoidSide.cpp \
	$(ARIA)/src/ArSimpleConnector.cpp

include $(BUILD_STATIC_LIBRARY)


include $(CLEAR_VARS)

LOCAL_MODULE    := libArNetworking_android
LOCAL_C_INCLUDES:=$(ARIA)/include $(ARIA)/ArNetworking/include
LOCAL_CPPFLAGS:=-fno-exceptions -Wall -fPIC
LOCAL_LDLIBS:=-lc -ldl -lstdc++ -llog -lGLESv2 # need -lpthread? does android have librt? or similar
LOCAL_STATIC_LIBRARIES:=Aria_android
LOCAL_CPP_FEATURES+=rtti
LOCAL_SRC_FILES := \
	native_ArjRobot.cpp\
	native_gl_code.cpp\
	$(ARIA)/ArNetworking/src/ArServerUserInfo.cpp \
	$(ARIA)/ArNetworking/src/ArServerMode.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerMapping.cpp \
	$(ARIA)/ArNetworking/src/ArNetPacketSenderTcp.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerPopup.cpp \
	$(ARIA)/ArNetworking/src/ArNetPacket.cpp \
	$(ARIA)/ArNetworking/src/ArHybridForwarderVideo.cpp \
	$(ARIA)/ArNetworking/src/ArServerInfoSensor.cpp \
	$(ARIA)/ArNetworking/src/ArServerData.cpp \
	$(ARIA)/ArNetworking/src/ArCentralManager.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerCamera.cpp \
	$(ARIA)/ArNetworking/src/ArServerModeWander.cpp \
	$(ARIA)/ArNetworking/src/ArServerModeIdle.cpp \
	$(ARIA)/ArNetworking/src/ArServerFileUtils.cpp \
	$(ARIA)/ArNetworking/src/ArServerSimpleCommands.cpp \
	$(ARIA)/ArNetworking/src/ArClientSwitchManager.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerCommMonitor.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerCameraCollection.cpp \
	$(ARIA)/ArNetworking/src/ArCentralForwarder.cpp \
	$(ARIA)/ArNetworking/src/ArTempDirectoryHelper.cpp \
	$(ARIA)/ArNetworking/src/ArServerInfoDrawings.cpp \
	$(ARIA)/ArNetworking/src/ArServerModeStop.cpp \
	$(ARIA)/ArNetworking/src/ArMapChanger.cpp \
	$(ARIA)/ArNetworking/src/ArClientHandlerRobotUpdate.cpp \
	$(ARIA)/ArNetworking/src/ArNetPacketReceiverUdp.cpp \
	$(ARIA)/ArNetworking/src/ArClientSimpleConnector.cpp \
	$(ARIA)/ArNetworking/src/ArClientData.cpp \
	$(ARIA)/ArNetworking/src/ArServerModeRatioDrive.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerConfig.cpp \
	$(ARIA)/ArNetworking/src/ArClientArgUtils.cpp \
	$(ARIA)/ArNetworking/src/ArServerModeDrive.cpp \
	$(ARIA)/ArNetworking/src/ArServerClient.cpp \
	$(ARIA)/ArNetworking/src/ArServerSimpleOpener.cpp \
	$(ARIA)/ArNetworking/src/md5.cpp \
	$(ARIA)/ArNetworking/src/ArClientBase.cpp \
	$(ARIA)/ArNetworking/src/ArNetPacketReceiverTcp.cpp \
	$(ARIA)/ArNetworking/src/ArServerBase.cpp \
	$(ARIA)/ArNetworking/src/ArServerInfoRobot.cpp \
	$(ARIA)/ArNetworking/src/ArClientHandlerConfig.cpp \
	$(ARIA)/ArNetworking/src/ArServerInfoStrings.cpp \
	$(ARIA)/ArNetworking/src/ArClientFileUtils.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerMap.cpp \
	$(ARIA)/ArNetworking/src/ArServerHandlerCommands.cpp

include $(BUILD_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#
#LOCAL_MODULE    := libAriaJava_android
#LOCAL_C_INCLUDES:=$(ARIA)/include $(JAVA_INCLUDE) $(JAVA_INCLUDE)/linux
#LOCAL_CPPFLAGS:=-fno-exceptions -Wall -fPIC
#LOCAL_LDLIBS:=-lAria_android -ldl -lstdc++ # need -lpthread? does android have librt? or similar
#LOCAL_CPP_FEATURES+=rtti
#LOCAL_SRC_FILES := $(ARIA)/java/AriaJava_wrap.cpp
#
#include $(BUILD_SHARED_LIBRARY)

LOCAL_EXPORT_C_INCLUDES:=$(ARIA)/include $(ARIA)/ArNetworking/include
