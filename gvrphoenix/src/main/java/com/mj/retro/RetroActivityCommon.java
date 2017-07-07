package com.mj.retro;

import java.util.List;
import java.util.ArrayList;

import android.app.Activity;
import android.content.SharedPreferences;
import android.content.res.Configuration;
import android.app.UiModeManager;
import android.location.Location;
import android.os.Bundle;
import android.util.Log;
import android.content.pm.PackageManager;
import android.Manifest;
import android.content.DialogInterface;
import android.app.AlertDialog;

/**
 * Class which provides common methods for RetroActivity related classes.
 */
public class RetroActivityCommon extends Activity
{
	static {
		System.loadLibrary("gvrretro");
	}
	final private int REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS = 124;

    private static int CONNECTION_FAILURE_RESOLUTION_REQUEST = 0;
    private Location mCurrentLocation;

    // Define an object that holds accuracy and frequency parameters
    boolean mUpdatesRequested = false;
    boolean locationChanged = false;
    boolean location_service_running = false;

//	private native void nativeOnCreate();

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		checkRuntimePermissionsRunnable();
//		nativeOnCreate();
	}

//    public double onLocationGetLatitude()
//    {
//        return mCurrentLocation.getLatitude();
//    }
    public double onLocationGetLongitude()
    {
        return mCurrentLocation.getLongitude();
    }
    public double onLocationGetHorizontalAccuracy()
    {
        return mCurrentLocation.getAccuracy();
    }

    /**
     * Tells us whether the location listener callback has
     * updated the current location since the last time
     * we polled.
     *
     * @return true if location has changed, false if location has not changed.
     */
    public boolean onLocationHasChanged()
    {
        boolean hasChanged = locationChanged;

        // Reset flag
        if (hasChanged)
            locationChanged = false;

        return hasChanged;
    }

    @Override
    public void onPause()
    {
        // Save the current setting for updates
        SharedPreferences prefs = UserPreferences.getPreferences(this);
        SharedPreferences.Editor edit = prefs.edit();
        edit.putBoolean("LOCATION_UPDATES_ON", mUpdatesRequested);
        edit.apply();

        super.onPause();
    }

    @Override
    public void onResume()
    {
        SharedPreferences prefs = UserPreferences.getPreferences(this);
        SharedPreferences.Editor edit = prefs.edit();

		/*
		 * Get any previous setting for location updates
		 * Gets "false" if an error occurs
		 */
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
    }

    @Override
    public void onStop()
    {
        super.onStop();
    }


	// Exiting cleanly from NDK seems to be nearly impossible.
	// Have to use exit(0) to avoid weird things happening, even with runOnUiThread() approaches.
	// Use a separate JNI function to explicitly trigger the readback.
//	public void onRetroArchExit()
//	{
//      finish();
//	}


	public void showMessageOKCancel(String message, DialogInterface.OnClickListener onClickListener)
	{
		new AlertDialog.Builder(this).setMessage(message)
			.setPositiveButton("OK", onClickListener).setCancelable(false)
			.setNegativeButton("Cancel", null).create().show();
	}

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

	public void checkRuntimePermissions()
	{
		runOnUiThread(new Runnable() {
			public void run() {
				checkRuntimePermissionsRunnable();
			}
		});
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


}
