package com.retroarch.browser.retroactivity;

import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by houguoli on 2017/6/28.
 */

public class BfRenderer implements Renderer {
    private long nativeTreasureHuntRenderer;

    public BfRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext){
        nativeTreasureHuntRenderer = nativeCreateRenderer(appClassLoader, context, nativeGvrContext);
    }

    public void onPause()
    {
        nativeOnPause(nativeTreasureHuntRenderer);
    }

    public void onResume()
    {
        nativeOnResume(nativeTreasureHuntRenderer);
    }

    public void setPath(String strPath){
        nativeSetPath(strPath);
    }

    public void onTriggerEvent()
    {
        nativeOnTriggerEvent(nativeTreasureHuntRenderer);
        nativeTreasureHuntRenderer = 0;
    }

    public void destroyRenderer()
    {
        nativeDestroyRenderer(nativeTreasureHuntRenderer);
    }
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        nativeInitializeGl(nativeTreasureHuntRenderer);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        nativeSurfaceChange(nativeTreasureHuntRenderer, width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        nativeDrawFrame(nativeTreasureHuntRenderer);
    }

    private native void nativeDispatchKeyEvent(long nativeTreasureHuntRenderer);
    private native void nativeSetPath(String strPath);
    private native long nativeCreateRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext);
    private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);
    private native void nativeInitializeGl(long nativeTreasureHuntRenderer);
    private native void nativeSurfaceChange(long nativeTreasureHuntRenderer, int width, int height);
    private native long nativeDrawFrame(long nativeTreasureHuntRenderer);
    private native void nativeOnTriggerEvent(long nativeTreasureHuntRenderer);
    private native void nativeOnPause(long nativeTreasureHuntRenderer);
    private native void nativeOnResume(long nativeTreasureHuntRenderer);

}
