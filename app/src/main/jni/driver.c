#include<stdio.h>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include<jni.h>
#include<fcntl.h>
#include<android/log.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define LOG_TAG "tag"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define ERROR            -1

/*
 *发送键值驱动
 *
 * */
int open_id;
struct input_event inputEvent;
struct input_event writeEvent;
struct uinput_user_dev uinp;

/*
 * 初始化 uinput 驱动
 *
 * */
JNIEXPORT jint JNICALL
Java_com_tapc_platform_jni_Driver_openUinput(JNIEnv *env, jobject instance, jstring devName) {
    const char *devNameChar = (*env)->GetStringUTFChars(env, devName, 0);
    int i;

    open_id = open(devNameChar, O_WRONLY | O_NDELAY);
    if (open_id < 0) {
        LOGE("open %s device error!\n", devNameChar);
        return ERROR;
    } else {
        LOGE("open %s device ok!\n", devNameChar);
    }

    memset(&uinp, 0, sizeof(uinp)); // Intialize the uInput device to NULL
    strncpy(uinp.name, devNameChar, sizeof(devNameChar));
    uinp.id.version = 4;
    uinp.id.bustype = BUS_USB;
    // Setup the uinput device
    ioctl(open_id, UI_SET_EVBIT, EV_KEY);
    ioctl(open_id, UI_SET_EVBIT, EV_REL);
    ioctl(open_id, UI_SET_RELBIT, REL_X);
    ioctl(open_id, UI_SET_RELBIT, REL_Y);

    for (i = 0; i < 256; i++) {
        ioctl(open_id, UI_SET_KEYBIT, i);
    }

    ioctl(open_id, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(open_id, UI_SET_KEYBIT, BTN_TOUCH);
    ioctl(open_id, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(open_id, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(open_id, UI_SET_KEYBIT, BTN_MIDDLE);
    ioctl(open_id, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(open_id, UI_SET_KEYBIT, BTN_FORWARD);
    ioctl(open_id, UI_SET_KEYBIT, BTN_BACK);

    /* Create input device into input sub-system */
    write(open_id, &uinp, sizeof(uinp));
    if (ioctl(open_id, UI_DEV_CREATE)) {
        LOGE("Unable to create UINPUT device.");
        return -1;
    }
    return open_id;
}

JNIEXPORT void JNICALL
Java_com_tapc_platform_jni_Driver_closeUinput(JNIEnv *env, jobject instance) {
    close(open_id);
}

void sendKey(__u16 KEY) {
    memset(&writeEvent, 0, sizeof(writeEvent));
    gettimeofday(&writeEvent.time, NULL);
    writeEvent.type = EV_KEY;
    writeEvent.code = KEY;
    writeEvent.value = 1;
    write(open_id, &writeEvent, sizeof(writeEvent));
    writeEvent.type = EV_SYN;
    writeEvent.code = SYN_REPORT;
    writeEvent.value = 0;
    write(open_id, &writeEvent, sizeof(writeEvent));

    memset(&writeEvent, 0, sizeof(writeEvent));
    gettimeofday(&writeEvent.time, NULL);
    writeEvent.type = EV_KEY;
    writeEvent.code = KEY;
    writeEvent.value = 0;
    write(open_id, &writeEvent, sizeof(writeEvent));
    writeEvent.type = EV_SYN;
    writeEvent.code = SYN_REPORT;
    writeEvent.value = 0;
    write(open_id, &writeEvent, sizeof(writeEvent));
}

JNIEXPORT void JNICALL
Java_com_tapc_platform_jni_Driver_backEvent(JNIEnv *env, jobject instance) {
    sendKey(KEY_BACK);
    LOGE("uinput key: %s", "back");
}

JNIEXPORT void JNICALL
Java_com_tapc_platform_jni_Driver_homeEvent(JNIEnv *env, jobject instance) {
    sendKey(KEY_HOMEPAGE);
    LOGE("uinput key: %s", "home");
}

/*
 * 初始化串口驱动
 *
 * */
JNIEXPORT jint JNICALL
Java_com_tapc_platform_jni_Driver_initCom(JNIEnv *env, jobject instance, jstring devName, jint baudRate) {
    struct termios termios_new;
    const char *devNameChar = (*env)->GetStringUTFChars(env, devName, 0);
    int setBauRate = B115200;

    int com_fd = open(devNameChar, O_RDWR);
    if (com_fd < 0) {
        LOGE("open %s device error!\n", devNameChar);
    } else {
        LOGE("open %s device ok!\n", devNameChar);
    }
    bzero(&termios_new, sizeof(termios_new));
    cfmakeraw(&termios_new);

    switch (baudRate) {
        case 2400:
            setBauRate = B2400;
            break;
        case 4800:
            setBauRate = B4800;
            break;
        case 9600:
            setBauRate = B9600;
            break;
        case 19200:
            setBauRate = B19200;
            break;
        case 38400:
            setBauRate = B38400;
            break;
        case 57600:
            setBauRate = B57600;
            break;
        case 115200:
            setBauRate = B115200;
            break;
        case 230400:
            setBauRate = B230400;
            break;
        default:
            setBauRate = B115200;
            break;
    }
    termios_new.c_cflag = (setBauRate);
    termios_new.c_cflag |= CLOCAL | CREAD;
    termios_new.c_cflag &= ~CSIZE;
    termios_new.c_cflag |= CS8;
    termios_new.c_cflag &= ~PARENB;
    termios_new.c_cflag &= ~CSTOPB;
    termios_new.c_cc[VTIME] = 0;
    termios_new.c_cc[VMIN] = 1;
    tcflush(com_fd, TCIFLUSH);
    tcsetattr(com_fd, TCSANOW, &termios_new);
    close(com_fd);
    return com_fd;
}