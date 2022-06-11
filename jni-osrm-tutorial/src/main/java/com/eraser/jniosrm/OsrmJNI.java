package com.eraser.jniosrm;

public class OsrmJNI {

    // private native int getOsrmRoute(double fromLongitude, double fromLatitude,
    // double toLongitude, double toLatitude);

    // public int returnOsrmRoute(double fromLongitude, double fromLatitude, double
    // toLongitude, double toLatitude) {
    // return getOsrmRoute(fromLongitude, fromLatitude, toLongitude, toLatitude);
    // }

    // 애초에 response를 반환하게 하지 말고, response를 넘겨 버리는 게 나은가?

    private native OsrmResponse getOsrmResponse(double fromLongitude, double fromLatitude, double toLongitude,
            double toLatitude);

    private native long getOsrmPointer();

    public OsrmResponse returnOsrmResponse(double fromLongitude, double fromLatitude, double toLongitude,
            double toLatitude) {
        return getOsrmResponse(fromLongitude, fromLatitude, toLongitude, toLatitude);
    }

    public long returnOsrmPointer() {
        return getOsrmPointer();
    }

    // load library as static
    static {
        // System.loadLibrary("/home/eraser/projects/jniosrm/src/src++/osrmjniexample.so");
        System.load("/home/eraser/projects/osrm-jni/jni-osrm-tutorial/src/src++/libosrmjni.so");
    }
}