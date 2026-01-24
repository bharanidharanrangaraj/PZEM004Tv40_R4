/*
  Change Address Example for PZEM004Tv40_R4
  
  This sketch demonstrates how to change the Modbus address of your PZEM-004T device.
  
  WARNING: Use this with caution! Make sure only ONE PZEM device is connected
  when changing addresses to avoid conflicts.
  
  Default PZEM address is 0x01 (1)
  Valid addresses range from 0x01 to 0xF7 (1 to 247)
  
  Hardware:
  - Arduino UNO R4 (Minima or WiFi)
  - PZEM-004T-100A
  
  Connections:
  - PZEM TX -> Arduino Pin 0 (RX)
  - PZEM RX -> Arduino Pin 1 (TX)
  - PZEM 5V -> Arduino 5V
  - PZEM GND -> Arduino GND
*/

#include <PZEM004Tv40_R4.h>

// Current address of your PZEM (default is 0x01)
#define CURRENT_ADDRESS 0x01

// New address you want to set (change this to your desired address)
#define NEW_ADDRESS 0x05

// Create PZEM object with current address
PZEM004Tv40_R4 pzem(&Serial1, CURRENT_ADDRESS);

void setup()
{
  Serial.begin(115200);
  pzem.begin();
  
  Serial.println("========================================");
  Serial.println("PZEM-004T Address Change Utility");
  Serial.println("========================================");
  Serial.println();
  
  delay(1000);
  
  // Display current configuration
  Serial.print("Current Address: 0x");
  Serial.println(CURRENT_ADDRESS, HEX);
  Serial.print("New Address:     0x");
  Serial.println(NEW_ADDRESS, HEX);
  Serial.println();
  
  // Verify address is valid
  if (NEW_ADDRESS < 0x01 || NEW_ADDRESS > 0xF7)
  {
    Serial.println("ERROR: Invalid address!");
    Serial.println("Valid range: 0x01 to 0xF7 (1 to 247)");
    while(1);  // Stop here
  }
  
  // Test connection with current address
  Serial.println("Testing connection with current address...");
  if (pzem.readAll())
  {
    Serial.println("Connection successful!");
    Serial.print("Voltage: ");
    Serial.print(pzem.getVoltage(), 1);
    Serial.println(" V");
    Serial.println();
  }
  else
  {
    Serial.println("Connection failed!");
    Serial.println("Please check:");
    Serial.println("1. PZEM is powered");
    Serial.println("2. Wiring is correct");
    Serial.println("3. Current address is correct");
    Serial.print("Error code: ");
    Serial.println(pzem.getLastError());
    while(1);  // Stop here
  }
  
  // Change address
  Serial.println("Changing address...");
  if (pzem.setAddress(NEW_ADDRESS))
  {
    Serial.println("Address changed successfully!");
    Serial.println();
    
    // Verify with new address
    Serial.println("Verifying new address...");
    PZEM004Tv40_R4 pzemNew(&Serial1, NEW_ADDRESS);
    pzemNew.begin();
    delay(100);
    
    if (pzemNew.readAll())
    {
      Serial.println("Verification successful!");
      Serial.print("New address 0x");
      Serial.print(NEW_ADDRESS, HEX);
      Serial.println(" is working correctly.");
      Serial.print("Voltage: ");
      Serial.print(pzemNew.getVoltage(), 1);
      Serial.println(" V");
      Serial.println();
      Serial.println("========================================");
      Serial.println("Address change completed successfully!");
      Serial.println("========================================");
      Serial.println();
      Serial.println("IMPORTANT: Remember to update your code");
      Serial.println("to use the new address in future sketches.");
    }
    else
    {
      Serial.println("Verification failed!");
      Serial.println("Address may have changed but verification");
      Serial.println("could not be completed. Try power cycling");
      Serial.println("the PZEM and testing with the new address.");
    }
  }
  else
  {
    Serial.println("Address change failed!");
    Serial.print("Error code: ");
    Serial.println(pzem.getLastError());
    Serial.println();
    Serial.println("Possible reasons:");
    Serial.println("1. Communication error");
    Serial.println("2. PZEM did not respond");
    Serial.println("3. Invalid address value");
  }
}

void loop()
{
  // Nothing to do here
}