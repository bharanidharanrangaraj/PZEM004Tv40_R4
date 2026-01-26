/*
  Basic Reading Example for Arduino UNO R3/Nano
  Copyright (c) 2026 Bharani Dharan Rangaraj
  MIT License
  
  Hardware:
  - Arduino UNO R3 / Nano / Mega
  - PZEM-004T V4.0 (100A)
  
  Connections:
  - PZEM TX -> Arduino Pin 2 (RX/SoftwareSerial)
  - PZEM RX -> Arduino Pin 3 (TX/SoftwareSerial)
  - PZEM 5V -> Arduino 5V
  - PZEM GND -> Arduino GND
*/

#include <SoftwareSerial.h>
#include <PZEM004Tv40_R4.h>

// Create SoftwareSerial for PZEM (RX=Pin2, TX=Pin3)
SoftwareSerial pzemSerial(2, 3);

// Create PZEM object on SoftwareSerial
PZEM004Tv40_R4 pzem(&pzemSerial);

void setup()
{
  Serial.begin(115200);
  pzemSerial.begin(9600);
  pzem.begin();
  
  Serial.println("PZEM-004T V4.0 Energy Monitor");
  Serial.println("Arduino UNO R3/Nano");
  Serial.println("==============================");
  delay(1000);
}

void loop()
{
  // Read all parameters
  if (pzem.readAll())
  {
    Serial.print("Voltage:      ");
    Serial.print(pzem.getVoltage(), 1);
    Serial.println(" V");
    
    Serial.print("Current:      ");
    Serial.print(pzem.getCurrent(), 3);
    Serial.println(" A");
    
    Serial.print("Power:        ");
    Serial.print(pzem.getPower(), 1);
    Serial.println(" W");
    
    Serial.print("Energy:       ");
    Serial.print(pzem.getEnergy(), 3);
    Serial.println(" kWh");
    
    Serial.print("Frequency:    ");
    Serial.print(pzem.getFrequency(), 1);
    Serial.println(" Hz");
    
    Serial.print("Power Factor: ");
    Serial.println(pzem.getPowerFactor(), 2);
    
    Serial.print("Alarm:        ");
    Serial.println(pzem.getAlarm() ? "YES" : "NO");
    
    Serial.println("------------------------");
  }
  else
  {
    Serial.print("Error reading PZEM! Code: ");
    Serial.println(pzem.getLastError());
  }
  
  delay(1000);
}