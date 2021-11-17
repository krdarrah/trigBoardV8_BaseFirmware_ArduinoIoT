#define OTA_DEBUG

#include "includes.h"
#include "thingProperties.h"//for arduino cloud
char temperatureString[5];//where the temperature in F/C is stored
boolean getMCP9808();

const char fwVersion[] = "11/14/21_IoTCloud";

void setup() {

  pinMode(ESPlatchPin, OUTPUT);
  digitalWrite(ESPlatchPin, HIGH);
  pinMode(LEDpin, OUTPUT);
  Serial.begin(115200);
  Wire.begin(SDApin, SCLpin);
  checkWakeupPins();
  loadConfiguration(filename, config);
  rtcInit(config.timerCountDown, false);
  Serial.println(getBattery(), 2);
  getMCP9808();//temperature unit is set in button message F or C

  arduinoTemperature = atof(temperatureString);
  arduinoBatteryVoltage = getBattery();
  arduinoContact = contactStatusClosed;//arduinoContact=true when contact closed
  initProperties();//ArduinoIoTCloud
  sendArduinoIoT();

  if (pushLogic()) { //decide if push will occur or nt and what message will be
    timestampAppend();
    if (wiFiNeeded) {
      if (connectWiFi()) {
        nptUpdateTime();
        //pushOver();//using these keys for Arduino IOT
        //pushSafer();//using these keys for Arduino IOT
        ifttt();
        telegram();
        mqtt();
      }
    }
    udp();
    tcp();
  }
  killPower();
  waitForButton();
  initBluetooth();
}

void loop() {
  if (!OTAsetup)
    serviceBluetooth();
  else
    checkOTA();
}
