
This readme contains directions on how to setup your system for development as well as how to
build the libaria-android and libarnetworking-android libraries using the android NDK. These instructions
only pertain to Linux (I used Ubuntu 10.10 running in a VM). This will build aria and arnetworking 
as native android libraries that can be used from your java source.


TODO: 
-----
  * Update these instructions for current versions of Ubuntu ad Android tools/sdk. 
  * Move makefiles and other configuration to ARIA library, so libAria,
    libArNetworking and the Java wrappers can be built for Android from there,
    and this app won't need to have copies of the ARIA/ArNetworking source code
    (neither C++ or java)


Remember to modify the path for source code in jni/Android.mk

Original Build Instructions from 2011
-------------------------------------

1. Download eclipse and extract

2. Download Android SDK, extract to ~/Applications/android-sdk

3. Download Android NDK, extract to ~/Applications/android-ndk

4. [[ Is this step still neccesary?? ]] Download CrystaX custom build of Android NDK from: http://www.crystax.net/android/ndk-r4.php#download
    extract to ~/Applications/android-ndk-r4-crystax
   *This is needed because the default NDK does not contain the standard C++ library
   
5. Modify ~/.bashrc with these lines:
    set path for android

    PATH=~/Applications/android-sdk/tools:"${PATH}"
    PATH=~/Applications/android-sdk/platform-tools:"${PATH}"
    PATH=~/Applications/android-ndk-r4-crystax:"${PATH}"
	
6. From http://robots.mobilerobots.com, download and install 
   using sudo dpkg -i the following:
    ARIA library
	
7. Setup eclipse and sdk [[TODO update for more recent ubuntu version]]
    add jdk repo to apt-get:  sudo add-apt-repository "deb http://archive.canonical.com/ lucid partner"
    update apt-get:           sudo apt-get update
    install jdk:              sudo apt-get install sun-java6-jdk
    run ~/Applications/android-sdk/android to install all available packages
    start eclipse, Help->Install new software, add https://dl-ssl.google.com/android/eclipse, install all available
    in eclipse, Window->Preferences->Android, browse to sdk install location ~/Applications/android-sdk
	
8. Build the jni code
    cd ../ar_android/
	  *This should be whatever local directory you copied the source to on your machine
    ndk-build
	  *The makefile that gets called when executing ndk-build is found at ../ar_android/jni/Android.mk
	  *This will build two libraries that can be imported and used in the java source,
	  *libarnetworking-android and libaria-android. libaria-android is built statically and is included
    *in the libarnetworking-android.so build. The libraries will be copied to ../ar_android/libs/armeabi/
    
