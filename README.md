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

After constructing a driver similar to that shown in the schmatic upload the sketch provided to your Arduino, changing the IP and I/O pins as needed.

A better guide is available on Instructables [here](http://www.instructables.com/id/Arduino-Web-Enabled-RGB-Lighting/).

Usage
-----

The internal web control page can be accessed by navigating to the IP address assigned to the controller.

The Android app can be configured by providing the URL to the webservice, this is ```http://IP_OF_CONTROLLER/service```

POST API
--------

The HTTP POST API takes 5 parameters:
-	```trans``` = Transition type: 0=No Transition, 1=Fade, 2=Fade Through Black
-	```time``` = Time taken for transition (no effect for trans=0)
-	```r``` = Red light level (0<=r<=255)
-	```g``` = Green light level (0<=g<=255)
-	```b``` = Blue light level (0<=b<=255)

The controller will respond with an XML formatted response giving details of the last request, an example of which is given below.

```
<?xml version="1.0"?>
<xml>
<currentColour>
<r>255</r>
<g>255</g>
<b>140</b>
</currentColour>
<mode>1</mode>
<lastTime>500</lastTime>
<lastTransition>1</lastTransition>
<exec>FALSE</exec>
</xml>
```
