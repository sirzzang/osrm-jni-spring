package com.eraser.jniosrm;

public class OsrmJNI {

    private native int getOsrmRoute(double fromLongitude, double fromLatitude, double toLongitude, double toLatitude);

    public int returnOsrmRoute(double fromLongitude, double fromLatitude, double toLongitude, double toLatitude) {
        return getOsrmRoute(fromLongitude, fromLatitude, toLongitude, toLatitude);
    }

    // load library as static
    static {
        // System.loadLibrary("/home/eraser/projects/osrm-jni/jniosrm/src/src++/osrmjniexample.so");
        System.load("/home/eraser/projects/osrm-jni/jniosrm/src/src++/build/libosrmjni-example.so");
    }
}