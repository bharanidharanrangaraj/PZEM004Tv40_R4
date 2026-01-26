/*!
 * @file PZEM004Tv40_R4.cpp
 * @brief Implementation file for PZEM-004T V4.0 Energy Monitor Library
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
 * Features:
 * - Read voltage, current, power, energy, frequency, power factor
 * - Check alarm status
 * - Reset energy counter
 * - Change device address
 * - Built-in error handling with detailed error codes
 * - CRC16 verification for reliable communication
 * - Support for both individual and batch parameter reading
 * - Universal compatibility (works on UNO R3, R4, Nano, Mega, etc.)
 *
 * @see https://github.com/bharanidharanrangaraj/PZEM004Tv40_R4
 */

#include "PZEM004Tv40_R4.h"

// Constructor for AVR boards (UNO R3, Nano, Mega) using SoftwareSerial
#if defined(__AVR__)
PZEM004Tv40_R4::PZEM004Tv40_R4(SoftwareSerial *port, uint8_t addr)
{
  _serial = port;
  _hwSerial = nullptr; // SoftwareSerial - user must call begin() on it before passing
  _addr = addr;
  _lastError = ERROR_NONE;

  // Initialize cached values
  _voltage = 0.0;
  _current = 0.0;
  _power = 0.0;
  _energy = 0.0;
  _frequency = 0.0;
  _powerFactor = 0.0;
  _alarm = false;
}
#endif

// Constructor for all boards including R4 using HardwareSerial
PZEM004Tv40_R4::PZEM004Tv40_R4(HardwareSerial *port, uint8_t addr)
{
  _serial = port;
  _hwSerial = port; // Store HardwareSerial pointer for begin()
  _addr = addr;
  _lastError = ERROR_NONE;

  // Initialize cached values
  _voltage = 0.0;
  _current = 0.0;
  _power = 0.0;
  _energy = 0.0;
  _frequency = 0.0;
  _powerFactor = 0.0;
  _alarm = false;
}

void PZEM004Tv40_R4::begin()
{
  // Only call begin() if we have a HardwareSerial pointer
  // For SoftwareSerial, user must call begin() on it before passing to constructor
  if (_hwSerial != nullptr)
  {
    _hwSerial->begin(9600);
  }
  delay(100);
}

bool PZEM004Tv40_R4::readAll()
{
  // Read all 10 registers in one request
  if (!sendRequest(REG_VOLTAGE, 10))
    return false;

  uint8_t buffer[25];
  if (!receiveResponse(buffer, 25))
    return false;

  // Verify we got the right amount of data
  if (buffer[2] != 20) // Should be 20 bytes of data (10 registers × 2)
  {
    _lastError = ERROR_INVALID_RESPONSE;
    return false;
  }

  // Parse all values
  uint16_t vRaw = (buffer[3] << 8) | buffer[4];
  _voltage = vRaw / 10.0;

  uint32_t iLow = (buffer[5] << 8) | buffer[6];
  uint32_t iHigh = (buffer[7] << 8) | buffer[8];
  uint32_t iRaw = (iHigh << 16) | iLow;
  _current = iRaw / 1000.0;

  uint32_t pLow = (buffer[9] << 8) | buffer[10];
  uint32_t pHigh = (buffer[11] << 8) | buffer[12];
  uint32_t pRaw = (pHigh << 16) | pLow;
  _power = pRaw / 10.0;

  uint32_t eLow = (buffer[13] << 8) | buffer[14];
  uint32_t eHigh = (buffer[15] << 8) | buffer[16];
  uint32_t eRaw = (eHigh << 16) | eLow;
  _energy = eRaw / 1000.0;

  uint16_t fRaw = (buffer[17] << 8) | buffer[18];
  _frequency = fRaw / 10.0;

  uint16_t pfRaw = (buffer[19] << 8) | buffer[20];
  _powerFactor = pfRaw / 100.0;

  uint16_t alarm = (buffer[21] << 8) | buffer[22];
  _alarm = (alarm == 0xFFFF);

  _lastError = ERROR_NONE;
  return true;
}

