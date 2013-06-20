// RGB LED Lighting System
// Dan Nixon 2013
// dan-nixon.com

//Includes
#include <SPI.h>
#include <Ethernet.h>
#include <WebServer.h>
#include <EEPROM.h>

//Pins
static const int RED_PIN = 5;
static const int GREEN_PIN = 6;
static const int BLUE_PIN = 3;
static const int BUTTON_PIN = 12;

//Colour Component Enum
static const int R = 0;
static const int G = 1;
static const int B = 2;

//Transition Mode Enum
static const int NO_EXEC = -1;
static const int INSTANT = 0;
static const int FADE_DIRECT = 1;
static const int FADE_BLACK = 2;

//Hardware Button Mode Enum
static const int LIGHT_OFF = 0;
static const int LIGHT_WEB = 1;
static const int LIGHT_ON = 2;

//Default Values
static const int DEFAULT_TRANSITION = FADE_DIRECT;
static const int DEFAULT_TIME = 200;

//Standard Light Modes
static int OFF[] = {0, 0, 0};
static int FULL_WHITE[] = {255, 255, 255};

//Webserver
static uint8_t MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
static uint8_t IP[] = {192, 168, 1, 200};
WebServer webserver("", 80);

//Variables
int currentColour[3];
int lastWebColour[3];
int buttonLast;
int lightMode;
int lastUsedTransition = DEFAULT_TRANSITION;
int lastUsedTime = DEFAULT_TIME;

