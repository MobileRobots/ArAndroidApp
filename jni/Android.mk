# Copyright (C) 2011 Adept Technology
#
# This makefile will build 2 libraries, along with the native code
# needed for the arandroid project. See ../ar_android/readme.txt for
# more detailed information on how to use this file.
#

LOCAL_PATH:= $(call my-dir)

# first lib is aria-android, which will be built statically
#
include $(CLEAR_VARS)

LOCAL_MODULE    := libaria-android
LOCAL_LDLIBS	:= -lc -ldl
LOCAL_SRC_FILES := \
	pthread_android_additions.cpp \
	ArAction.cpp \
	ArActionAvoidFront.cpp \
	ArActionAvoidSide.cpp \
	ArActionBumpers.cpp \
	ArActionColorFollow.cpp \
	ArActionConstantVelocity.cpp \
	ArActionDeceleratingLimiter.cpp \
	ArActionDesired.cpp \
	ArActionDriveDistance.cpp \
	ArActionGoto.cpp \
	ArActionGotoStraight.cpp \
	ArActionGroup.cpp \
	ArActionGroups.cpp \
	ArActionInput.cpp \
	ArActionIRs.cpp \
	ArActionJoydrive.cpp \
	ArActionKeydrive.cpp \
	ArActionLimiterBackwards.cpp \
	ArActionLimiterForwards.cpp \
	ArActionLimiterTableSensor.cpp \
	ArActionMovementParameters.cpp \
	ArActionRatioInput.cpp \
	ArActionRobotJoydrive.cpp \
	ArActionStallRecover.cpp \
	ArActionStop.cpp \
	ArActionTriangleDriveTo.cpp \
	ArActionTurn.cpp \
	ArACTS.cpp \
	ArAMPTU.cpp \
	ArAnalogGyro.cpp \
	ArArg.cpp \
	ArArgumentBuilder.cpp \
	ArArgumentParser.cpp \
	ArASyncTask.cpp \
	ArBasePacket.cpp \
	ArBumpers.cpp\
	ArCameraCollection.cpp \
	ArCameraCommands.cpp \
	ArCondition_LIN.cpp \
	ArConfig.cpp \
	ArConfigArg.cpp \
	ArConfigGroup.cpp \
	ArDataLogger.cpp \
	ArDeviceConnection.cpp \
	ArDPPTU.cpp \
	ArFileParser.cpp \
	ArForbiddenRangeDevice.cpp \
	ArFunctorASyncTask.cpp \
	ArGPS.cpp \
	ArGPSConnector.cpp \
	ArGPSCoords.cpp \
	ArGripper.cpp \
	Aria.cpp \
	ariaUtil.cpp \
	ArInterpolation.cpp \
	ArIrrfDevice.cpp \
	ArIRs.cpp \
	ArJoyHandler.cpp \
	ArJoyHandler_LIN.cpp \
	ArKeyHandler.cpp \
	ArLaser.cpp \
	ArLaserConnector.cpp \
	ArLaserFilter.cpp \
	ArLaserLogger.cpp \
	ArLaserReflectorDevice.cpp \
	ArLineFinder.cpp \
	ArLMS1XX.cpp \
	ArLMS2xx.cpp \
	ArLMS2xxPacket.cpp \
	ArLMS2xxPacketReceiver.cpp \
	ArLog.cpp \
	ArLogFileConnection.cpp \
	ArMap.cpp \
	ArMapComponents.cpp \
	ArMapInterface.cpp \
	ArMapObject.cpp \
	ArMapUtils.cpp \
	ArMD5Calculator.cpp \
	ArMode.cpp \
	ArModes.cpp \
	ArModuleLoader.cpp \
	ArMutex.cpp \
	ArMutex_LIN.cpp \
	ArNetServer.cpp \
	ArNMEAParser.cpp \
	ArNovatelGPS.cpp \
	ArP2Arm.cpp \
	ArPriorityResolver.cpp \
	ArPTZ.cpp \
	ArRangeBuffer.cpp \
	ArRangeDevice.cpp \
	ArRangeDeviceThreaded.cpp \
	ArRatioInputJoydrive.cpp \
	ArRatioInputRobotJoydrive.cpp \
	ArRatioInputKeydrive.cpp \
	ArRecurrentTask.cpp \
	ArRobot.cpp \
	ArRobotConfig.cpp \
	ArRobotConfigPacketReader.cpp \
	ArRobotConnector.cpp \
	ArRobotJoyHandler.cpp \
	ArRobotPacket.cpp \
	ArRobotPacketSender.cpp \
	ArRobotPacketReceiver.cpp \
	ArRobotParams.cpp \
	ArRobotTypes.cpp \
	ArRVisionPTZ.cpp \
	ArSensorReading.cpp \
	ArSerialConnection_LIN.cpp \
	ArSick.cpp \
	ArSignalHandler_LIN.cpp \
	ArSimpleConnector.cpp \
	ArSimulatedLaser.cpp \
	ArSocket.cpp \
	ArSocket_LIN.cpp \
	ArSonarAutoDisabler.cpp \
	ArSonarDevice.cpp \
	ArSonyPTZ.cpp \
	ArSpeech.cpp \
	ArStringInfoGroup.cpp \
	ArSyncLoop.cpp \
	ArSyncTask.cpp \
	ArSystemStatus.cpp \
	ArTCM2.cpp \
	ArTCMCompassDirect.cpp \
	ArTCMCompassRobot.cpp \
	ArTcpConnection.cpp \
	ArThread.cpp \
	ArThread_LIN.cpp \
	ArTransform.cpp \
	ArTrimbleGPS.cpp \
	ArUrg.cpp \
	ArVCC4.cpp \
	ArVersalogicIO.cpp \
	md5.cpp \

