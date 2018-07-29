package com.tapc.platform.jni;

/**
 * Created by Administrator on 2018/7/29.
 */

public abstract class BaseDriver {
    public abstract int open(String devName);

    public abstract void close();

    public abstract int read(byte[] data, int size);

    public abstract int write(byte[] data, int size);

    public abstract int ioctl(String devName);

    public abstract void setDebug(boolean isDebug);
}
