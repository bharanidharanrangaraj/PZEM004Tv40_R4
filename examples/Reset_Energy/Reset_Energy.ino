/*
  Reset Energy Counter Example for PZEM004Tv40_R4
  
  This sketch demonstrates how to reset the accumulated energy counter
  on the PZEM-004T V4.0 device back to zero.
  
  WARNING: This will permanently reset the energy counter!
           Make sure to record the current energy value if needed.
  
  Hardware:
  - Arduino UNO R4 (Minima or WiFi)
  - PZEM-004T V4.0 (100A version)
  
  Connections:
  - PZEM TX -> Arduino Pin 0 (RX)
  - PZEM RX -> Arduino Pin 1 (TX)
  - PZEM 5V -> Arduino 5V
  - PZEM GND -> Arduino GND
  
  Instructions:
  1. Upload this sketch
  2. Open Serial Monitor (115200 baud)
  3. Follow the prompts to reset the energy counter
*/

#include <PZEM004Tv40_R4.h>

// Create PZEM object on Serial1
PZEM004Tv40_R4 pzem(&Serial1);

void setup()
{
  Serial.begin(115200);
  while (!Serial);  // Wait for Serial Monitor to open
  
  pzem.begin();
  
  Serial.println("========================================");
  Serial.println("PZEM-004T V4.0 Energy Counter Reset");
  Serial.println("========================================");
  Serial.println();
  
  delay(1000);
  
  // First, read the current energy value
  Serial.println("Reading current energy value...");
  if (pzem.readAll())
  {
    float currentEnergy = pzem.getEnergy();
    
    Serial.println("✓ Connection successful!");
    Serial.println();
    Serial.println("Current readings:");
    Serial.println("-------------------------------------------");
    Serial.print("  Voltage:      ");
    Serial.print(pzem.getVoltage(), 1);
    Serial.println(" V");
    
    Serial.print("  Current:      ");
    Serial.print(pzem.getCurrent(), 3);
    Serial.println(" A");
    
    Serial.print("  Power:        ");
    Serial.print(pzem.getPower(), 1);
    Serial.println(" W");
    
    Serial.print("  Energy:       ");
    Serial.print(currentEnergy, 3);
    Serial.println(" kWh  ← Will be reset");
    
    Serial.print("  Frequency:    ");
    Serial.print(pzem.getFrequency(), 1);
    Serial.println(" Hz");
    
    Serial.print("  Power Factor: ");
    Serial.println(pzem.getPowerFactor(), 2);
    Serial.println("-------------------------------------------");
    Serial.println();
    
    // Warning message
    Serial.println("⚠ WARNING ⚠");
    Serial.println("This will permanently reset the energy counter!");
    Serial.print("Current energy value (");
    Serial.print(currentEnergy, 3);
    Serial.println(" kWh) will be lost.");
    Serial.println();
    
    // Ask for confirmation
    Serial.println("Type 'YES' and press Enter to continue...");
    Serial.println("Type anything else to cancel.");
    Serial.println();
    
    // Wait for user input
    String input = "";
    unsigned long startWait = millis();
    
    while (input.length() == 0 && (millis() - startWait < 30000))  // 30 second timeout
    {
      if (Serial.available())
      {
        input = Serial.readStringUntil('\n');
        input.trim();
        input.toUpperCase();
      }
    }
    
    if (input.length() == 0)
    {
      Serial.println();
      Serial.println("❌ Timeout - Reset cancelled.");
      Serial.println("Energy counter was NOT reset.");
      return;
    }
    
    if (input == "YES")
    {
      Serial.println();
      Serial.println("Resetting energy counter...");
      delay(500);
      
      if (pzem.resetEnergy())
      {
        Serial.println("✓ Energy counter reset successfully!");
        Serial.println();
        
        // Verify the reset
        delay(1000);
        Serial.println("Verifying reset...");
        
        if (pzem.readAll())
        {
          float newEnergy = pzem.getEnergy();
          Serial.print("New energy value: ");
          Serial.print(newEnergy, 3);
          Serial.println(" kWh");
          
          if (newEnergy < 0.001)  // Should be zero or very close
          {
            Serial.println("✓ Reset verified!");
          }
          else
          {
            Serial.println("⚠ Energy value not zero.");
            Serial.println("   Try power cycling the PZEM.");
          }
        }
        else
        {
          Serial.println("⚠ Could not verify reset.");
          Serial.print("   Error code: ");
          Serial.println(pzem.getLastError());
        }
        
        Serial.println();
        Serial.println("========================================");
        Serial.println("Reset complete!");
        Serial.println("========================================");
      }
      else
      {
        Serial.println("✗ Failed to reset energy counter!");
        Serial.print("Error code: ");
        Serial.println(pzem.getLastError());
        Serial.println();
        
        // Detailed error messages
        switch(pzem.getLastError())
        {
          case PZEM004Tv40_R4::ERROR_TIMEOUT:
            Serial.println("ERROR: Timeout - No response from PZEM");
            Serial.println("Possible causes:");
            Serial.println("  1. PZEM is not powered");
            Serial.println("  2. Wiring is incorrect");
            Serial.println("  3. PZEM address mismatch");
            break;
            
          case PZEM004Tv40_R4::ERROR_CRC:
            Serial.println("ERROR: CRC verification failed");
            Serial.println("Possible causes:");
            Serial.println("  1. Communication interference");
            Serial.println("  2. Loose wiring");
            Serial.println("  3. PZEM firmware issue");
            Serial.println();
            Serial.println("Try:");
            Serial.println("  - Check all connections");
            Serial.println("  - Use shorter wires");
            Serial.println("  - Add 120Ω termination resistor");
            break;
            
          case PZEM004Tv40_R4::ERROR_INVALID_RESPONSE:
            Serial.println("ERROR: Invalid response from PZEM");
            Serial.println("Possible causes:");
            Serial.println("  1. Wrong PZEM model");
            Serial.println("  2. Address mismatch");
            Serial.println("  3. Corrupted data");
            break;
            
          default:
            Serial.println("ERROR: Unknown error");
            break;
        }
        
        Serial.println();
        Serial.println("Troubleshooting:");
        Serial.println("  - Verify PZEM is V4.0 (100A model)");
        Serial.println("  - Check wiring:");
        Serial.println("    PZEM TX -> Arduino RX (Pin 0)");
        Serial.println("    PZEM RX -> Arduino TX (Pin 1)");
        Serial.println("  - Power cycle both devices");
        Serial.println("  - Try again");
      }
    }
    else
    {
      Serial.println();
      Serial.println("❌ Reset cancelled by user.");
      Serial.println("Energy counter was NOT reset.");
    }
  }
  else
  {
    Serial.println("✗ Failed to connect to PZEM!");
    Serial.println();
    Serial.println("Please check:");
    Serial.println("  1. PZEM is powered (LED should be on)");
    Serial.println("  2. Wiring is correct:");
    Serial.println("     - PZEM TX -> Arduino RX (Pin 0)");
    Serial.println("     - PZEM RX -> Arduino TX (Pin 1)");
    Serial.println("     - PZEM 5V -> Arduino 5V");
    Serial.println("     - PZEM GND -> Arduino GND");
    Serial.println("  3. PZEM address is 0x01 (default)");
    Serial.println("  4. PZEM model is V4.0 (100A)");
    Serial.println();
    Serial.print("Error code: ");
    Serial.println(pzem.getLastError());
  }
}

void loop()
{
  // Nothing to do here
}