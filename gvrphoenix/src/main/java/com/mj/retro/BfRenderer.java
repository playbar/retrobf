package com.mj.retro;

import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by houguoli on 2017/6/28.
 */

public class BfRenderer implements Renderer {
    private long nativePtr;

    public BfRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext){
        nativePtr = nativeCreateRenderer(appClassLoader, context, nativeGvrContext);
    }

    public void onPause()
    {
        nativeOnPause(nativePtr);
    }

    public void onResume()
    {
        nativeOnResume(nativePtr);
    }

    public void setPath(String strPath){
        nativeSetPath(strPath);
    }

    public void onTriggerEvent()
    {
        nativeOnTriggerEvent(nativePtr);
        nativePtr = 0;
    }

    public void destroyRenderer()
    {
        nativeDestroyRenderer(nativePtr);
    }
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInitializeGl(nativePtr);
        nativeRetroInit(nativePtr);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChange(nativePtr, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame(nativePtr);
    }


    public void dispatchMotionEvent(int source, int id,
                                    float x, float y, float z, float rz, float hatx, float haty,
                                    float ltrig, float rtrig, float brake, float gas  )
    {
        nativeDispatchMotionEvent(nativePtr, source, id, x, y, z, rz, hatx, haty, ltrig, rtrig, brake, gas);
    }

    public void dispatchKeyEvent(int source, int id, int keycode, int action, int mate){
        nativeDispatchKeyEvent(nativePtr, source, id, keycode, action, mate);
    }


    private native void nativeDispatchMotionEvent(long nativePtr, int source, int id,
                                                  float x, float y, float z, float rz, float hatx, float haty,
                                                  float ltrig, float rtrig, float brake, float gas  );
    private native void nativeDispatchKeyEvent(long nativePtr, int source, int id, int keycode, int action, int mate);
    private native void nativeSetPath(String strPath);
    private native long nativeCreateRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext);
    private native void nativeDestroyRenderer(long nativePtr);
    private native void nativeInitializeGl(long nativePtr);
    private native void nativeSurfaceChange(long nativePtr, int width, int height);
    private native long nativeDrawFrame(long nativePtr);
    private native void nativeRetroInit(long nativePtr);
    private native void nativeRetroSurfaceChange(long nativePtr, int width, int height );
    private native void nativeRetroDrawFrame(long nativePtr);
    private native void nativeOnTriggerEvent(long nativePtr);
    private native void nativeOnPause(long nativePtr);
    private native void nativeOnResume(long nativePtr);

}
