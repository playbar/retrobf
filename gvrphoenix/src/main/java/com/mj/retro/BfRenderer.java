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
    private RetroRender retroRender;

    public BfRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext){
        nativePtr = nativeCreateRenderer(appClassLoader, context, nativeGvrContext);
    }

    public void setRetroRender(RetroRender render){
        retroRender = render;
    }

    public void onPause()
    {
        nativeOnPause(nativePtr);
    }

    public void onResume()
    {
        nativeOnResume(nativePtr);
    }


    public void onTriggerEvent()
    {
        nativeOnTriggerEvent(nativePtr);
    }

    public void destroyRenderer()
    {
        nativeDestroyRenderer(nativePtr);
        nativePtr = 0;
    }
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInitializeGl(nativePtr);
        retroRender.retroInit();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChange(nativePtr, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame(nativePtr);
    }


    private native long nativeCreateRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext);
    private native void nativeDestroyRenderer(long nativePtr);
    private native void nativeInitializeGl(long nativePtr);
    private native void nativeSurfaceChange(long nativePtr, int width, int height);
    private native long nativeDrawFrame(long nativePtr);
    private native void nativeOnTriggerEvent(long nativePtr);
    private native void nativeOnPause(long nativePtr);
    private native void nativeOnResume(long nativePtr);

}
