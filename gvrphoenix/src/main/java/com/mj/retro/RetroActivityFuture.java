package com.mj.retro;

import android.Manifest;
import android.app.Activity;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.Bundle;
import android.os.Environment;
import android.os.Vibrator;
import android.preference.PreferenceManager;
import android.provider.Settings;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.content.Intent;
import android.content.Context;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.ArrayList;
import java.util.List;

import android.hardware.input.InputManager;

import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.GvrLayout;
import com.google.vr.sdk.base.Eye;
import com.google.vr.sdk.base.GvrActivity;
import com.google.vr.sdk.base.GvrView;
import com.google.vr.sdk.base.HeadTransform;
import com.google.vr.sdk.base.Viewport;
import com.retroarch.R;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class RetroActivityFuture extends GvrActivity implements GvrView.StereoRenderer {

	static {
//		System.loadLibrary("gvr");
//		System.loadLibrary("gvr_audio");
		System.loadLibrary("gvrretro");
	}

	protected float[] modelCube;
	protected float[] modelPosition;

	private static final String TAG = "TreasureHuntActivity";

	private static final float Z_NEAR = 0.1f;
	private static final float Z_FAR = 100.0f;

	private static final float CAMERA_Z = 0.01f;
	private static final float TIME_DELTA = 0.3f;

	private static final float YAW_LIMIT = 0.12f;
	private static final float PITCH_LIMIT = 0.12f;

	private static final int COORDS_PER_VERTEX = 3;

	// We keep the light always position just above the user.
	private static final float[] LIGHT_POS_IN_WORLD_SPACE = new float[] {0.0f, 2.0f, 0.0f, 1.0f};

	// Convenience vector for extracting the position from a matrix via multiplication.
	private static final float[] POS_MATRIX_MULTIPLY_VEC = {0, 0, 0, 1.0f};

	private static final float MIN_MODEL_DISTANCE = 3.0f;
	private static final float MAX_MODEL_DISTANCE = 7.0f;

	private static final String OBJECT_SOUND_FILE = "cube_sound.wav";
	private static final String SUCCESS_SOUND_FILE = "success.wav";

	private final float[] lightPosInEyeSpace = new float[4];

	private FloatBuffer floorVertices;
	private FloatBuffer floorColors;
	private FloatBuffer floorNormals;

	private FloatBuffer cubeVertices;
	private FloatBuffer cubeColors;
	private FloatBuffer cubeFoundColors;
	private FloatBuffer cubeNormals;

	private int cubeProgram;
	private int floorProgram;

	private int cubePositionParam;
	private int cubeNormalParam;
	private int cubeColorParam;
	private int cubeModelParam;
	private int cubeModelViewParam;
	private int cubeModelViewProjectionParam;
	private int cubeLightPosParam;

	private int floorPositionParam;
	private int floorNormalParam;
	private int floorColorParam;
	private int floorModelParam;
	private int floorModelViewParam;
	private int floorModelViewProjectionParam;
	private int floorLightPosParam;

	private float[] camera;
	private float[] view;
	private float[] headView;
	private float[] modelViewProjection;
	private float[] modelView;
	private float[] modelFloor;

	private float[] tempPosition;
	private float[] headRotation;

	private float objectDistance = MAX_MODEL_DISTANCE / 2.0f;
	private float floorDepth = 20f;

	private Vibrator vibrator;

	final private int REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS = 124;

	// If set to true then Retroarch will completely exit when it loses focus
	private boolean quitfocus = false;
	boolean mUpdatesRequested = false;
	boolean locationChanged = false;
	boolean location_service_running = false;

	// Opaque native pointer to the native TreasureHuntRenderer instance.
	private long nativeTreasureHuntRenderer;

	private GvrLayout gvrLayout;
	private GLSurfaceView surfaceView;
	private BfRenderer render;
	private RetroRender retroRender;

	// Note that pause and resume signals to the native renderer are performed on the GL thread,
	// ensuring thread-safety.
	private final Runnable pauseNativeRunnable =
			new Runnable() {
				@Override
				public void run() {
					render.onPause();
				}
			};

	private final Runnable resumeNativeRunnable =
			new Runnable() {
				@Override
				public void run() {
					render.onResume();
				}
			};

	private boolean addPermission(List<String> permissionsList, String permission)
	{
		if (checkSelfPermission(permission) != PackageManager.PERMISSION_GRANTED)
		{
			permissionsList.add(permission);

			// Check for Rationale Option
			if (!shouldShowRequestPermissionRationale(permission))
				return false;
		}

		return true;
	}

	public void checkRuntimePermissionsRunnable()
	{
		if (android.os.Build.VERSION.SDK_INT >= 23)
		{
			// Android 6.0+ needs runtime permission checks
			List<String> permissionsNeeded = new ArrayList<String>();
			final List<String> permissionsList = new ArrayList<String>();

			if (!addPermission(permissionsList, Manifest.permission.READ_EXTERNAL_STORAGE))
				permissionsNeeded.add("Read External Storage");
			if (!addPermission(permissionsList, Manifest.permission.WRITE_EXTERNAL_STORAGE))
				permissionsNeeded.add("Write External Storage");

			if (permissionsList.size() > 0)
			{
				requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
						REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS);
			}
		}
	}

	@Override
	public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
	{
		switch (requestCode)
		{
			case REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS:
				for (int i = 0; i < permissions.length; i++)
				{
					if(grantResults[i] == PackageManager.PERMISSION_GRANTED)
					{
						Log.i("RetroActivity", "Permission: " + permissions[i] + " was granted.");
					}
					else
					{
						Log.i("RetroActivity", "Permission: " + permissions[i] + " was not granted.");
					}
				}
				break;
			default:
				super.onRequestPermissionsResult(requestCode, permissions, grantResults);
				break;
		}
	}

	public void initNativeView()
	{
		retroRender = new RetroRender();
		// Ensure fullscreen immersion.
		setImmersiveSticky();
		getWindow().getDecorView().setOnSystemUiVisibilityChangeListener(
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
		render = new BfRenderer(getClass().getClassLoader(), getApplicationContext(), gvrLayout.getGvrApi().getNativeGvrContext());
//		render.onCreate();
//		retroRender.setPath("lib2048.so");
		render.setRetroRender(retroRender);

		// Add the GLSurfaceView to the GvrLayout.
		surfaceView = new GLSurfaceView(this);
		surfaceView.setEGLContextClientVersion(2);
		surfaceView.setEGLConfigChooser(8, 8, 8, 0, 0, 0);
		surfaceView.setPreserveEGLContextOnPause(true);
		surfaceView.setRenderer(render);
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
											render.onTriggerEvent();
										}
									});
							return true;
						}
						return false;
					}
				});
		surfaceView.setOnKeyListener(new View.OnKeyListener(){
			public	boolean onKey(View v, int keyCode, KeyEvent event)
			{
				return true;
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

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		checkRuntimePermissionsRunnable();
		UserPreferences.updateConfigFile(this);
		final SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(this);
		nativeOnCreate(null,
				prefs.getString("libretro_path", getApplicationInfo().dataDir + "/cores/"),
				UserPreferences.getDefaultConfigPath(this),
				Settings.Secure.getString(getContentResolver(), Settings.Secure.DEFAULT_INPUT_METHOD),
				getApplicationInfo().dataDir,
				getApplicationInfo().sourceDir,
				Environment.getExternalStorageDirectory().getAbsolutePath(),
				Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).getAbsolutePath(),
				Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getAbsolutePath(),
				Environment.getExternalStorageDirectory().getAbsolutePath() + "/Android/data/com.retroarch/files");

		initializeGvrView();

		modelCube = new float[16];
		camera = new float[16];
		view = new float[16];
		modelViewProjection = new float[16];
		modelView = new float[16];
		modelFloor = new float[16];
		tempPosition = new float[4];
		// Model first appears directly in front of user.
		modelPosition = new float[] {0.0f, 0.0f, -MAX_MODEL_DISTANCE / 2.0f};
		headRotation = new float[4];
		headView = new float[16];
		vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

	}

	public void initializeGvrView() {
		setContentView(R.layout.common_ui);
		retroRender = new RetroRender();

		GvrView gvrView = (GvrView) findViewById(R.id.gvr_view);
		gvrView.setEGLConfigChooser(8, 8, 8, 8, 16, 8);

		gvrView.setRenderer(this);
		gvrView.setTransitionViewEnabled(false);

		// Enable Cardboard-trigger feedback with Daydream headsets. This is a simple way of supporting
		// Daydream controller input for basic interactions using the existing Cardboard trigger API.
		gvrView.enableCardboardTriggerEmulation();

		if (gvrView.setAsyncReprojectionEnabled(true)) {
			// Async reprojection decouples the app framerate from the display framerate,
			// allowing immersive interaction even at the throttled clockrates set by
			// sustained performance mode.
			com.google.vr.sdk.base.AndroidCompat.setSustainedPerformanceMode(this, true);
		}

		setGvrView(gvrView);
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		boolean value = super.onTouchEvent(event);
		System.out.println("super.onTouchEvent: " + value+ " event: " + event.getAction());
		return value;
	}

	@Override
	public boolean dispatchTouchEvent(MotionEvent ev)
	{
		boolean value = super.dispatchTouchEvent( ev );
		return value;
	}
	@Override
	public boolean dispatchTrackballEvent(MotionEvent ev){
		boolean value = super.dispatchTrackballEvent(ev);
		return value;
	}
	@Override
	public boolean dispatchGenericMotionEvent(MotionEvent ev){
		super.dispatchGenericMotionEvent(ev);
		retroRender.dispatchMotionEvent( ev);
		return true;
	}

	@Override
	public boolean onTrackballEvent(MotionEvent event)
	{
		boolean value = super.onTrackballEvent(event);
		return value;
	}

	@Override
	public boolean dispatchKeyEvent(android.view.KeyEvent event) {
		retroRender.dispatchKeyEvent( event);
		return true;
	}

	@Override
	public void onPause() {
		// Save the current setting for updates
		SharedPreferences prefs = UserPreferences.getPreferences(this);
		SharedPreferences.Editor edit = prefs.edit();
		edit.putBoolean("LOCATION_UPDATES_ON", mUpdatesRequested);
		edit.apply();

//		surfaceView.queueEvent(pauseNativeRunnable);
//		surfaceView.onPause();
//		gvrLayout.onPause();
		super.onPause();
	}

	@Override
	public void onResume() {
		SharedPreferences prefs = UserPreferences.getPreferences(this);
		SharedPreferences.Editor edit = prefs.edit();
		if (prefs.contains("LOCATION_UPDATES_ON"))
		{
			mUpdatesRequested = prefs.getBoolean("LOCATION_UPDATES_ON", false);
			if (mUpdatesRequested)
				location_service_running = true;
		}
		else // Otherwise, turn off location updates
		{
			edit.putBoolean("LOCATION_UPDATES_ON", false);
			edit.apply();
			location_service_running = false;
		}

		super.onResume();

//		gvrLayout.onResume();
//		surfaceView.onResume();
//		surfaceView.queueEvent(resumeNativeRunnable);

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
//		gvrLayout.shutdown();
		render.destroyRenderer();
	}

	@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		super.onWindowFocusChanged(hasFocus);
		if (hasFocus) {
			setImmersiveSticky();
		}

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

	private native void nativeOnCreate(String strROM, String strLIBRETRO, String strCONFIGFILE, String strIME, String strDATADIR,
									   String strAPK, String strSDCARD, String strDOWNLOADS, String strSCREENSHOTS, String strEXTERNAL);

	protected void setCubeRotation() {
		Matrix.rotateM(modelCube, 0, TIME_DELTA, 0.5f, 0.5f, 1.0f);
	}

	private String readRawTextFile(int resId) {
		InputStream inputStream = getResources().openRawResource(resId);
		try {
			BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
			StringBuilder sb = new StringBuilder();
			String line;
			while ((line = reader.readLine()) != null) {
				sb.append(line).append("\n");
			}
			reader.close();
			return sb.toString();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return null;
	}

	protected void updateModelPosition() {
		Matrix.setIdentityM(modelCube, 0);
		Matrix.translateM(modelCube, 0, modelPosition[0], modelPosition[1], modelPosition[2]);
		checkGLError("updateCubePosition");
	}

	private int loadGLShader(int type, int resId) {
		String code = readRawTextFile(resId);
		int shader = GLES20.glCreateShader(type);
		GLES20.glShaderSource(shader, code);
		GLES20.glCompileShader(shader);

		// Get the compilation status.
		final int[] compileStatus = new int[1];
		GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileStatus, 0);

		// If the compilation failed, delete the shader.
		if (compileStatus[0] == 0) {
			Log.e(TAG, "Error compiling shader: " + GLES20.glGetShaderInfoLog(shader));
			GLES20.glDeleteShader(shader);
			shader = 0;
		}

		if (shader == 0) {
			throw new RuntimeException("Error creating shader.");
		}

		return shader;
	}

	private static void checkGLError(String label) {
		int error;
		while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
			Log.e(TAG, label + ": glError " + error);
			throw new RuntimeException(label + ": glError " + error);
		}
	}

	private boolean isLookingAtObject() {
		// Convert object space to camera space. Use the headView from onNewFrame.
		Matrix.multiplyMM(modelView, 0, headView, 0, modelCube, 0);
		Matrix.multiplyMV(tempPosition, 0, modelView, 0, POS_MATRIX_MULTIPLY_VEC, 0);

		float pitch = (float) Math.atan2(tempPosition[1], -tempPosition[2]);
		float yaw = (float) Math.atan2(tempPosition[0], -tempPosition[2]);

		return Math.abs(pitch) < PITCH_LIMIT && Math.abs(yaw) < YAW_LIMIT;
	}

	public void drawCube() {
		GLES20.glUseProgram(cubeProgram);

		GLES20.glUniform3fv(cubeLightPosParam, 1, lightPosInEyeSpace, 0);

		// Set the Model in the shader, used to calculate lighting
		GLES20.glUniformMatrix4fv(cubeModelParam, 1, false, modelCube, 0);

		// Set the ModelView in the shader, used to calculate lighting
		GLES20.glUniformMatrix4fv(cubeModelViewParam, 1, false, modelView, 0);

		// Set the position of the cube
		GLES20.glVertexAttribPointer(
				cubePositionParam, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, 0, cubeVertices);

		// Set the ModelViewProjection matrix in the shader.
		GLES20.glUniformMatrix4fv(cubeModelViewProjectionParam, 1, false, modelViewProjection, 0);

		// Set the normal positions of the cube, again for shading
		GLES20.glVertexAttribPointer(cubeNormalParam, 3, GLES20.GL_FLOAT, false, 0, cubeNormals);
		GLES20.glVertexAttribPointer(cubeColorParam, 4, GLES20.GL_FLOAT, false, 0,
				isLookingAtObject() ? cubeFoundColors : cubeColors);

		// Enable vertex arrays
		GLES20.glEnableVertexAttribArray(cubePositionParam);
		GLES20.glEnableVertexAttribArray(cubeNormalParam);
		GLES20.glEnableVertexAttribArray(cubeColorParam);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 36);

		// Disable vertex arrays
		GLES20.glDisableVertexAttribArray(cubePositionParam);
		GLES20.glDisableVertexAttribArray(cubeNormalParam);
		GLES20.glDisableVertexAttribArray(cubeColorParam);

		checkGLError("Drawing cube");
	}

	public void drawFloor() {
		GLES20.glUseProgram(floorProgram);

		// Set ModelView, MVP, position, normals, and color.
		GLES20.glUniform3fv(floorLightPosParam, 1, lightPosInEyeSpace, 0);
		GLES20.glUniformMatrix4fv(floorModelParam, 1, false, modelFloor, 0);
		GLES20.glUniformMatrix4fv(floorModelViewParam, 1, false, modelView, 0);
		GLES20.glUniformMatrix4fv(floorModelViewProjectionParam, 1, false, modelViewProjection, 0);
		GLES20.glVertexAttribPointer(
				floorPositionParam, COORDS_PER_VERTEX, GLES20.GL_FLOAT, false, 0, floorVertices);
		GLES20.glVertexAttribPointer(floorNormalParam, 3, GLES20.GL_FLOAT, false, 0, floorNormals);
		GLES20.glVertexAttribPointer(floorColorParam, 4, GLES20.GL_FLOAT, false, 0, floorColors);

		GLES20.glEnableVertexAttribArray(floorPositionParam);
		GLES20.glEnableVertexAttribArray(floorNormalParam);
		GLES20.glEnableVertexAttribArray(floorColorParam);

		GLES20.glDrawArrays(GLES20.GL_TRIANGLES, 0, 24);

		GLES20.glDisableVertexAttribArray(floorPositionParam);
		GLES20.glDisableVertexAttribArray(floorNormalParam);
		GLES20.glDisableVertexAttribArray(floorColorParam);

		checkGLError("drawing floor");
	}

	@Override
	public void onNewFrame(HeadTransform headTransform) {
		setCubeRotation();

		// Build the camera matrix and apply it to the ModelView.
		Matrix.setLookAtM(camera, 0, 0.0f, 0.0f, CAMERA_Z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		headTransform.getHeadView(headView, 0);

		// Update the 3d audio engine with the most recent head rotation.
		headTransform.getQuaternion(headRotation, 0);

		checkGLError("onReadyToDraw");
	}

	@Override
	public void onDrawEye(Eye eye) {
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);

//		checkGLError("colorParam");
//
//		// Apply the eye transformation to the camera.
//		Matrix.multiplyMM(view, 0, eye.getEyeView(), 0, camera, 0);
//
//		// Set the position of the light
//		Matrix.multiplyMV(lightPosInEyeSpace, 0, view, 0, LIGHT_POS_IN_WORLD_SPACE, 0);
//
//		// Build the ModelView and ModelViewProjection matrices
//		// for calculating cube position and light.
//		float[] perspective = eye.getPerspective(Z_NEAR, Z_FAR);
//		Matrix.multiplyMM(modelView, 0, view, 0, modelCube, 0);
//		Matrix.multiplyMM(modelViewProjection, 0, perspective, 0, modelView, 0);
//		drawCube();
//
//		// Set modelView for the floor, so we draw floor in the correct location
//		Matrix.multiplyMM(modelView, 0, view, 0, modelFloor, 0);
//		Matrix.multiplyMM(modelViewProjection, 0, perspective, 0, modelView, 0);
//		drawFloor();
		if( eye.getType() == 1)
			retroRender.retroDrawFirst();
		else
			retroRender.retroDrawSecond();
	}

	@Override
	public void onFinishFrame(Viewport viewport) {
		Log.i(TAG, "onFinishFrame");
	}

	@Override
	public void onSurfaceChanged(int i, int i1) {
		Log.i(TAG, "onSurfaceChanged");
	}

	@Override
	public void onSurfaceCreated(EGLConfig eglConfig) {
		Log.i(TAG, "onSurfaceCreated");
		GLES20.glClearColor(0.1f, 0.1f, 0.1f, 0.5f); // Dark background so text shows up well.

		ByteBuffer bbVertices = ByteBuffer.allocateDirect(WorldLayoutData.CUBE_COORDS.length * 4);
		bbVertices.order(ByteOrder.nativeOrder());
		cubeVertices = bbVertices.asFloatBuffer();
		cubeVertices.put(WorldLayoutData.CUBE_COORDS);
		cubeVertices.position(0);

		ByteBuffer bbColors = ByteBuffer.allocateDirect(WorldLayoutData.CUBE_COLORS.length * 4);
		bbColors.order(ByteOrder.nativeOrder());
		cubeColors = bbColors.asFloatBuffer();
		cubeColors.put(WorldLayoutData.CUBE_COLORS);
		cubeColors.position(0);

		ByteBuffer bbFoundColors = ByteBuffer.allocateDirect(WorldLayoutData.CUBE_FOUND_COLORS.length * 4);
		bbFoundColors.order(ByteOrder.nativeOrder());
		cubeFoundColors = bbFoundColors.asFloatBuffer();
		cubeFoundColors.put(WorldLayoutData.CUBE_FOUND_COLORS);
		cubeFoundColors.position(0);

		ByteBuffer bbNormals = ByteBuffer.allocateDirect(WorldLayoutData.CUBE_NORMALS.length * 4);
		bbNormals.order(ByteOrder.nativeOrder());
		cubeNormals = bbNormals.asFloatBuffer();
		cubeNormals.put(WorldLayoutData.CUBE_NORMALS);
		cubeNormals.position(0);

		// make a floor
		ByteBuffer bbFloorVertices = ByteBuffer.allocateDirect(WorldLayoutData.FLOOR_COORDS.length * 4);
		bbFloorVertices.order(ByteOrder.nativeOrder());
		floorVertices = bbFloorVertices.asFloatBuffer();
		floorVertices.put(WorldLayoutData.FLOOR_COORDS);
		floorVertices.position(0);

		ByteBuffer bbFloorNormals = ByteBuffer.allocateDirect(WorldLayoutData.FLOOR_NORMALS.length * 4);
		bbFloorNormals.order(ByteOrder.nativeOrder());
		floorNormals = bbFloorNormals.asFloatBuffer();
		floorNormals.put(WorldLayoutData.FLOOR_NORMALS);
		floorNormals.position(0);

		ByteBuffer bbFloorColors = ByteBuffer.allocateDirect(WorldLayoutData.FLOOR_COLORS.length * 4);
		bbFloorColors.order(ByteOrder.nativeOrder());
		floorColors = bbFloorColors.asFloatBuffer();
		floorColors.put(WorldLayoutData.FLOOR_COLORS);
		floorColors.position(0);

		int vertexShader = loadGLShader(GLES20.GL_VERTEX_SHADER, R.raw.light_vertex);
		int gridShader = loadGLShader(GLES20.GL_FRAGMENT_SHADER, R.raw.grid_fragment);
		int passthroughShader = loadGLShader(GLES20.GL_FRAGMENT_SHADER, R.raw.passthrough_fragment);

		cubeProgram = GLES20.glCreateProgram();
		GLES20.glAttachShader(cubeProgram, vertexShader);
		GLES20.glAttachShader(cubeProgram, passthroughShader);
		GLES20.glLinkProgram(cubeProgram);
		GLES20.glUseProgram(cubeProgram);

		checkGLError("Cube program");

		cubePositionParam = GLES20.glGetAttribLocation(cubeProgram, "a_Position");
		cubeNormalParam = GLES20.glGetAttribLocation(cubeProgram, "a_Normal");
		cubeColorParam = GLES20.glGetAttribLocation(cubeProgram, "a_Color");

		cubeModelParam = GLES20.glGetUniformLocation(cubeProgram, "u_Model");
		cubeModelViewParam = GLES20.glGetUniformLocation(cubeProgram, "u_MVMatrix");
		cubeModelViewProjectionParam = GLES20.glGetUniformLocation(cubeProgram, "u_MVP");
		cubeLightPosParam = GLES20.glGetUniformLocation(cubeProgram, "u_LightPos");

		checkGLError("Cube program params");

		floorProgram = GLES20.glCreateProgram();
		GLES20.glAttachShader(floorProgram, vertexShader);
		GLES20.glAttachShader(floorProgram, gridShader);
		GLES20.glLinkProgram(floorProgram);
		GLES20.glUseProgram(floorProgram);

		checkGLError("Floor program");

		floorModelParam = GLES20.glGetUniformLocation(floorProgram, "u_Model");
		floorModelViewParam = GLES20.glGetUniformLocation(floorProgram, "u_MVMatrix");
		floorModelViewProjectionParam = GLES20.glGetUniformLocation(floorProgram, "u_MVP");
		floorLightPosParam = GLES20.glGetUniformLocation(floorProgram, "u_LightPos");

		floorPositionParam = GLES20.glGetAttribLocation(floorProgram, "a_Position");
		floorNormalParam = GLES20.glGetAttribLocation(floorProgram, "a_Normal");
		floorColorParam = GLES20.glGetAttribLocation(floorProgram, "a_Color");

		checkGLError("Floor program params");

		Matrix.setIdentityM(modelFloor, 0);
		Matrix.translateM(modelFloor, 0, 0, -floorDepth, 0); // Floor appears below user.

		updateModelPosition();

		String corepath = "/data/data/com.retroarch/lib/libsnes9x.so";
		String gamepath = "/storage/emulated/0/apsp/sfc/3000528.zip";
		retroRender.retroInit(corepath, gamepath);

		checkGLError("onSurfaceCreated");
	}

	@Override
	public void onRendererShutdown() {
		Log.i(TAG, "onRendererShutdown");
	}
}
