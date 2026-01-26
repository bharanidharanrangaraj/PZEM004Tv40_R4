/*!
 * @file PZEM004Tv40_R4.h
 * @brief Header file for PZEM-004T V4.0 Energy Monitor Library
 *
 * @author Bharani Dharan Rangaraj <bharanidharanrangaraj@gmail.com>
 * @date 2026-01-26
 * @version 1.1.0
 *
 * @copyright Copyright (c) 2026 Bharani Dharan Rangaraj
 *
 * @license MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @details
 * This library provides an interface to communicate with the PZEM-004T V4.0
 * energy monitor module using the Modbus RTU protocol. It is compatible with
 * both Arduino UNO R3 (using SoftwareSerial) and Arduino UNO R4 (using HardwareSerial).
 *
 * Hardware Support:
 * - PZEM-004T V4.0 (100A version)
 * - Compatible with Arduino UNO R3, UNO R4 Minima, UNO R4 WiFi, Nano, Mega
 *
 * Measurements Supported:
 * - Voltage: 80-260V AC (±0.5% accuracy)
 * - Current: 0-100A (±0.5% accuracy)
 * - Active Power: 0-23kW (±0.5% accuracy)
 * - Energy: 0-9999.99kWh (±0.5% accuracy)
 * - Frequency: 45-65Hz (±0.1Hz accuracy)
 * - Power Factor: 0.00-1.00 (±1% accuracy)
 * - Alarm Status: Over-power alarm
 *
 * Features:
 * - Efficient batch reading with readAll() method
 * - Individual parameter reading methods
 * - Energy counter reset functionality
 * - Dynamic address configuration
 * - Comprehensive error handling with detailed error codes
 * - CRC16 verification for data integrity
 * - Modbus RTU protocol implementation
 * - Universal compatibility (HardwareSerial and SoftwareSerial)
 *
 * @note This library uses serial communication at 9600 baud rate.
 * @note Default Modbus address is 0x01 (can be changed using setAddress())
 * @note For UNO R3/Nano: Use SoftwareSerial on pins 2 (RX) and 3 (TX)
 * @note For UNO R4: Use HardwareSerial (Serial1)
 *
 * @see https://github.com/bharanidharanrangaraj/PZEM004Tv40_R4
 *
 * @example Arduino UNO R4
 * @code
 * #include <PZEM004Tv40_R4.h>
 *
 * PZEM004Tv40_R4 pzem(&Serial1);
 *
 * void setup() {
 *   Serial.begin(115200);
 *   pzem.begin();
 * }
 *
 * void loop() {
 *   if (pzem.readAll()) {
 *     Serial.print("Voltage: ");
 *     Serial.print(pzem.getVoltage());
 *     Serial.println(" V");
 *   }
 *   delay(1000);
 * }
 * @endcode
 *
 * @example Arduino UNO R3
 * @code
 * #include <SoftwareSerial.h>
 * #include <PZEM004Tv40_R4.h>
 *
 * SoftwareSerial pzemSerial(2, 3); // RX, TX
 * PZEM004Tv40_R4 pzem(&pzemSerial);
 *
 * void setup() {
 *   Serial.begin(115200);
 *   pzemSerial.begin(9600);
 *   pzem.begin();
 * }
 *
 * void loop() {
 *   if (pzem.readAll()) {
 *     Serial.print("Voltage: ");
 *     Serial.print(pzem.getVoltage());
 *     Serial.println(" V");
 *   }
 *   delay(1000);
 * }
 * @endcode
 */

#ifndef PZEM004TV40_R4_H
#define PZEM004TV40_R4_H

#include "Arduino.h"

// Support both HardwareSerial and SoftwareSerial
#if defined(__AVR__)
#include <SoftwareSerial.h>
#endif

// Default PZEM Modbus address
#define PZEM_DEFAULT_ADDR 0x01

// Modbus function codes
#define PZEM_READ_INPUT_REG 0x04

// Register addresses
#define REG_VOLTAGE 0x0000
#define REG_CURRENT_L 0x0001
#define REG_CURRENT_H 0x0002
#define REG_POWER_L 0x0003
#define REG_POWER_H 0x0004
#define REG_ENERGY_L 0x0005
#define REG_ENERGY_H 0x0006
#define REG_FREQUENCY 0x0007
#define REG_POWER_FACTOR 0x0008
#define REG_ALARM 0x0009

// Response timeout
#define PZEM_TIMEOUT 200

class PZEM004Tv40_R4
{
public:
// Constructor - accepts both HardwareSerial and SoftwareSerial
#if defined(__AVR__)
  // For AVR boards (UNO R3, Nano, Mega) - SoftwareSerial
  PZEM004Tv40_R4(SoftwareSerial *port, uint8_t addr = PZEM_DEFAULT_ADDR);
#endif
  // For all boards including R4 - HardwareSerial
  PZEM004Tv40_R4(HardwareSerial *port, uint8_t addr = PZEM_DEFAULT_ADDR);

  // Initialize
  void begin();

  // Read all parameters at once (most efficient)
  bool readAll();

  // Get individual values (must call readAll() first)
  float getVoltage();
  float getCurrent();
  float getPower();
  float getEnergy();
  float getFrequency();
  float getPowerFactor();
  bool getAlarm();

  // Read individual parameters (less efficient, makes separate requests)
  float readVoltage();
  float readCurrent();
  float readPower();
  float readEnergy();
  float readFrequency();
  float readPowerFactor();

  // Reset energy counter
  bool resetEnergy();

  // Change device address (use with caution!)
  bool setAddress(uint8_t newAddr);

  // Get last error
  uint8_t getLastError();

  // Error codes
  enum ErrorCode
  {
    ERROR_NONE = 0,
    ERROR_TIMEOUT,
    ERROR_CRC,
    ERROR_INVALID_RESPONSE,
    ERROR_NO_DATA
  };

private:
  Stream *_serial;          // Changed to Stream* for universal compatibility
  HardwareSerial *_hwSerial; // Store HardwareSerial pointer if used (for begin())
  uint8_t _addr;
  uint8_t _lastError;

  // Cached values from last readAll()
  float _voltage;
  float _current;
  float _power;
  float _energy;
  float _frequency;
  float _powerFactor;
  bool _alarm;

  // Internal methods
  bool sendRequest(uint16_t reg, uint16_t count);
  bool receiveResponse(uint8_t *buffer, uint16_t length);
  uint16_t calculateCRC(uint8_t *data, uint16_t length);
  bool verifyCRC(uint8_t *data, uint16_t length);
};

#endif