package com.dannixon.rgblighting;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.SeekBar;

public class ColourSliderListener implements SeekBar.OnSeekBarChangeListener {
	private UIManager uiMan;
	private Context main;
	
	ColourSliderListener(Context main, UIManager uiMan) {
		this.uiMan = uiMan;
		this.main = main;
	}
	
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		int red = uiMan.getRed();
		int green = uiMan.getGreen();
		int blue = uiMan.getBlue();
		int average = (red + green + blue) / 3;
		Log.d("ColourSliderChange", String.format("AverageIntensity:%d", average));
		uiMan.setWhite(average);
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
		String serviceURI = prefs.getString("service_uri", "");
		new DeviceConnector(serviceURI, uiMan).execute(true);
	}
}