//HTML for web front end UI
P(frontEndHTML) = 
  "<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>"
  "<html xmlns='http://www.w3.org/1999/xhtml'>"
  "<head>"
  "<title>RGB Lights</title>"
  "<meta http-equiv='Content-type' content='text/html;charset=UTF-8' />"
  "<style type='text/css'>"
  "div.const-width {"
  "font-family: Arial, 'Helvetica Neue', Helvetica, sans-serif;"
  "width: 300px;"
  "margin-left: auto;"
  "margin-right: auto;"
  "margin-bottom: 5px;"
  "}"
  "#header, #footer {"
  "border: 1px solid blue;"
  "}"
  "p.tight {"
  "padding-top: 0px;"
  "padding-bottom: 0px;"
  "margin-top: 0px;"
  "margin-bottom: 0px;"
  "}"
  "h1 {"
  "font-family: Arial, 'Helvetica Neue', Helvetica, sans-serif;"
  "padding-top: 0px;"
  "padding-bottom: 0px;"
  "margin-top: 0px;"
  "margin-bottom: 0px;"
  "}"
  ".slider, #time-spinner {"
  "margin-top: 10px;"
  "margin-bottom: 10px;"
  "}"
  "#time-spinner {"
  "width: 120px;"
  "}"
  "#transition-select {"
  "width: 150px;"
  "}"
  "</style>"
  "<link href='http://code.jquery.com/ui/1.10.2/themes/smoothness/jquery-ui.css' rel='stylesheet' />"
  "<script type='text/javascript' src='http://code.jquery.com/jquery-1.9.1.js'></script>"
  "<script type='text/javascript' src='http://code.jquery.com/ui/1.10.2/jquery-ui.js'></script>"
  "<script type='text/javascript'>"
  "$(function() {"
  "var sliderOptions = {min: 0, max: 255, step: 1, stop: updateHandler};"
  "var tSpinnerOptions = {min: 0, max: 2000, stop: 1};"
  "$('#red-slider').slider(sliderOptions);"
  "$('#green-slider').slider(sliderOptions);"
  "$('#blue-slider').slider(sliderOptions);"
  "$('#white-slider').slider({min: 0, max: 255, step: 1, stop: whiteHandler});"
  "$('#time-spinner').spinner(tSpinnerOptions);"
  "$('#white-button')"
  ".button()"
  ".click(function() {"
  "masterHandler(255, 255, 50);"
  "});"
  "$('#full-on-button')"
  ".button()"
  ".click(function() {"
  "masterHandler(255, 255, 255);"
  "});"
  "$('#off-button')"
  ".button()"
  ".click(function() {"
  "masterHandler(0, 0, 0);"
  "});"
  "$('#refresh-button')"
  ".button()"
  ".click(function() {"
  "getLightState();"
  "});"
  "getLightState();"
  "});"
  "function whiteHandler() {"
  "var whiteLightLevel = $('#white-slider').slider('values', 0);"
  "masterHandler(whiteLightLevel, whiteLightLevel, whiteLightLevel);"
  "}"
  "function masterHandler(r, g, b) {"
  "$('#red-slider').slider('value', r);"
  "$('#green-slider').slider('value', g);"
  "$('#blue-slider').slider('value', b);"
  "updateHandler();"
  "}"
  "function setLights(red, green, blue, transition, ttime) {"
  "$.post('http://192.168.1.200/service', {r: red, g: green, b: blue, trans: transition, time: ttime})"
  ".done(function(data) {"
  "handleXMLresponse(data);"
  "});"
  "}"
  "function getLightState() {"
  "$.get('http://192.168.1.200/service')"
  ".done(function(data) {"
  "handleXMLresponse(data);"
  "});"
  "}"
  "function handleXMLresponse(data) {"
  "var parser = new DOMParser();"
  "var doc = parser.parseFromString(data, 'text/xml');"
  "xmlDoc = $.parseXML(data);"
  "$xml = $(xmlDoc);"
  "var red = $xml.find('r').text();"
  "var green = $xml.find('g').text();"
  "var blue = $xml.find('b').text();"
  "var transition = $xml.find('lastTransition').text();"
  "var time = $xml.find('lastTime').text();"
  "$('#red-slider').slider('value', red);"
  "$('#green-slider').slider('value', green);"
  "$('#blue-slider').slider('value', blue);"
  "$('#time-spinner').spinner('value', time);"
  "var average = (parseInt(red) + parseInt(green) + parseInt(blue)) / 3;"
  "$('#white-slider').slider('value', average);"
  "document.getElementById('transition-select').value = transition;"
  "}"
  "function updateHandler() {"
  "var red = $('#red-slider').slider('values', 0);"
  "var green = $('#green-slider').slider('values', 0);"
  "var blue = $('#blue-slider').slider('values', 0);"
  "var time = $( '#time-spinner' ).spinner('value');"
  "var transition = document.getElementById('transition-select').value;"
  "setLights(red, green, blue, transition, time);"
  "}"
  "</script>"
  "</head>"
  "<body>"
  "<div class='const-width' id='header'>"
  "<h1>RGB LED Lighting</h1>"
  "</div>"
  "<div class='const-width' id='colour-sliders'>"
  "<div class='slider' id='red-slider' style='background:red'></div>"
  "<div class='slider' id='green-slider' style='background:green'></div>"
  "<div class='slider' id='blue-slider' style='background:blue'></div>"
  "<div class='slider' id='white-slider'></div>"
  "</div>"
  "<div class='const-width' id='time-selector'>"
  "<label>Transition Time:</label>"
  "<input id='time-spinner' name='ttime' />"
  "</div>"
  "<div class='const-width' id='transition-selector'>"
  "<label>Transition Type:</label>"
  "<select id='transition-select'>"
  "<option value='0'>No Transition</option>"
  "<option value='1'>Fade</option>"
  "<option value='2'>Fade Through Black</option>"
  "</select>"
  "</div>"
  "<div class='const-width' id='function-buttons'>"
  "<button id='white-button'>White</button>"
  "<button id='full-on-button'>Full On</button>"
  "<button id='off-button'>Off</button>"
  "<button id='refresh-button'>Refresh</button>"
  "</div>"
  "<div class='const-width' id='footer'>"
  "<p class='tight'><a href='http://dan-nixon.com' >Dan Nixon</a> 2013</p>"
  "</div>"
  "</body>"
  "</html>";

//Serves web front end to control light from a web browser
void webUI(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  server.httpSuccess();
  if (type == WebServer::GET) {
    server.printP(frontEndHTML);
  }
}

