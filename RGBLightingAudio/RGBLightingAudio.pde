/**
  RGB Audio light show
  Dan Nixon 2013
  http://dan-nixon.com
*/

import processing.net.*;
import ddf.minim.analysis.*;
import ddf.minim.*;
import processing.serial.*;

Client httpClient;
Minim minim;
AudioPlayer player;
FFT fft;
Serial serial;

static final int FRAME_RATE = 20;
static final int LED_TRANS = 0;
static int LED_UPDATE;

static final int INDEX_AVERAGE = 20;

static final float RED_CENTRE_FREQ = 500.0;
static final float GREEN_CENTRE_FREQ = 8000.0;
static final float BLUE_CENTRE_FREQ = 15000.0;

static final float RED_MULT = 50.0;
static final float GREEN_MULT = 100.0;
static final float BLUE_MULT = 100.0;

static int r;
static int g;
static int b;

static final int AVERAGE_COUNT = 2;
static int rA[] = new int[AVERAGE_COUNT];
static int gA[] = new int[AVERAGE_COUNT];
static int bA[] = new int[AVERAGE_COUNT];
static int i = 0;

void setup() {
  frameRate(FRAME_RATE);
  LED_UPDATE = (1000/FRAME_RATE)-20;
  //println(Serial.list());
  //serial = new Serial(this, "", 115200);
  for(int j = 0; j < AVERAGE_COUNT; j++) {
    rA[j] = 0;
    gA[j] = 0;
    bA[j] = 0;
  }
  size(512, 120);
  minim = new Minim(this);
  //This audio file must be located in the "data" folder on the same level as this source file
  player = minim.loadFile("ct.mp3", 1024);
  player.play();
  fft = new FFT(player.bufferSize(), player.sampleRate());
}

void draw() {
  fft.forward(player.mix);
  
  background(0);
  stroke(255);
  for(int i = 0; i < fft.specSize(); i++) {
    line( i, height, i, height - fft.getBand(i)*8 );
  }
  
  int redIndex = fft.freqToIndex(RED_CENTRE_FREQ);
  float red = 0;
  for(int i = (redIndex-INDEX_AVERAGE); i < (redIndex+INDEX_AVERAGE); i++) {
    red +=fft.getBand(i);
  }
  red /= (INDEX_AVERAGE*2);
  red *= RED_MULT;
  int redOut = (int) red;
  if(redOut > 255) redOut = 255;
  
  int greenIndex = fft.freqToIndex(GREEN_CENTRE_FREQ);
  float green = 0;
  for(int i = (greenIndex-INDEX_AVERAGE); i < (greenIndex+INDEX_AVERAGE); i++) {
    green +=fft.getBand(i);
  }
  green /= (INDEX_AVERAGE*2);
  green *= GREEN_MULT;
  int greenOut = (int) green;
  if(greenOut > 255) greenOut = 255;
  
  int blueIndex = fft.freqToIndex(BLUE_CENTRE_FREQ);
  float blue = 0;
  for(int i = (blueIndex-INDEX_AVERAGE); i < (blueIndex+INDEX_AVERAGE); i++) {
    blue +=fft.getBand(i);
  }
  blue /= (INDEX_AVERAGE*2);
  blue *= BLUE_MULT;
  int blueOut = (int) blue;
  if(blueOut > 255) blueOut = 255;
  
  rA[i] = redOut;
  gA[i] = greenOut;
  bA[i] = blueOut;
  
  r = 0;
  b = 0;
  g = 0;
  for(int j = 0; j < AVERAGE_COUNT; j++) {
    r += rA[j];
    g += gA[j];
    b += bA[j];
  }
  
  r /= AVERAGE_COUNT;
  g /= AVERAGE_COUNT;
  b /= AVERAGE_COUNT;
  
  i++;
  if(i == AVERAGE_COUNT) i = 0;
  
  //setLightsSerial();
  thread("setLights");
}

void setLightsSerial() {
  serial.write("4,"+r+","+g+","+b+";");
}

void setLights() {
  String data = "r="+r+"&g="+g+"&b="+b+"&trans="+LED_TRANS+"&time="+LED_UPDATE;
  postTransact("172.18.16.200", data);
}

void postTransact(String server, String data) {
  httpClient = new Client(this, server, 80); 
  httpClient.write("POST /service  HTTP/1.0\r\n");
  httpClient.write("Host: "+server+" \r\n");
  httpClient.write("Content-Type: application/x-www-form-urlencoded\r\n");
  httpClient.write("Content-Length: 32\r\n");
  httpClient.write("\r\n");
  httpClient.write(data);
  httpClient.write("\r\n");
}
