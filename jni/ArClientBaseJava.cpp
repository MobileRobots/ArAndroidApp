
#include <jni.h>
#include <string.h>
#include <android/log.h>

#define DEBUG_TAG "MainActivity"

extern "C" {
	JNIEXPORT void JNICALL
	Java_com_adept_arnetworkingandroid_test(JNIEnv * env, jobject obj);
};


JNIEXPORT void JNICALL
Java_com_adept_arnetworkingandroid_test(JNIEnv *env, jobject obj)
{
	
}

