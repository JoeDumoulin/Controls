# sesh_control.py - this file provides commands for controlling analog 
#  input and digital output we need for sesh.  Analog inputs are modeled 
#  as pot values that come to the pi GPIO through a MCP3008 8-channel 
#  10-bit ADC.  These values are meant to emulate temperature and pressure 
#  sensors in the actual electronics.  
#
#  The digital outputs are controlling leds  (on under some condition, 
#  off under another). The leds emulate relays which will be activated 
#  and deactivated by the digital signal. 

import time
import os
import RPi.GPIO as GPIO

# (mosi,miso,clk,cs) = getSpi():
def getSpi():
  return (10,9,11,8)
 
def setPins(CLK, MOSI, MISO, CS):
  GPIO.setmode(GPIO.BCM)

  # set up the SPI interface pins
  GPIO.setup(MOSI, GPIO.OUT)
  GPIO.setup(MISO, GPIO.IN)
  GPIO.setup(CLK, GPIO.OUT)
  GPIO.setup(CS, GPIO.OUT)
 
# read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
def readadc(adcnum):
        (mosipin, misopin, clockpin, cspin) = getSpi()
        if ((adcnum > 7) or (adcnum < 0)):
                return -1
        GPIO.output(cspin, True)
 
        GPIO.output(clockpin, False)  # start clock low
        GPIO.output(cspin, False)     # bring CS low
 
        commandout = adcnum
        commandout |= 0x18  # start bit + single-ended bit
        commandout <<= 3    # we only need to send 5 bits here
        for i in range(5):
                if (commandout & 0x80):
                        GPIO.output(mosipin, True)
                else:
                        GPIO.output(mosipin, False)
                commandout <<= 1
                GPIO.output(clockpin, True)
                GPIO.output(clockpin, False)
 
        adcout = 0
        # read in one empty bit, one null bit and 10 ADC bits
        for i in range(12):
                GPIO.output(clockpin, True)
                GPIO.output(clockpin, False)
                adcout <<= 1
                if (GPIO.input(misopin)):
                        adcout |= 0x1
 
        GPIO.output(cspin, True)
        
        adcout >>= 1       # first bit is 'null' so drop it
        return adcout
 
def main():
  (mosi,miso,clk,cs) = getSpi();
  setPins(clk, mosi, miso, cs)

  last_read = 0       # this keeps track of the last potentiometer value
  tolerance = 5       # to keep from being jittery we'll only change
                    # volume when the pot has moved more than 5 'counts'
  trim_pot = [0,0]
  trim_pot_changed = [0,0]
  last_read = [0,0]

  while True:
    for adc in [0,1]:
        # we'll assume that the pot didn't move
        trim_pot_changed[adc] = False
 
        # read the analog pin
        trim_pot[adc] = readadc(adc)
        # how much has it changed since the last read?
        pot_adjust = abs(trim_pot[adc] - last_read[adc])
 
        if ( pot_adjust > tolerance ):
               trim_pot_changed[adc] = True
 
        if ( trim_pot_changed[adc] ):
                print "adc channel:", adc
                print "trim_pot:", trim_pot[adc]
                print "pot_adjust:", pot_adjust
                print "last_read", last_read[adc]
 
                # save the potentiometer reading for the next loop
                last_read[adc] = trim_pot[adc]
 
        # hang out and do nothing for a half second
        time.sleep(0.05)

if __name__ == '__main__':
  main()

