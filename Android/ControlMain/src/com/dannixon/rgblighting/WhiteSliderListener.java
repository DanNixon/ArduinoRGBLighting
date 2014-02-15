package com.dannixon.rgblighting;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.widget.SeekBar;

public class WhiteSliderListener implements SeekBar.OnSeekBarChangeListener {
	private UIManager uiMan;
	private Context main;
	
	WhiteSliderListener(Context main, UIManager uiMan) {
		this.uiMan = uiMan;
		this.main = main;
	}
	
	@Override
	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
		if(fromUser) {
			uiMan.setRGB(progress, progress, progress);
		}
	}

	@Override
	public void onStartTrackingTouch(SeekBar seekBar) {
	}

	@Override
	public void onStopTrackingTouch(SeekBar seekBar) {
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
		final String serviceURI = prefs.getString("service_uri", "");
		new DeviceConnector(serviceURI, uiMan).execute(true);
	}
}
