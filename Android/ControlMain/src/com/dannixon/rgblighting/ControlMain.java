package com.dannixon.rgblighting;

import android.os.Bundle;
import android.preference.PreferenceManager;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

public class ControlMain extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_control_main);
		final SeekBar redSeekBar = (SeekBar) findViewById(R.id.redSlider);
		final SeekBar greenSeekBar = (SeekBar) findViewById(R.id.greenSlider);
		final SeekBar blueSeekBar = (SeekBar) findViewById(R.id.blueSlider);
		final SeekBar whiteSeekBar = (SeekBar) findViewById(R.id.whiteSlider);
		final Button whiteButton = (Button) findViewById(R.id.whiteButton);
		final Button onButton = (Button) findViewById(R.id.onButton);
		final Button offButton = (Button) findViewById(R.id.offButton);
		final Button refreshButton = (Button) findViewById(R.id.refreshButton);
		final TextView timeText = (TextView) findViewById(R.id.transTime);
		final Spinner transSelect = (Spinner) findViewById(R.id.transitionSelect);
		final Context main = this;
		final UIManager uiMan = new UIManager(redSeekBar, greenSeekBar, blueSeekBar, whiteSeekBar, timeText, transSelect);
		redSeekBar.setOnSeekBarChangeListener(new ColourSliderListener(main, uiMan));
		greenSeekBar.setOnSeekBarChangeListener(new ColourSliderListener(main, uiMan));
		blueSeekBar.setOnSeekBarChangeListener(new ColourSliderListener(main, uiMan));
		whiteSeekBar.setOnSeekBarChangeListener(new WhiteSliderListener(main, uiMan));
		whiteButton.setOnClickListener(new View.OnClickListener() {	
			@Override
			public void onClick(View v) {
				uiMan.setRGB(255, 255, 50);
				SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
				final String serviceURI = prefs.getString("service_uri", "");
				new DeviceConnector(serviceURI, uiMan).execute(true);
			}
		});
		onButton.setOnClickListener(new View.OnClickListener() {	
			@Override
			public void onClick(View v) {
				uiMan.setRGB(255, 255, 255);
				SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
				final String serviceURI = prefs.getString("service_uri", "");
				new DeviceConnector(serviceURI, uiMan).execute(true);
			}
		});
		offButton.setOnClickListener(new View.OnClickListener() {	
			@Override
			public void onClick(View v) {
				uiMan.setRGB(0, 0, 0);
				SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
				final String serviceURI = prefs.getString("service_uri", "");
				new DeviceConnector(serviceURI, uiMan).execute(true);
			}
		});
		refreshButton.setOnClickListener(new View.OnClickListener() {	
			@Override
			public void onClick(View v) {
				SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
				final String serviceURI = prefs.getString("service_uri", "");
				new DeviceConnector(serviceURI, uiMan).execute(false);
			}
		});
		SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(main);
		final String serviceURI = prefs.getString("service_uri", "");
		new DeviceConnector(serviceURI, uiMan).execute(false);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.control_main, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    switch (item.getItemId()) {
	        case R.id.action_settings:
	        	Intent myIntent = new Intent(ControlMain.this, SettingsActivity.class);
                ControlMain.this.startActivity(myIntent);
	            return true;
	        default:
	            return super.onOptionsItemSelected(item);
	    }
	}

}
