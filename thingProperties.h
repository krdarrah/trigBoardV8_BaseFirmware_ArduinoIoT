#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
float arduinoTemperature;
float arduinoBatteryVoltage;
boolean arduinoContact;

//**************************************
//THING ID is Pushover User Key
//Device ID is Pushover API Token/Key
//Secret Key is Push Safer Key
//**************************************

boolean doneSendingToArduino = false;
boolean connectedToArduino = false;

WiFiConnectionHandler ArduinoIoTPreferredConnection(config.ssid, config.pw);

void onIoTConnect() {
  Serial.println(">>> connected to Arduino IoT Cloud");
  connectedToArduino = true;
}
void onIoTDisconnect() {
  Serial.println(">>> disconnected to Arduino IoT Cloud");
}

void onIoTSync() {
  Serial.println(">>> Board and Cloud SYNC OK");
  doneSendingToArduino = true;
}

void initProperties() {

  ArduinoCloud.setBoardId(config.pushAPIKey);
  ArduinoCloud.setSecretDeviceKey(config.pushSaferKey);
  ArduinoCloud.setThingId(config.pushUserKey);
  ArduinoCloud.addProperty(arduinoTemperature, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(arduinoBatteryVoltage, READ, 1 * SECONDS, NULL);
  ArduinoCloud.addProperty(arduinoContact, READ, 1 * SECONDS, NULL);

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, onIoTConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, onIoTDisconnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, onIoTSync);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void sendArduinoIoT() {
  unsigned long wifiStart = millis();
  while (millis() - wifiStart < config.wifiTimeout) {
    ArduinoCloud.update();
    if (doneSendingToArduino)
      break;
    if (connectedToArduino) {
      connectedToArduino = false;
      wifiStart = millis(); //now give more time for server update
    }
  }
  wifiStart = millis();
  while (millis() - wifiStart < 1000) {//just one more second of sync
    ArduinoCloud.update();
  }
  pinMode(LEDpin, OUTPUT);
}
