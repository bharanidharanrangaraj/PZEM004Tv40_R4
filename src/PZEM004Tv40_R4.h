/*
  PZEM004Tv40_R4.h - Library for PZEM-004T V4.0 Energy Monitor
  Compatible with Arduino UNO R4 (Minima/WiFi)
  
  Created by [Your Name], 2024
  Released under MIT License
  
  Hardware Support: PZEM-004T V4.0 (100A version)
  
  Features:
  - Voltage measurement (80-260V AC)
  - Current measurement (0-100A)
  - Power measurement
  - Energy measurement
  - Frequency measurement
  - Power factor measurement
  - Alarm status
*/

#ifndef PZEM004TV40_R4_H
#define PZEM004TV40_R4_H

#include "Arduino.h"

// Default PZEM Modbus address
#define PZEM_DEFAULT_ADDR 0x01

// Modbus function codes
#define PZEM_READ_INPUT_REG 0x04

// Register addresses
#define REG_VOLTAGE       0x0000
#define REG_CURRENT_L     0x0001
#define REG_CURRENT_H     0x0002
#define REG_POWER_L       0x0003
#define REG_POWER_H       0x0004
#define REG_ENERGY_L      0x0005
#define REG_ENERGY_H      0x0006
#define REG_FREQUENCY     0x0007
#define REG_POWER_FACTOR  0x0008
#define REG_ALARM         0x0009

// Response timeout
#define PZEM_TIMEOUT 200

class PZEM004Tv40_R4
{
  public:
    // Constructor
    PZEM004Tv40_R4(HardwareSerial* port, uint8_t addr = PZEM_DEFAULT_ADDR);
    
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
    enum ErrorCode {
      ERROR_NONE = 0,
      ERROR_TIMEOUT,
      ERROR_CRC,
      ERROR_INVALID_RESPONSE,
      ERROR_NO_DATA
    };
    
  private:
    HardwareSerial* _serial;
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
    bool receiveResponse(uint8_t* buffer, uint16_t length);
    uint16_t calculateCRC(uint8_t* data, uint16_t length);
    bool verifyCRC(uint8_t* data, uint16_t length);
};

#endif