//Provides back end to control lights from front end and other apps
void webBackend(WebServer &server, WebServer::ConnectionType type, char *, bool) {
  char name[16];
  char value[16];
  server.httpSuccess();
  int transition = NO_EXEC;
  if(type == WebServer::POST) {
    int colour[3];
    int time = DEFAULT_TIME;
    while (server.readPOSTparam(name, 16, value, 16)) {
      if(strcmp(name, "r") == 0) colour[R] = atoi(value);
      if(strcmp(name, "g") == 0) colour[G] = atoi(value);
      if(strcmp(name, "b") == 0) colour[B] = atoi(value);
      if(strcmp(name, "trans") == 0) transition = atoi(value);
      if(strcmp(name, "time") == 0) time = atoi(value);
    }
    if(transition != NO_EXEC) {
      lightMode = LIGHT_WEB;
      lastWebColour[R] = colour[R];
      lastWebColour[G] = colour[G];
      lastWebColour[B] = colour[B];
      lightChange(colour, transition, time);
    }
  }
  if((type == WebServer::POST) || (type == WebServer::GET)) {
    server.println("<xml>");
    server.println("<currentColour>");
    server.print("<r>");
    server.print(currentColour[R]);
    server.println("</r>");
    server.print("<g>");
    server.print(currentColour[G]);   
    server.println("</g>");
    server.print("<b>");
    server.print(currentColour[B]);
    server.println("</b>");
    server.println("</currentColour>");
    server.print("<mode>");
    server.print(lightMode);
    server.println("</mode>");
    server.print("<lastTime>");
    server.print(lastUsedTime);
    server.println("</lastTime>");
    server.print("<lastTransition>");
    server.print(lastUsedTransition);
    server.println("</lastTransition>");
    server.print("<exec>");
    if(transition == NO_EXEC) server.print("FALSE");
    else server.print("TRUE");
    server.println("</exec>");
    server.println("</xml>");
  }
}

//Used to handle a change in lighting
void lightChange(int colour[], int transition, int time) {
  if(transition == NO_EXEC) return;
  int oldColour[3];
  oldColour[R] = currentColour[R];
  oldColour[G] = currentColour[G];
  oldColour[B] = currentColour[B];
  switch(transition) {
    case INSTANT:
      setRGB(colour);
      break;
    case FADE_DIRECT:
      fade(oldColour, colour, time);
      break;
    case FADE_BLACK:
      int timeHalf = time / 2;
      fade(oldColour, OFF, timeHalf);
      fade(OFF, colour, timeHalf);
      break;
  }
  lastUsedTime = time;
  lastUsedTransition = transition;
}

//Controls a smooth lighting fade
void fade(int startColour[], int endColour[], int fadeTime) {
  for(int t = 0; t < fadeTime; t++) {
    int colour[3];
    colour[R] = map(t, 0, fadeTime, startColour[R], endColour[R]);
    colour[G] = map(t, 0, fadeTime, startColour[G], endColour[G]);
    colour[B] = map(t, 0, fadeTime, startColour[B], endColour[B]);
    setRGB(colour);
    delay(1);
  }
  setRGB(endColour);
}

//Sets an RGB colour
void setRGB(int colour[3]) {
  if(colour[R] < 0) colour[R] = 0;
  if(colour[R] > 255) colour[R] = 255;
  if(colour[G] < 0) colour[G] = 0;
  if(colour[G] > 255) colour[G] = 255;
  if(colour[B] < 0) colour[B] = 0;
  if(colour[B] > 255) colour[B] = 255;
  analogWrite(RED_PIN, colour[R]);
  analogWrite(GREEN_PIN, colour[G]);
  analogWrite(BLUE_PIN, colour[B]);
  currentColour[R] = colour[R];
  currentColour[G] = colour[G];
  currentColour[B] = colour[B];
}

void buttonHandler() {
  switch(lightMode) {
    case LIGHT_OFF:
      lightMode = LIGHT_WEB;
      lightChange(lastWebColour, lastUsedTransition, lastUsedTime);
      break;
    case LIGHT_WEB:
      lightMode = LIGHT_ON;
      lightChange(FULL_WHITE, lastUsedTransition, lastUsedTime);
      break;
    case LIGHT_ON:
      lightMode = LIGHT_OFF;
      lightChange(OFF, lastUsedTransition, lastUsedTime);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  Ethernet.begin(MAC, IP);
  webserver.setDefaultCommand(&webUI);
  webserver.addCommand("index", &webUI);
  webserver.addCommand("service", &webBackend);
  webserver.begin();
  buttonLast = digitalRead(BUTTON_PIN);
  lightMode = LIGHT_OFF;
}

void loop() {
  char buff[64];
  int len = 64;
  webserver.processConnection(buff, &len);
  int buttonState = digitalRead(BUTTON_PIN);
  if(buttonState != buttonLast) {
    if(buttonState == LOW) {
      buttonHandler();
    }
    buttonLast = buttonState;
  }
}
