# Control
## Analog control test bed for Arduino and Raspberry Pi

This repo contains code for Arduino and raspberry pi.  the code is quite different but the purpose is the same.  we want to control some on/off switch or relay based on a value recieved from an analog sensor.  

The system may not be in place so the code here drives a test bed where the analog signals are driven by potentiometers and the on/of signal is modeled with an led.  

####Arduino
The arduino code also includes UTFT code to drive a TFT touch screen through a CTE control board.  We use the SainSmar screen and CTE Mega shield for this.  The code assumes an Arduino Mega 2560 dev board is being used.

####Raspberry Pi
The RPi code includes An SPI read function needed to talk to the MCP3008 8-channel 10-bit ADC chip.  The chip Data sheet can be found [here](https://cdn-shop.adafruit.com/datasheets/MCP3008.pdf).



 