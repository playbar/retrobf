package com.mj.retro;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesClient.ConnectionCallbacks;
import com.google.android.gms.common.GooglePlayServicesClient.OnConnectionFailedListener;
import com.google.android.gms.location.LocationClient;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationRequest;

import android.Manifest;
import android.app.AlertDialog;
import android.app.NativeActivity;
import android.app.UiModeManager;
import android.content.DialogInterface;
import android.content.IntentSender;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.location.Location;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class RetroActivityCommon extends NativeActivity
implements ConnectionCallbacks, OnConnectionFailedListener, LocationListener
{
	static {
		System.loadLibrary("gvr");
		System.loadLibrary("gvr_audio");
		System.loadLibrary("gvrretro");
	}

	/* LOCATION VARIABLES */
	private static int CONNECTION_FAILURE_RESOLUTION_REQUEST = 0;
	private LocationClient mLocationClient = null;
	private Location mCurrentLocation;

	// Define an object that holds accuracy and frequency parameters
	LocationRequest mLocationRequest = null;
	boolean mUpdatesRequested = false;
	boolean locationChanged = false;
	boolean location_service_running = false;

	final private int REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS = 124;

	// Exiting cleanly from NDK seems to be nearly impossible.
	// Have to use exit(0) to avoid weird things happening, even with runOnUiThread() approaches.
	// Use a separate JNI function to explicitly trigger the readback.
	public void onRetroArchExit()
	{
		finish();
	}

	public void showMessageOKCancel(String message, DialogInterface.OnClickListener onClickListener)
	{
		new AlertDialog.Builder(this).setMessage(message).setPositiveButton("OK", onClickListener)
				.setCancelable(false).setNegativeButton("Cancel", null).create().show();
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
				if (permissionsNeeded.size() > 0)
				{
					// Need Rationale
					Log.i("RetroActivity", "Need to request external storage permissions.");
					String message = "You need to grant access to " + permissionsNeeded.get(0);
					for (int i = 1; i < permissionsNeeded.size(); i++)
						message = message + ", " + permissionsNeeded.get(i);

					showMessageOKCancel(message,
							new DialogInterface.OnClickListener()
							{
								@Override
								public void onClick(DialogInterface dialog, int which)
								{
									requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
											REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS);

									Log.i("RetroActivity", "User accepted request for external storage permissions.");
								}
							});
				}
				else
				{
					requestPermissions(permissionsList.toArray(new String[permissionsList.size()]),
							REQUEST_CODE_ASK_MULTIPLE_PERMISSIONS);

					Log.i("RetroActivity", "Requested external storage permissions.");
				}
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

	public boolean isAndroidTV()
	{
		Configuration config = getResources().getConfiguration();
		UiModeManager uiModeManager = (UiModeManager)getSystemService(UI_MODE_SERVICE);

		if (uiModeManager.getCurrentModeType() == Configuration.UI_MODE_TYPE_TELEVISION)
		{
			Log.i("RetroActivity", "isAndroidTV == true");
			return true;
		}
		else
		{
			Log.i("RetroActivity", "isAndroidTV == false");
			return false;
		}
	}

	/**
	 * Called by Location Services when the request to connect the
	 * client finishes successfully. At this point, you can
	 * request the current location or start periodic updates
	 */
	@Override
	public void onConnected(Bundle dataBundle)
	{
		if (mLocationClient == null)
			return;
		
		// Display the connection status
		Toast.makeText(this, "Connected", Toast.LENGTH_SHORT).show();
		location_service_running = true;

		// If already requested, start periodic updates
		if (mUpdatesRequested)
		{
			mLocationClient.requestLocationUpdates(mLocationRequest, this, null);
		}
		else
		{
			// Get last known location
			mCurrentLocation = mLocationClient.getLastLocation();
			locationChanged = true;
		}
	}

	/**
	 * Called by Location Services if the connection to the
	 * location client drops because of an error.
	 */
	@Override
	public void onDisconnected()
	{
		if (mLocationClient == null)
			return;
		
		// Display the connection status
		Toast.makeText(this, "Disconnected. Please re-connect.", Toast.LENGTH_SHORT).show();

		// If the client is connected
		if (mLocationClient.isConnected())
		{
			/*
			 * Remove location updates for a listener.
			 * The current Activity is the listener, so
			 * the argument is "this".
			 */
			mLocationClient.removeLocationUpdates(this);
		}

		location_service_running = false;
	}

	/**
	 * Called by Location Services if the attempt to
	 * Location Services fails.
	 */
	@Override
	public void onConnectionFailed(ConnectionResult connectionResult)
	{
		/*
		 * Google Play services can resolve some errors it detects.
		 * If the error has a resolution, try sending an Intent to
		 * start a Google Play services activity that can resolve
		 * error.
		 */
		if (connectionResult.hasResolution())
		{
			try
			{
				// Start an Activity that tries to resolve the error
				connectionResult.startResolutionForResult(this, CONNECTION_FAILURE_RESOLUTION_REQUEST);
			}
			catch (IntentSender.SendIntentException e)
			{
				// Thrown if Google Play services cancelled the original PendingIntent
				e.printStackTrace();
			}
		}
		else
		{
			/*
			 * If no resolution is available, display a dialog to the
			 * user with the error.
			 */
			Log.e("Connection failed", "error code: " + connectionResult.getErrorCode());
		}
	}

	/**
	 * Sets the update interval at which location-based updates 
	 * should occur
	 */
	public void onLocationSetInterval(int update_interval_in_ms, int distance_interval)
	{
		// Use high accuracy
        if (mLocationRequest == null)
        	return;
        
        mLocationRequest.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);

		if (update_interval_in_ms == 0)
			mLocationRequest.setInterval(5 * 1000); // 5 seconds
		else
			mLocationRequest.setInterval(update_interval_in_ms);

		// Set the fastest update interval to 1 second
		mLocationRequest.setFastestInterval(1000);
	}

	/**
	 * Initializing methods for location based functionality.
	 */
	public void onLocationInit()
	{
		/*
		 * Create a new location client, using the enclosing class to
		 * handle callbacks.
		 */
		if (mLocationClient == null)
			mLocationClient = new LocationClient(this, this, this);

		// Start with updates turned off
		mUpdatesRequested = false;

		// Create the LocationRequest object
		if (mLocationRequest == null)
			mLocationRequest = LocationRequest.create();

		onLocationSetInterval(0, 0);
	}


	/**
	 * Executed upon starting the {@link LocationClient}.
	 */
	public void onLocationStart()
	{
		if (mLocationClient == null)
			return;
		
		mUpdatesRequested = true;

		// Connect the client.
        mLocationClient.connect();
	}

	/**
	 * Free up location services resources.
	 */
	public void onLocationFree()
	{
		/* TODO/FIXME */
	}

	/**
	 * Executed upon stopping the location client.
	 * Does nothing if called when the client is not started.
	 */
	public void onLocationStop()
	{
		// Disconnecting the client invalidates it.
		if (mLocationClient != null && mUpdatesRequested)
			mLocationClient.disconnect();
	}

	/**
	 * Gets the latitude at the current location in degrees.
	 * 
	 * @return the latitude at the current location.
	 */
	public double onLocationGetLatitude()
	{
		return mCurrentLocation.getLatitude();
	}

	/**
	 * Gets the longitude at the current location in degrees.
	 * 
	 * @return the longitude at the current location.
	 */
	public double onLocationGetLongitude()
	{
		return mCurrentLocation.getLongitude();
	}
	
	/**
	 * Gets the horizontal accuracy of the current location 
	 * in meters. (NOTE: There seems to be no vertical accuracy
	 * for a given location with the Android location API)
	 * 
	 * @return the horizontal accuracy of the current position.
	 */
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

	// Define the callback method that receives location updates
	@Override
	public void onLocationChanged(Location location)
	{
		if (!location_service_running)
			return;
		
		locationChanged = true;
		mCurrentLocation = location;

		// Report to the UI that the location was updated
		String msg = "Updated Location: " + location.getLatitude() + ", " + location.getLongitude();
		Log.i("RetroArch GPS", msg);
		//Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
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
		onLocationStop();
		super.onStop();
	}
}
