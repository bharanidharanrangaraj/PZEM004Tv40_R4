/*
  Basic Reading Example
  Copyright (c) 2026 Bharani Dharan Rangaraj
  MIT License
  
  Reads all parameters from PZEM-004T and displays on Serial Monitor
  
  Hardware:
  - Arduino UNO R4 (Minima or WiFi)
  - PZEM-004T V4.0 (100A)
  
  Connections:
  - PZEM TX -> Arduino Pin 0 (RX)
  - PZEM RX -> Arduino Pin 1 (TX)
  - PZEM 5V -> Arduino 5V
  - PZEM GND -> Arduino GND
*/

#include <PZEM004Tv40_R4.h>

// Create PZEM object on Serial1
PZEM004Tv40_R4 pzem(&Serial1);

void setup()
{
  Serial.begin(115200);
  pzem.begin();
  
  Serial.println("PZEM-004T-100A Energy Monitor");
  Serial.println("==============================");
  delay(1000);
}

void loop()
{
  // Read all parameters
  if (pzem.readAll())
  {
    Serial.println("------------------------");
    
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