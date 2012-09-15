/*
 * Toyotomi.ino - Toyotomi HVAC Remote Control
 * 
 * Created by: Kontodimas G. Konstantinos, May 30, 2012
 * Department of Computer Engineering and Informatics
 * University of Patras, Greece
 * 
 * Release into the public domain.
*/


#include <XbeeRadio.h>
#include <XBee.h>
#include <Toyotomi.h>

#include <Uberdust.h>

//#define ZONE_NAME "light"

// Create the xbee object
XBeeRadio xbee = XBeeRadio(); 

// Create reusable response objects for responses we expect to handle 
XBeeRadioResponse response = XBeeRadioResponse();  

// Allocate two bytes for to hold a 32-bit analog reading
uint8_t payload[] = { 
  102, 0,  0, 0, 0, 0};

// 16-bit addressing: Enter address of remote XBee, typically the coordinator
Tx16Request tx = Tx16Request(0xffff, payload, sizeof(payload));

TxStatusResponse txStatus = TxStatusResponse();


uint8_t ledPin = 13;


Uberdust uber = Uberdust();
Toyotomi toyo = Toyotomi(DEFAULT_TEMP, AUTO, DEFAULT_SP, DEFAULT_TIMER, DEFAULT_TIMER, DEFAULT_POWER);

void setup()
{

  xbee.initialize_xbee_module();
  // setup xbee 
  xbee.begin(38400);
  xbee.init();

  uber.setup(&xbee, &tx);
/*
  delay(1000);
  uber.blinkLED(numOfRelays, 200*numOfRelays);
  delay(1000);
*/
  sendCapabilities();
}

void loop()
{
  static unsigned long ledTimestamp = 0;

  if(millis() - ledTimestamp > 5000)
  {
    uber.blinkLED(1,100);
    ledTimestamp = millis();
  }

  if (xbee.checkForData(112))
  {
    xbee.getResponse(response);
    if (response.getData(0) == 1)
    {
      switch(response.getData(1))
      {
         case 1: //temperature
             toyo.setTemperature(response.getData(2));
             sendState(toyo);
             break;
         case 2: //mode
             toyo.setMode((Mode)response.getData(2));
             sendState(toyo);
             break;
         case 3: //fanspeed
             toyo.setFanSpeed((FanSpeed)response.getData(2));
             sendState(toyo);
             break;             
         case 4: //timeron
             toyo.setTimerOn((TimerTime)response.getData(2));
             sendState(toyo);
             break;
         case 5: //timeroff
             toyo.setTimerOff((TimerTime)response.getData(2));
             sendState(toyo);
             break;
         case 6: //poweron
             toyo.powerOn();
             sendState(toyo);
             break;
         case 7: //poweroff
             toyo.powerOff();
             sendState(toyo);
             break;
         case 8: //swing
             toyo.buttonSwing();
             sendState(toyo);
             break;
         /*case 9: //sleep
             toyo.setSleep((bool)response.getData(2));
             sendState(toyo);
             break;*/
         case 10: //airdirection
             toyo.buttonAirDirection();
             sendState(toyo);
             break;
         case 11: //cleanair
             toyo.buttonCleanAir();
             sendState(toyo);
             break;
         case 12: //leddisplay
             toyo.buttonLedDisplay();
             sendState(toyo);
             break;
         case 13: //turbo
             toyo.buttonTurbo();
             sendState(toyo);
             break;
         case 14: //setvalues
             toyo.setState(response.getData(2), (Mode)response.getData(3), (FanSpeed)response.getData(4));
             sendState(toyo);
             break;
         default:
             break;
      }
    }
  }
    
  periodicCapabilities();
}

void periodicCapabilities()
{
  static unsigned long capabTimestamp = 0;
  if(millis() - capabTimestamp > 60000)
  {
    digitalWrite(ledPin, HIGH);
    sendCapabilities();
    digitalWrite(ledPin, LOW);
    delay(100);
    capabTimestamp = millis();
  }
}

void sendCapabilities(void)
{               
  uber.sendValue("report", "airconditioner");
}

void sendState(Toyotomi &toyo)
{
  uber.sendValue("ac_active", String(int(toyo.isPoweredOn())));
  uber.sendValue("ac_temp", String((int)(toyo.getTemperature())));
  uber.sendValue("ac_mode", String(toyo.getMode()));
  uber.sendValue("ac_fanspeed", String(toyo.getFanSpeed()));
  uber.sendValue("ac_timeron", String(toyo.getTimerOn()));
  uber.sendValue("ac_timeroff", String(toyo.getTimerOff()));
  uber.sendValue("ac_sleep", String(int(toyo.isSleepOn())));
}