include $(BUILD_STATIC_LIBRARY)

# second lib is arnetworking-android, which will depend on and include the
# first one. It also includes the native code.
#
include $(CLEAR_VARS)

LOCAL_MODULE    := libarnetworking-android
LOCAL_LDLIBS	:= -lc -ldl -llog -lGLESv2
LOCAL_SRC_FILES := \
	native_gl_code.cpp \
	native_ArjRobot.cpp \
	ArCentralForwarder.cpp \
	ArCentralManager.cpp \
	ArClientArgUtils.cpp \
	ArClientBase.cpp \
	ArClientData.cpp \
	ArClientFileUtils.cpp \
	ArClientHandlerConfig.cpp \
	ArClientSimpleConnector.cpp \
	ArClientSwitchManager.cpp \
	ArHybridForwarderVideo.cpp \
	ArMapChanger.cpp \
	ArNetPacket.cpp \
	ArNetPacketReceiverTcp.cpp \
	ArNetPacketReceiverUdp.cpp \
	ArNetPacketSenderTcp.cpp \
	ArServerBase.cpp \
	ArServerClient.cpp \
	ArServerData.cpp \
	ArServerFileUtils.cpp \
	ArServerHandlerCamera.cpp \
	ArServerHandlerCameraCollection.cpp \
	ArServerHandlerCommands.cpp \
	ArServerHandlerCommMonitor.cpp \
	ArServerHandlerConfig.cpp \
	ArServerHandlerMap.cpp \
	ArServerHandlerMapping.cpp \
	ArServerHandlerPopup.cpp \
	ArServerInfoDrawings.cpp \
	ArServerInfoRobot.cpp \
	ArServerInfoSensor.cpp \
	ArServerInfoStrings.cpp \
	ArServerMode.cpp \
	ArServerModeDrive.cpp \
	ArServerModeIdle.cpp \
	ArServerModeRatioDrive.cpp \
	ArServerModeStop.cpp \
	ArServerModeWander.cpp \
	ArServerSimpleCommands.cpp \
	ArServerSimpleOpener.cpp \
	ArServerUserInfo.cpp \
	md5.cpp \

LOCAL_STATIC_LIBRARIES := libaria-android

include $(BUILD_SHARED_LIBRARY)
