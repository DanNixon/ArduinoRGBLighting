ArduinoRGBLighting
==================

Arduino powered RGB lighting controller with HTTP POST API, self hosted web control panel and Android application.

Requirements
------------

-	[Webduino](https://github.com/sirleech/Webduino) which is used to provide a better HTTP server on the Arduino
-	[jQuery](http://jquery.com/) is used for the web control page, however all that is requiered is a connection to the internet on the client as the JS is pulled from the jQuery site
-	There is possibly more, I will update the list as I find them

Installation/Setup
------------------

Usage
-----

The internal web control page can be accessed by navigating to the IP address assigned to the controller.

The Android app can be configured by providing the URL to the webservice, this is http://IP_OF_CONTROLLER/service

POST API
--------

The HTTP POST API takes 5 parameters:
-	trans = Transition type: 0=No Transition, 1=Fade, 2=Fade Through Black
-	time = Time taken for transition
-	r = Red light level
-	g = Green light level
-	b = Blue light level

The controller will respond with an XML formatted response giving details of the last request.
