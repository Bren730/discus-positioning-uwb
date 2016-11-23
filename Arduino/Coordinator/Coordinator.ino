byte addresses[4];
uint16_t distances[4];
byte deviceCount = 0;

#include <Wire.h>

void setup() {

  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);

  Wire.begin(0);
  Wire.onReceive(receiveEvent);

  Serial.println("Starting ranging");

  i2cScan();

}

void loop() {

  for (byte i = 0; i < deviceCount; i++) {

    //Serial.println("Requesting bytes from " + String(addresses[i]));
    // Request two bytes from one of the anchors
    Wire.requestFrom(addresses[i], 2);

    byte high;
    byte low;
    uint16_t distance;
    byte buffer[2];
    byte bufferIndex = 0;

    while (Wire.available()) {

      buffer[bufferIndex] = Wire.read();
      bufferIndex++;
      
    }

    distance = buffer[0];
    distance = distance << 8;
    distance |= buffer[1];
    Serial.println(String(distance / 1000.0));
    
    
  }

  delay(1000);
  
}

void receiveEvent(int count) {

  while (Wire.available() > 0) {

    char c = Wire.read();
    Serial.println(c);

  }

}

void i2cScan() {

  byte error, address;
  byte i = 0;

  for (address = 0; address < 128; address++) {

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    //Serial.println(String(address) + " " + String(error));

    if (error == 0) {

      addresses[i] = address;

      deviceCount++;
      i++;

    }

    if (error == 4) {

      Serial.println("Unknown error at address "  + String(address));
      
    }

  }

  if (deviceCount == 0) {

    Serial.println("No devices found :(");

  } else {

    Serial.println("Using the following devices");

    for (byte j = 0; j < deviceCount; j++) {
      
      Serial.println(addresses[j]);
      
    }
  }

}