float PZEM004Tv40_R4::getVoltage()
{
  return _voltage;
}

float PZEM004Tv40_R4::getCurrent()
{
  return _current;
}

float PZEM004Tv40_R4::getPower()
{
  return _power;
}

float PZEM004Tv40_R4::getEnergy()
{
  return _energy;
}

float PZEM004Tv40_R4::getFrequency()
{
  return _frequency;
}

float PZEM004Tv40_R4::getPowerFactor()
{
  return _powerFactor;
}

bool PZEM004Tv40_R4::getAlarm()
{
  return _alarm;
}

float PZEM004Tv40_R4::readVoltage()
{
  if (!sendRequest(REG_VOLTAGE, 1))
    return -1.0;

  uint8_t buffer[7];
  if (!receiveResponse(buffer, 7))
    return -1.0;

  uint16_t raw = (buffer[3] << 8) | buffer[4];
  return raw / 10.0;
}

float PZEM004Tv40_R4::readCurrent()
{
  if (!sendRequest(REG_CURRENT_L, 2))
    return -1.0;

  uint8_t buffer[9];
  if (!receiveResponse(buffer, 9))
    return -1.0;

  uint32_t low = (buffer[3] << 8) | buffer[4];
  uint32_t high = (buffer[5] << 8) | buffer[6];
  uint32_t raw = (high << 16) | low;
  return raw / 1000.0;
}

float PZEM004Tv40_R4::readPower()
{
  if (!sendRequest(REG_POWER_L, 2))
    return -1.0;

  uint8_t buffer[9];
  if (!receiveResponse(buffer, 9))
    return -1.0;

  uint32_t low = (buffer[3] << 8) | buffer[4];
  uint32_t high = (buffer[5] << 8) | buffer[6];
  uint32_t raw = (high << 16) | low;
  return raw / 10.0;
}

float PZEM004Tv40_R4::readEnergy()
{
  if (!sendRequest(REG_ENERGY_L, 2))
    return -1.0;

  uint8_t buffer[9];
  if (!receiveResponse(buffer, 9))
    return -1.0;

  uint32_t low = (buffer[3] << 8) | buffer[4];
  uint32_t high = (buffer[5] << 8) | buffer[6];
  uint32_t raw = (high << 16) | low;
  return raw / 1000.0;
}

float PZEM004Tv40_R4::readFrequency()
{
  if (!sendRequest(REG_FREQUENCY, 1))
    return -1.0;

  uint8_t buffer[7];
  if (!receiveResponse(buffer, 7))
    return -1.0;

  uint16_t raw = (buffer[3] << 8) | buffer[4];
  return raw / 10.0;
}

float PZEM004Tv40_R4::readPowerFactor()
{
  if (!sendRequest(REG_POWER_FACTOR, 1))
    return -1.0;

  uint8_t buffer[7];
  if (!receiveResponse(buffer, 7))
    return -1.0;

  uint16_t raw = (buffer[3] << 8) | buffer[4];
  return raw / 100.0;
}

bool PZEM004Tv40_R4::resetEnergy()
{
  // Clear any pending data
  while (_serial->available())
    _serial->read();

  delay(50);

  // PZEM Reset Energy Command
  // Format: [ADDR][CMD][0x00][0x00][CRC_L][CRC_H]
  uint8_t resetCmd[6];
  resetCmd[0] = _addr;
  resetCmd[1] = 0x42; // Reset energy command
  resetCmd[2] = 0x00;
  resetCmd[3] = 0x00;

  // Calculate CRC16 (Modbus)
  uint16_t crc = calculateCRC(resetCmd, 4);
  resetCmd[4] = crc & 0xFF;        // Low byte first
  resetCmd[5] = (crc >> 8) & 0xFF; // High byte

  // Send command
  _serial->write(resetCmd, 6);

// For HardwareSerial, use flush()
#if !defined(__AVR__)
  _serial->flush();
#endif

  // Wait for PZEM to process
  delay(100);

  // Read response
  uint8_t buffer[10]; // Larger buffer to catch any response
  unsigned long startTime = millis();
  uint16_t bytesRead = 0;

  while ((millis() - startTime) < 500) // Longer timeout for reset
  {
    if (_serial->available())
    {
      if (bytesRead < 10)
      {
        buffer[bytesRead++] = _serial->read();
      }
    }
  }

  // Check response
  if (bytesRead == 0)
  {
    _lastError = ERROR_TIMEOUT;
    return false;
  }

  // Minimum valid response is 4 bytes: [ADDR][CMD][CRC_L][CRC_H]
  if (bytesRead >= 4)
  {
    // Check if address and command match
    if (buffer[0] == _addr && buffer[1] == 0x42)
    {
      // Success - don't verify CRC for reset response as some PZEM versions
      // don't send proper CRC on reset acknowledgment
      _lastError = ERROR_NONE;
      return true;
    }
  }

  _lastError = ERROR_INVALID_RESPONSE;
  return false;
}

