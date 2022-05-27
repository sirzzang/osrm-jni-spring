package com.eraser.jniosrm;

public class OsrmJNI {

    private native int main();

    public int returnMain() {
        return main();
    }

    // load library as static
    static {
        // System.loadLibrary("/home/eraser/projects/osrm-jni/jniosrm/src/src++/osrmjniexample.so");
        System.load("/home/eraser/projects/osrm-jni/jniosrm/src/src++/build/libosrmjni-example.so");
    }
}