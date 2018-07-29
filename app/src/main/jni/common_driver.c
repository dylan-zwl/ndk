#include<stdio.h>
#include<unistd.h>
#include<jni.h>
#include<fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <android/log.h>

#define LOG_TAG "ndk"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define ERROR            -1

int open_id;
const char *devNameChar;
int DEBUG = 1;

JNIEXPORT jint JNICALL
Java_com_tapc_platform_jni_CommonDriver_open(JNIEnv *env, jobject instance, jstring devName) {
    devNameChar = (*env)->GetStringUTFChars(env, devName, 0);
    open_id = open(devNameChar, O_WRONLY | O_NDELAY);
    if (open_id < 0) {
        LOGE("open %s device error!\n", devNameChar);
        return ERROR;
    } else {
        LOGE("open %s device ok!\n", devNameChar);
    }
    return open_id;
}

JNIEXPORT void JNICALL
Java_com_tapc_platform_jni_CommonDriver_close(JNIEnv *env, jobject instance) {
    close(open_id);
    LOGE("close %s device ok!\n", devNameChar);
}

JNIEXPORT jint JNICALL
Java_com_tapc_platform_jni_CommonDriver_readInt(JNIEnv *env, jobject instance) {
    char buf[1];
    return read(open_id, buf, 1);
}

JNIEXPORT jbyteArray JNICALL
Java_com_tapc_platform_jni_CommonDriver_read(JNIEnv *env, jobject instance, jint size) {
    char buf[size];
    read(open_id, buf, size);
    jbyteArray data = (*env)->NewByteArray(env, size);
    (*env)->SetByteArrayRegion(env, data, 0, size, buf);
    return data;
}

JNIEXPORT jint JNICALL
Java_com_tapc_platform_jni_CommonDriver_write(JNIEnv *env, jobject instance, jbyteArray array, jint size) {
    jbyte *bytes = (*env)->GetByteArrayElements(env, array, 0);
    return write(open_id, bytes, size);
}

JNIEXPORT jint JNICALL
Java_com_tapc_platform_jni_CommonDriver_ioctl(JNIEnv *env, jobject instance, jint cmd, jint data) {
    return ioctl(open_id, cmd, data);
}

JNIEXPORT void JNICALL
Java_com_tapc_platform_jni_CommonDriver_setDebug(JNIEnv *env, jobject instance, jboolean isDebug) {
    if (isDebug) {
        DEBUG = 1;
    } else {
        DEBUG = 0;
    }
}

