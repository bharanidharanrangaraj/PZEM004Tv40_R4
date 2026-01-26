/*
  Individual Parameters Example (Universal)
  Copyright (c) 2026 Bharani Dharan Rangaraj
  MIT License

  This sketch demonstrates two methods of reading PZEM data:
  1. readAll() - Efficient method that reads all parameters at once (RECOMMENDED)
  2. Individual read functions - Less efficient but useful for specific needs

  Works on Arduino UNO R3, R4, Nano, Mega automatically

  Hardware:
  - Arduino UNO R3/R4/Nano/Mega
  - PZEM-004T V4.0 (100A)

  Connections for UNO R4:
  - PZEM TX -> Arduino Pin 0 (RX/Serial1)
  - PZEM RX -> Arduino Pin 1 (TX/Serial1)

  Connections for UNO R3/Nano:
  - PZEM TX -> Arduino Pin 2 (RX/SoftwareSerial)
  - PZEM RX -> Arduino Pin 3 (TX/SoftwareSerial)

  - PZEM 5V -> Arduino 5V
  - PZEM GND -> Arduino GND
*/

#include <PZEM004Tv40_R4.h>

// Auto-detect board and use appropriate serial interface
#if defined(ARDUINO_UNOR4_WIFI) || defined(ARDUINO_UNOR4_MINIMA)
// Arduino UNO R4 - Use Hardware Serial1
PZEM004Tv40_R4 pzem(&Serial1);
#define BOARD_TYPE "Arduino UNO R4"
#else
// Arduino UNO R3, Nano, etc. - Use SoftwareSerial
#include <SoftwareSerial.h>
SoftwareSerial pzemSerial(2, 3); // RX, TX
PZEM004Tv40_R4 pzem(&pzemSerial);
#define BOARD_TYPE "Arduino UNO R3/Nano"
#endif

void setup()
{
  Serial.begin(115200);

// Initialize PZEM serial
#if !defined(ARDUINO_UNOR4_WIFI) && !defined(ARDUINO_UNOR4_MINIMA)
  pzemSerial.begin(9600);
#endif

  pzem.begin();

  Serial.println("========================================");
  Serial.println("PZEM-004T Individual Parameters Demo");
  Serial.print("Board: ");
  Serial.println(BOARD_TYPE);
  Serial.println("========================================");
  Serial.println();
  delay(1000);
}

void loop()
{
  // ============================================================
  // METHOD 1: Read All Parameters at Once (RECOMMENDED)
  // ============================================================
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║  METHOD 1: Read All (Efficient)        ║");
  Serial.println("╚════════════════════════════════════════╝");

  unsigned long startTime = millis();

  if (pzem.readAll())
  {
    unsigned long elapsedTime = millis() - startTime;

    Serial.println("SUCCESS: Data read successfully!");
    Serial.println();
    Serial.println("Parameters:");
    Serial.println("-------------------------------------------");

    Serial.print("  Voltage:       ");
    Serial.print(pzem.getVoltage(), 1);
    Serial.println(" V");

    Serial.print("  Current:       ");
    Serial.print(pzem.getCurrent(), 3);
    Serial.println(" A");

    Serial.print("  Power:         ");
    Serial.print(pzem.getPower(), 1);
    Serial.println(" W");

    Serial.print("  Energy:        ");
    Serial.print(pzem.getEnergy(), 3);
    Serial.println(" kWh");

    Serial.print("  Frequency:     ");
    Serial.print(pzem.getFrequency(), 1);
    Serial.println(" Hz");

    Serial.print("  Power Factor:  ");
    Serial.println(pzem.getPowerFactor(), 2);

    Serial.print("  Alarm Status:  ");
    Serial.println(pzem.getAlarm() ? "ALARM!" : "Normal");

    Serial.println("-------------------------------------------");
    Serial.print("Execution Time: ");
    Serial.print(elapsedTime);
    Serial.println(" ms");
    Serial.println();
  }
  else
  {
    Serial.println("ERROR: Failed to read data!");
    Serial.print("Error Code: ");
    Serial.println(pzem.getLastError());
    Serial.println();
  }

  delay(2000);

  // ============================================================
  // METHOD 2: Read Individual Parameters (Less Efficient)
  // ============================================================
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║  METHOD 2: Individual Reads            ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println("Note: Each parameter requires a separate");
  Serial.println("      Modbus request - slower overall");
  Serial.println();

  startTime = millis();

  // Read Voltage
  Serial.print("Reading Voltage... ");
  float voltage = pzem.readVoltage();
  if (voltage >= 0)
  {
    Serial.print("OK: ");
    Serial.print(voltage, 1);
    Serial.println(" V");
  }
  else
  {
    Serial.println("ERROR: Failed");
  }
  delay(100);

  // Read Current
  Serial.print("Reading Current... ");
  float current = pzem.readCurrent();
  if (current >= 0)
  {
    Serial.print("OK: ");
    Serial.print(current, 3);
    Serial.println(" A");
  }
  else
  {
    Serial.println("ERROR: Failed");
  }
  delay(100);

  // Read Power
  Serial.print("Reading Power... ");
  float power = pzem.readPower();
  if (power >= 0)
  {
    Serial.print("OK: ");
    Serial.print(power, 1);
    Serial.println(" W");
  }
  else
  {
    Serial.println("ERROR: Failed");
  }
  delay(100);

  // Read Energy
  Serial.print("Reading Energy... ");
  float energy = pzem.readEnergy();
  if (energy >= 0)
  {
    Serial.print("OK: ");
    Serial.print(energy, 3);
    Serial.println(" kWh");
  }
  else
  {
    Serial.println("ERROR: Failed");
  }
  delay(100);

  // Read Frequency
  Serial.print("Reading Frequency... ");
  float frequency = pzem.readFrequency();
  if (frequency >= 0)
  {
    Serial.print("OK: ");
    Serial.print(frequency, 1);
    Serial.println(" Hz");
  }
  else
  {
    Serial.println("ERROR: Failed");
  }
  delay(100);

  // Read Power Factor
  Serial.print("Reading Power Factor... ");
  float powerFactor = pzem.readPowerFactor();
  if (powerFactor >= 0)
  {
    Serial.print("OK: ");
    Serial.println(powerFactor, 2);
  }
  else
  {
    Serial.println("ERROR: Failed");
  }

  unsigned long elapsedTime = millis() - startTime;
  Serial.println("-------------------------------------------");
  Serial.print("Execution Time: ");
  Serial.print(elapsedTime);
  Serial.println(" ms");
  Serial.println();

  // ============================================================
  // COMPARISON
  // ============================================================
  Serial.println("╔════════════════════════════════════════╗");
  Serial.println("║  RECOMMENDATION                        ║");
  Serial.println("╚════════════════════════════════════════╝");
  Serial.println("Use readAll() method for:");
  Serial.println("  • Faster execution (1 request vs 6+)");
  Serial.println("  • Lower bus traffic");
  Serial.println("  • Synchronized readings");
  Serial.println();
  Serial.println("Use individual reads only when:");
  Serial.println("  • You need just ONE specific parameter");
  Serial.println("  • Memory is extremely limited");
  Serial.println("  • Reading at different intervals");
  Serial.println();

  Serial.println("========================================");
  Serial.println();

  delay(3000);
}