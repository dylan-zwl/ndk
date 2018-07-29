package com.tapc.platform.jni;

/**
 * Created by Administrator on 2017/11/2.
 */

public class CommonDriver {
    static {
        System.loadLibrary("common_driver");
    }

    public native int open(String devName);

    public native void close();

    public native int readInt();

    public native byte[] read(int size);

    public native int write(byte[] data, int size);

    public native int ioctl(int cmd, int data);

    public native void setDebug(boolean isDebug);
}
