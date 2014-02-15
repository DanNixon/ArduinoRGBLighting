package com.dannixon.rgblighting;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import android.os.AsyncTask;
import android.util.Log;

public class DeviceConnector extends AsyncTask<Boolean, Void, String>{
	private UIManager uiMan;
	private String serviceURI;
	
	DeviceConnector(String serviceURI, UIManager uiMan) {
		this.uiMan = uiMan;
		this.serviceURI = serviceURI;
	}
	
	@Override
	protected String doInBackground(Boolean... params) {
		Integer redLevel = uiMan.getRed();
		Integer greenLevel = uiMan.getGreen();
		Integer blueLevel = uiMan.getBlue();
		Integer trans = uiMan.getTransition();
		Integer time = uiMan.getTime();
		if(!params[0]) trans = -1;
		Log.d("LightRequest", String.format("R:%d, G:%d, B:%d, Trans:%d, Time:%d", redLevel, greenLevel, blueLevel, trans, time));
		try {
			HttpClient httpclient = new DefaultHttpClient();
			HttpPost httppost = new HttpPost(serviceURI);
		    List<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>(2);
		    nameValuePairs.add(new BasicNameValuePair("r", redLevel.toString()));
		    nameValuePairs.add(new BasicNameValuePair("g", greenLevel.toString()));
		    nameValuePairs.add(new BasicNameValuePair("b", blueLevel.toString()));
		    nameValuePairs.add(new BasicNameValuePair("trans", trans.toString()));
		    nameValuePairs.add(new BasicNameValuePair("time", time.toString()));
		    httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
		    HttpResponse response = httpclient.execute(httppost);
		    HttpEntity entity = response.getEntity();
		    if (entity != null) {
		        InputStream instream = entity.getContent();
		        StringBuilder sb = new StringBuilder();
				String line;
				BufferedReader reader = new BufferedReader(new InputStreamReader(instream, "UTF-8"));
				while ((line = reader.readLine()) != null) {
					sb.append(line).append("\n");
				}
				String result = sb.toString();
		        return result;
		    }
		} catch (ClientProtocolException e) {
		    Log.e(e.getClass().toString(), e.getMessage());
		} catch (IOException e) {
			Log.e(e.getClass().toString(), e.getMessage());
		} catch (IllegalArgumentException e) {
			Log.e(e.getClass().toString(), e.getMessage());
		} catch (Exception e) {
			Log.e(e.getClass().toString(), e.getMessage());
		}
		return "null";
	}

	@Override
    protected void onPostExecute(String result) {
        super.onPostExecute(result);
        Log.d("XMLResult", result);
    	try {
    		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
			DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
			InputStream xmlData = new ByteArrayInputStream(result.getBytes());
			Document xmlDoc = dBuilder.parse(xmlData);;
			int redLevel = Integer.parseInt(xmlDoc.getElementsByTagName("r").item(0).getTextContent());
			int greenLevel = Integer.parseInt(xmlDoc.getElementsByTagName("g").item(0).getTextContent());
			int blueLevel = Integer.parseInt(xmlDoc.getElementsByTagName("b").item(0).getTextContent());
			int transition = Integer.parseInt(xmlDoc.getElementsByTagName("lastTransition").item(0).getTextContent());
			Integer time = Integer.parseInt(xmlDoc.getElementsByTagName("lastTime").item(0).getTextContent());
			uiMan.setRGB(redLevel, greenLevel, blueLevel);
			uiMan.setTime(time);
			uiMan.setTransition(transition);
		} catch (ParserConfigurationException e) {
			Log.e(e.getClass().toString(), e.getMessage());
		} catch (SAXException e) {
			Log.e(e.getClass().toString(), e.getMessage());
		} catch (IOException e) {
			Log.e(e.getClass().toString(), e.getMessage());
		}
    	
    }
	
}
