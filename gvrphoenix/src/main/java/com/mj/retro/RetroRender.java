package com.mj.retro;

/**
 * Created by houguoli on 2017/6/29.
 */

public class RetroRender {

    public void setPath(String strPath)
    {
        nativeSetPath(strPath);
    }

    public void retroInit()
    {
        nativeRetroInit();
    }

    public void dispatchMotionEvent(int source, int id,
                                    float x, float y, float z, float rz, float hatx, float haty,
                                    float ltrig, float rtrig, float brake, float gas  )
    {
        nativeDispatchMotionEvent(source, id, x, y, z, rz, hatx, haty, ltrig, rtrig, brake, gas);
    }

    public void dispatchKeyEvent(int source, int id, int keycode, int action, int mate){
        nativeDispatchKeyEvent( source, id, keycode, action, mate);
    }

    private native void nativeDispatchMotionEvent(int source, int id,
                                                  float x, float y, float z, float rz, float hatx, float haty,
                                                  float ltrig, float rtrig, float brake, float gas  );
    private native void nativeDispatchKeyEvent( int source, int id, int keycode, int action, int mate);
    private native void nativeSetPath(String strPath);
    private native void nativeRetroInit();
    private native void nativeRetroSurfaceChange(int width, int height );
    private native void nativeRetroDrawFrame( );

}