bool PZEM004Tv40_R4::setAddress(uint8_t newAddr)
{
  if (newAddr < 0x01 || newAddr > 0xF7)
    return false;

  uint8_t cmd[] = {_addr, 0x41, newAddr, 0x00, 0x00, 0x00};

  // Calculate CRC
  uint16_t crc = calculateCRC(cmd, 4);
  cmd[4] = crc & 0xFF;
  cmd[5] = (crc >> 8) & 0xFF;

  // Clear buffer
  while (_serial->available())
    _serial->read();

  // Send command
  _serial->write(cmd, 6);
  delay(100);

  // Check for response
  uint8_t buffer[5];
  if (!receiveResponse(buffer, 5))
    return false;

  if (buffer[0] == newAddr && buffer[1] == 0x41)
  {
    _addr = newAddr;
    return true;
  }

  return false;
}

uint8_t PZEM004Tv40_R4::getLastError()
{
  return _lastError;
}

bool PZEM004Tv40_R4::sendRequest(uint16_t reg, uint16_t count)
{
  uint8_t request[8];

  request[0] = _addr;
  request[1] = PZEM_READ_INPUT_REG;
  request[2] = (reg >> 8) & 0xFF;
  request[3] = reg & 0xFF;
  request[4] = (count >> 8) & 0xFF;
  request[5] = count & 0xFF;

  uint16_t crc = calculateCRC(request, 6);
  request[6] = crc & 0xFF;
  request[7] = (crc >> 8) & 0xFF;

  // Clear any old data
  while (_serial->available())
    _serial->read();

  // Send request
  _serial->write(request, 8);

  return true;
}

bool PZEM004Tv40_R4::receiveResponse(uint8_t *buffer, uint16_t length)
{
  unsigned long startTime = millis();
  uint16_t index = 0;

  while (index < length)
  {
    if (_serial->available())
    {
      buffer[index++] = _serial->read();
    }

    if (millis() - startTime > PZEM_TIMEOUT)
    {
      _lastError = ERROR_TIMEOUT;
      return false;
    }
  }

  // Verify CRC
  if (!verifyCRC(buffer, length))
  {
    _lastError = ERROR_CRC;
    return false;
  }

  // Verify address and function code
  if (buffer[0] != _addr || buffer[1] != PZEM_READ_INPUT_REG)
  {
    _lastError = ERROR_INVALID_RESPONSE;
    return false;
  }

  _lastError = ERROR_NONE;
  return true;
}

uint16_t PZEM004Tv40_R4::calculateCRC(uint8_t *data, uint16_t length)
{
  uint16_t crc = 0xFFFF;

  for (uint16_t i = 0; i < length; i++)
  {
    crc ^= (uint16_t)data[i];

    for (uint8_t j = 0; j < 8; j++)
    {
      if (crc & 0x0001)
      {
        crc >>= 1;
        crc ^= 0xA001;
      }
      else
      {
        crc >>= 1;
      }
    }
  }

  return crc;
}

bool PZEM004Tv40_R4::verifyCRC(uint8_t *data, uint16_t length)
{
  uint16_t receivedCRC = data[length - 2] | (data[length - 1] << 8);
  uint16_t calculatedCRC = calculateCRC(data, length - 2);

  return (receivedCRC == calculatedCRC);
}