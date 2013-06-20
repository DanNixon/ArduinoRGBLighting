package com.dannixon.rgblighting;

import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;

public class UIManager {
	private SeekBar redSeekBar;
	private SeekBar greenSeekBar;
	private SeekBar blueSeekBar;
	private SeekBar whiteSeekBar;
	private TextView timeText;
	private Spinner transSelect;
	
	UIManager(SeekBar redSeekBar, SeekBar greenSeekBar, SeekBar blueSeekBar, SeekBar whiteSeekBar, TextView timeText, Spinner transSelect) {
		this.redSeekBar = redSeekBar;
		this.greenSeekBar = greenSeekBar;
		this.blueSeekBar = blueSeekBar;
		this.whiteSeekBar = whiteSeekBar;
		this.timeText = timeText;
		this.transSelect = transSelect;
	}
	
	public int getRed() {
		return redSeekBar.getProgress();
	}
	
	public int getGreen() {
		return greenSeekBar.getProgress();
	}
	
	public int getBlue() {
		return blueSeekBar.getProgress();
	}
	
	public int getWhite() {
		return whiteSeekBar.getProgress();
	}
	
	public int getTransition() {
		return transSelect.getSelectedItemPosition();
	}
	
	public int getTime() {
		try {
			return Integer.parseInt(timeText.getText().toString());
		} catch (NumberFormatException e) {
			return 0;
		}
	}
	
	public void setRGB(int r, int g, int b) {
		redSeekBar.setProgress(r);
		greenSeekBar.setProgress(g);
		blueSeekBar.setProgress(b);
	}
	
	public void setWhite(int w) {
		whiteSeekBar.setProgress(w);
	}
	
	public void setTransition(int trans) {
		transSelect.setSelection(trans);
	}
	
	public void setTime(Integer time) {
		timeText.setText(time.toString());
	}
}
