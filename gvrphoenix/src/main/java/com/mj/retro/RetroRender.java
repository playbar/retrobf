package com.mj.retro;

import android.content.Context;
import android.view.KeyEvent;
import android.view.MotionEvent;

/**
 * Created by houguoli on 2017/6/29.
 */

public class RetroRender {

    public RetroRender(Context ctx){
//        nativeOnCreate(ctx);
    }

    public void setPath(String strPath)
    {
        nativeSetPath(strPath);
    }

    public void retroInit()
    {
        String corepath = "/data/data/com.retroarch/cores/ppsspp_libretro_android.so";
        String gamepath = "/storage/emulated/0/apsp/sola.iso";
        nativeRetroInit(corepath, gamepath);
    }

    public void dispatchMotionEvent(MotionEvent ev )
    {
        int     AXIS_X = 0;
        int		AXIS_Y = 1;
        int		AXIS_Z = 11;
        int		AXIS_RZ = 14;
        int		AXIS_HAT_X = 15;
        int		AXIS_HAT_Y = 16;
        int		AXIS_LTRIGGER = 17;
        int		AXIS_RTRIGGER = 18;
        int		AXIS_GAS = 22;
        int		AXIS_BRAKE = 23;

        int motion_ptr = ev.getAction() >> 8;

        int source = ev.getSource();
        int id = ev.getDeviceId();
        //////////////
        float x = ev.getAxisValue(AXIS_X, motion_ptr);
        float y           = ev.getAxisValue( AXIS_Y, motion_ptr);
        float z           = ev.getAxisValue( AXIS_Z, motion_ptr);
        float rz          = ev.getAxisValue( AXIS_RZ, motion_ptr);
        float hatx        = ev.getAxisValue( AXIS_HAT_X, motion_ptr);
        float haty        = ev.getAxisValue( AXIS_HAT_Y, motion_ptr);
        float ltrig       = ev.getAxisValue( AXIS_LTRIGGER, motion_ptr);
        float rtrig       = ev.getAxisValue( AXIS_RTRIGGER, motion_ptr);
        float brake       = ev.getAxisValue( AXIS_BRAKE, motion_ptr);
        float gas         = ev.getAxisValue( AXIS_GAS, motion_ptr);
        nativeDispatchMotionEvent(source, id, x, y, z, rz, hatx, haty, ltrig, rtrig, brake, gas);
    }

    public void dispatchKeyEvent(KeyEvent event){
        int source = event.getSource();
        int id = event.getDeviceId();
        int keycode = event.getKeyCode();
        int action = event.getAction();
        int mate = event.getMetaState();
        nativeDispatchKeyEvent( source, id, keycode, action, mate);
    }

//    private native void nativeOnCreate(Context ctx);
    private native void nativeDispatchMotionEvent(int source, int id,
                                                  float x, float y, float z, float rz, float hatx, float haty,
                                                  float ltrig, float rtrig, float brake, float gas  );
    private native void nativeDispatchKeyEvent( int source, int id, int keycode, int action, int mate);
    private native void nativeSetPath(String strPath);
    private native void nativeRetroInit(String strCorepath, String strGamePath);
    private native void nativeRetroSurfaceChange(int width, int height );
    private native void nativeRetroDrawFirst( );
    private native void nativeRetroDrawSecond();

}
