package com.retroarch.browser.retroactivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Vibrator;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.content.Intent;
import android.content.Context;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import android.hardware.input.InputManager;

import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.GvrLayout;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class RetroActivityFuture extends RetroActivityCamera {

	static {
		System.loadLibrary("gvr");
		System.loadLibrary("gvr_audio");
		System.loadLibrary("gvrretro");
	}

	// If set to true then Retroarch will completely exit when it loses focus
	private boolean quitfocus = false;

	// Opaque native pointer to the native TreasureHuntRenderer instance.
	private long nativeTreasureHuntRenderer;

	private GvrLayout gvrLayout;
	private GLSurfaceView surfaceView;

	// Note that pause and resume signals to the native renderer are performed on the GL thread,
	// ensuring thread-safety.
	private final Runnable pauseNativeRunnable =
			new Runnable() {
				@Override
				public void run() {
					nativeOnPause(nativeTreasureHuntRenderer);
				}
			};

	private final Runnable resumeNativeRunnable =
			new Runnable() {
				@Override
				public void run() {
					nativeOnResume(nativeTreasureHuntRenderer);
				}
			};



	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// Ensure fullscreen immersion.
		setImmersiveSticky();
		getWindow()
				.getDecorView()
				.setOnSystemUiVisibilityChangeListener(
						new View.OnSystemUiVisibilityChangeListener() {
							@Override
							public void onSystemUiVisibilityChange(int visibility) {
								if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
									setImmersiveSticky();
								}
							}
						});

		// Initialize GvrLayout and the native renderer.
		gvrLayout = new GvrLayout(this);
		nativeTreasureHuntRenderer = nativeCreateRenderer(getClass().getClassLoader(), this.getApplicationContext(), gvrLayout.getGvrApi().getNativeGvrContext());

		// Add the GLSurfaceView to the GvrLayout.
		surfaceView = new GLSurfaceView(this);
		surfaceView.setEGLContextClientVersion(2);
		surfaceView.setEGLConfigChooser(8, 8, 8, 0, 0, 0);
		surfaceView.setPreserveEGLContextOnPause(true);
		surfaceView.setRenderer(
				new GLSurfaceView.Renderer() {
					@Override
					public void onSurfaceCreated(GL10 gl, EGLConfig config) {
						nativeInitializeGl(nativeTreasureHuntRenderer);
					}

					@Override
					public void onSurfaceChanged(GL10 gl, int width, int height) {}

					@Override
					public void onDrawFrame(GL10 gl) {
						nativeDrawFrame(nativeTreasureHuntRenderer);
					}
				});
		surfaceView.setOnTouchListener(
				new View.OnTouchListener() {
					@Override
					public boolean onTouch(View v, MotionEvent event) {
						if (event.getAction() == MotionEvent.ACTION_DOWN) {
							// Give user feedback and signal a trigger event.
							((Vibrator) getSystemService(Context.VIBRATOR_SERVICE)).vibrate(50);
							surfaceView.queueEvent(
									new Runnable() {
										@Override
										public void run() {
											nativeOnTriggerEvent(nativeTreasureHuntRenderer);
										}
									});
							return true;
						}
						return false;
					}
				});
		gvrLayout.setPresentationView(surfaceView);

		// Add the GvrLayout to the View hierarchy.
		setContentView(gvrLayout);

		// Enable scan line racing.
		if (gvrLayout.setAsyncReprojectionEnabled(true)) {
			// Scanline racing decouples the app framerate from the display framerate,
			// allowing immersive interaction even at the throttled clockrates set by
			// sustained performance mode.
			AndroidCompat.setSustainedPerformanceMode(this, true);
		}

		// Enable VR Mode.
		AndroidCompat.setVrModeEnabled(this, true);
	}

	@Override
	public void onPause() {
		surfaceView.queueEvent(pauseNativeRunnable);
		surfaceView.onPause();
		gvrLayout.onPause();
		super.onPause();
	}

	@Override
	public void onResume() {
		super.onResume();

		gvrLayout.onResume();
		surfaceView.onResume();
		surfaceView.queueEvent(resumeNativeRunnable);

		if (android.os.Build.VERSION.SDK_INT >= 19) {
			// Immersive mode

			// Constants from API > 14
			final int API_SYSTEM_UI_FLAG_LAYOUT_STABLE = 0x00000100;
			final int API_SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION = 0x00000200;
			final int API_SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN = 0x00000400;
			final int API_SYSTEM_UI_FLAG_FULLSCREEN = 0x00000004;
			final int API_SYSTEM_UI_FLAG_IMMERSIVE_STICKY = 0x00001000;

			View thisView = getWindow().getDecorView();
			thisView.setSystemUiVisibility(API_SYSTEM_UI_FLAG_LAYOUT_STABLE
					| API_SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
					| API_SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
					| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
					| API_SYSTEM_UI_FLAG_FULLSCREEN
					| API_SYSTEM_UI_FLAG_IMMERSIVE_STICKY);

			// Check for Android UI specific parameters
			Intent retro = getIntent();
			String refresh = retro.getStringExtra("REFRESH");

			// If REFRESH parameter is provided then try to set refreshrate accordingly
			if(refresh != null) {
				WindowManager.LayoutParams params = getWindow().getAttributes();
				params.preferredRefreshRate = Integer.parseInt(refresh);
				getWindow().setAttributes(params);
			}

			// If QUITFOCUS parameter is provided then enable that Retroarch quits when focus is lost
			quitfocus = retro.hasExtra("QUITFOCUS");

			// If HIDEMOUSE parameters is provided then hide the mourse cursor
			// This requires NVIDIA Android extensions (available on NVIDIA Shield), if they are not
			// available then nothing will be done
			if (retro.hasExtra("HIDEMOUSE")) hideMouseCursor();
		}
	}

	public void hideMouseCursor() {

		// Check for NVIDIA extensions and minimum SDK version
		Method mInputManager_setCursorVisibility;
		try { mInputManager_setCursorVisibility =
			InputManager.class.getMethod("setCursorVisibility", boolean.class);
		}
		catch (NoSuchMethodException ex) {
			return; // Extensions were not available so do nothing
		}

		// Hide the mouse cursor
		InputManager inputManager = (InputManager) getSystemService(Context.INPUT_SERVICE);
		try { mInputManager_setCursorVisibility.invoke(inputManager, false); }
		catch (InvocationTargetException ite) { }
		catch (IllegalAccessException iae)    { }
	}

	@Override
	public void onStop() {
		super.onStop();

		// If QUITFOCUS parameter was set then completely exit Retroarch when focus is lost
		if (quitfocus) System.exit(0);
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		// Destruction order is important; shutting down the GvrLayout will detach
		// the GLSurfaceView and stop the GL thread, allowing safe shutdown of
		// native resources from the UI thread.
		gvrLayout.shutdown();
		nativeDestroyRenderer(nativeTreasureHuntRenderer);
		nativeTreasureHuntRenderer = 0;
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus) {
			setImmersiveSticky();
		}

	}

	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		// Avoid accidental volume key presses while the phone is in the VR headset.
		if (event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_UP
				|| event.getKeyCode() == KeyEvent.KEYCODE_VOLUME_DOWN) {
			return true;
		}
		return super.dispatchKeyEvent(event);
	}

	private void setImmersiveSticky() {
		getWindow()
				.getDecorView()
				.setSystemUiVisibility(
						View.SYSTEM_UI_FLAG_LAYOUT_STABLE
								| View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
								| View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
								| View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
								| View.SYSTEM_UI_FLAG_FULLSCREEN
								| View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
	}

	private native long nativeCreateRenderer(ClassLoader appClassLoader, Context context, long nativeGvrContext);
	private native void nativeDestroyRenderer(long nativeTreasureHuntRenderer);
	private native void nativeInitializeGl(long nativeTreasureHuntRenderer);
	private native long nativeDrawFrame(long nativeTreasureHuntRenderer);
	private native void nativeOnTriggerEvent(long nativeTreasureHuntRenderer);
	private native void nativeOnPause(long nativeTreasureHuntRenderer);
	private native void nativeOnResume(long nativeTreasureHuntRenderer);

}
