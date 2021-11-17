#include "Adafruit_MCP9808.h"
// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


boolean getMCP9808() {
  float tempC = 0;
  float tempF = 0;
  
  unsigned long getMCP9808startTime = millis();
  if (!tempsensor.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }

  Serial.println("Found MCP9808!");

  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms




  Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
  tempsensor.wake();   // wake up, ready to read!

  // Read and print out the temperature, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempsensor.getResolution());
  tempC = tempsensor.readTempC();
  tempF = tempsensor.readTempF();
  Serial.print("Temp: ");
  Serial.print(tempC, 4); Serial.print("*C\t and ");
  Serial.print(tempF, 4); Serial.println("*F.");
  Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling

  if (strcmp(config.buttonMessage, "C") == 0) {
    Serial.println("Sending  in C");
    dtostrf(tempC, 1, 2, temperatureString);//convert float to str
  } else { //in F
    Serial.println("Sending  in F");
    dtostrf(tempF, 1, 2, temperatureString);//convert float to str
  }
  Serial.print("ms to measure ");
  Serial.println(millis() - getMCP9808startTime);
  return true;
}
