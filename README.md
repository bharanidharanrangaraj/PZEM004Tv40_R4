# PZEM004Tv40_R4

[![Arduino](https://img.shields.io/badge/Arduino-UNO%20R4-00979D?logo=arduino)](https://www.arduino.cc/)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/bharanidharanrangaraj/library/PZEM004Tv40_R4.svg)](https://registry.platformio.org/libraries/bharanidharanrangaraj/PZEM004Tv40_R4)
[![Version](https://img.shields.io/badge/version-1.1.0-blue.svg)](https://github.com/bharanidharanrangaraj/PZEM004Tv40_R4)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Arduino library for the **PZEM-004T V4.0 (100A)** Energy Monitor, compatible with **Arduino UNO R3, R4, Nano, Mega**, and other Arduino boards. This library enables easy reading of voltage, current, power, energy, frequency, power factor, and alarm status using the Modbus RTU protocol.

## Features

- **Full Parameter Support**: Voltage, Current, Power, Energy, Frequency, Power Factor, Alarm Status
- **Efficient Data Reading**: Read all parameters with a single Modbus request
- **Individual Parameter Access**: Option to read specific parameters separately
- **Energy Reset**: Reset accumulated energy counter
- **Address Management**: Change device Modbus address for multi-device setups
- **Built-in Error Handling**: CRC verification and comprehensive error codes
- **Universal Compatibility**: Works with Arduino UNO R3, R4, Nano, Mega, and other boards
- **Flexible Serial Communication**: Supports both HardwareSerial (R4) and SoftwareSerial (R3/Nano)

## Hardware Specifications

**Supported Device**: PZEM-004T-100A (V4.0)

| Parameter | Measurement Range | Resolution |
|-----------|------------------|------------|
| Voltage | 80-260V AC | 0.1V |
| Current | 0-100A | 0.001A |
| Power | 0-23kW | 0.1W |
| Energy | 0-9999.99kWh | 1Wh |
| Frequency | 45-65Hz | 0.1Hz |
| Power Factor | 0.00-1.00 | 0.01 |

**Communication**: Modbus RTU, 9600 baud, 8N1

## Installation

### Arduino IDE

1. Download this repository as a ZIP file
2. In Arduino IDE, go to **Sketch** → **Include Library** → **Add .ZIP Library**
3. Select the downloaded ZIP file
4. Restart Arduino IDE

### PlatformIO

Add to your `platformio.ini`:

```ini
lib_deps = 
    https://github.com/bharanidharanrangaraj/PZEM004Tv40_R4
```

### Manual Installation

1. Clone or download this repository
2. Copy the `PZEM004Tv40_R4` folder to your Arduino libraries directory:
   - **Windows**: `Documents\Arduino\libraries\`
   - **MacOS**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`

## Wiring

### Arduino UNO R4 (Minima/WiFi)

Connect the PZEM-004T to Arduino UNO R4:

| PZEM-004T | Arduino UNO R4 |
|-----------|----------------|
| TX | Pin 0 (RX / Serial1) |
| RX | Pin 1 (TX / Serial1) |
| 5V | 5V |
| GND | GND |

> **Note**: The PZEM-004T uses **Serial1** on Arduino UNO R4, leaving the main USB serial (Serial) free for debugging.

### Arduino UNO R3 / Nano / Mega

Connect the PZEM-004T to Arduino UNO R3/Nano:

| PZEM-004T | Arduino UNO R3/Nano |
|-----------|--------------------|
| TX | Pin 2 (RX / SoftwareSerial) |
| RX | Pin 3 (TX / SoftwareSerial) |
| 5V | 5V |
| GND | GND |

> **Note**: R3/Nano use **SoftwareSerial** for PZEM communication. You can change pins 2 and 3 to other digital pins if needed.

## Quick Start

### Arduino UNO R4

```cpp
#include <PZEM004Tv40_R4.h>

// Create PZEM object on Serial1
PZEM004Tv40_R4 pzem(&Serial1);

void setup() {
  Serial.begin(115200);
  pzem.begin();
}

void loop() {
  // Read all parameters at once (recommended)
  if (pzem.readAll()) {
    Serial.print("Voltage: ");
    Serial.print(pzem.getVoltage(), 1);
    Serial.println(" V");
    
    Serial.print("Current: ");
    Serial.print(pzem.getCurrent(), 3);
    Serial.println(" A");
    
    Serial.print("Power: ");
    Serial.print(pzem.getPower(), 1);
    Serial.println(" W");
    
    Serial.print("Energy: ");
    Serial.print(pzem.getEnergy(), 3);
    Serial.println(" kWh");
  } else {
    Serial.print("Error: ");
    Serial.println(pzem.getLastError());
  }
  
  delay(1000);
}
```

### Arduino UNO R3 / Nano

```cpp
#include <SoftwareSerial.h>
#include <PZEM004Tv40_R4.h>

// Create SoftwareSerial for PZEM (RX=Pin2, TX=Pin3)
SoftwareSerial pzemSerial(2, 3);

// Create PZEM object on SoftwareSerial
PZEM004Tv40_R4 pzem(&pzemSerial);

void setup() {
  Serial.begin(115200);
  pzemSerial.begin(9600);
  pzem.begin();
}

void loop() {
  // Read all parameters at once (recommended)
  if (pzem.readAll()) {
    Serial.print("Voltage: ");
    Serial.print(pzem.getVoltage(), 1);
    Serial.println(" V");
    
    Serial.print("Current: ");
    Serial.print(pzem.getCurrent(), 3);
    Serial.println(" A");
    
    Serial.print("Power: ");
    Serial.print(pzem.getPower(), 1);
    Serial.println(" W");
  } else {
    Serial.print("Error: ");
    Serial.println(pzem.getLastError());
  }
  
  delay(1000);
}
```

## API Reference

### Constructor

```cpp
PZEM004Tv40_R4(HardwareSerial* port, uint8_t addr = 0x01)
```

Creates a PZEM object with specified serial port and optional Modbus address.

**Parameters:**
- `port`: Pointer to hardware serial port (use `&Serial1` for UNO R4)
- `addr`: Modbus address (default: 0x01, range: 0x01-0xF7)

### Methods

#### Initialization

```cpp
void begin()
```

Initialize the serial communication. Must be called in `setup()`.

---

#### Read All Parameters (Recommended)

```cpp
bool readAll()
```

Reads all parameters from PZEM in a single Modbus transaction. Most efficient method.

**Returns:** `true` if successful, `false` on error

**Example:**
```cpp
if (pzem.readAll()) {
  // Access data using get methods
  float voltage = pzem.getVoltage();
  float current = pzem.getCurrent();
}
```

---

#### Get Cached Values

After calling `readAll()`, retrieve individual parameters:

```cpp
float getVoltage()       // Returns voltage in V
float getCurrent()       // Returns current in A
float getPower()         // Returns power in W
float getEnergy()        // Returns energy in kWh
float getFrequency()     // Returns frequency in Hz
float getPowerFactor()   // Returns power factor (0.00-1.00)
bool getAlarm()          // Returns alarm status
```

---

#### Read Individual Parameters

Read single parameters (less efficient, requires separate Modbus request):

```cpp
float readVoltage()      // Read and return voltage
float readCurrent()      // Read and return current
float readPower()        // Read and return power
float readEnergy()       // Read and return energy
float readFrequency()    // Read and return frequency
float readPowerFactor()  // Read and return power factor
```

**Returns:** Parameter value on success, `-1.0` on error

---

#### Energy Reset

```cpp
bool resetEnergy()
```

Resets the accumulated energy counter to zero.

**Returns:** `true` if successful, `false` on error

**Example:**
```cpp
if (pzem.resetEnergy()) {
  Serial.println("Energy counter reset!");
}
```

---

#### Change Address

```cpp
bool setAddress(uint8_t newAddr)
```

Changes the device Modbus address. Use with caution!

**Parameters:**
- `newAddr`: New address (range: 0x01-0xF7)

**Returns:** `true` if successful, `false` on error

> **Warning**: Only connect ONE PZEM device when changing addresses. Multiple devices with the same address will cause communication conflicts.

---

#### Error Handling

```cpp
uint8_t getLastError()
```

Returns the last error code.

**Error Codes:**
- `0`: No error
- `1`: Timeout - device did not respond
- `2`: CRC error - corrupted data
- `3`: Invalid response
- `4`: No data available

## Examples

The library includes four comprehensive examples:

### 1. Basic_Reading

Simple example demonstrating basic parameter reading and display.

```bash
File → Examples → PZEM004Tv40_R4 → Basic_Reading
```

### 2. Individual_Parameters

Compares efficient `readAll()` method vs individual parameter reads. Shows execution time comparison.

```bash
File → Examples → PZEM004Tv40_R4 → Individual_Parameters
```

### 3. Reset_Energy

Demonstrates automatic energy counter reset at specified intervals (daily, weekly, etc.) with cost calculation.

```bash
File → Examples → PZEM004Tv40_R4 → Reset_Energy
```

### 4. Change_Address

Utility for changing the device Modbus address with verification.

```bash
File → Examples → PZEM004Tv40_R4 → Change_Address
```

## Best Practices

### ✅ Do's

- **Use `readAll()`** for reading multiple parameters - it's much faster
- **Cache values** by calling `readAll()` once, then use `get` methods multiple times
- **Check return values** - always verify `readAll()` returns `true`
- **Add delays** between read operations (minimum 100ms recommended)
- **Power cycle PZEM** if communication issues persist

### ❌ Don'ts

- **Don't read individual parameters** frequently - use `readAll()` instead
- **Don't change addresses** with multiple devices connected
- **Don't exceed 10 requests/second** - PZEM needs time to process
- **Don't ignore error codes** - use `getLastError()` for debugging

## Troubleshooting

| Problem | Solution |
|---------|----------|
| No data received | Check wiring, ensure PZEM is powered, verify address |
| CRC errors | Check for loose connections, electromagnetic interference |
| Timeout errors | Increase delay between reads, check baud rate (9600) |
| Wrong readings | Ensure proper CT clamp installation, check phase wiring |
| Address change fails | Connect only ONE device, power cycle before retry |

## Performance

**Execution Times** (typical on Arduino UNO R4):

- `readAll()`: ~200ms (single Modbus request)
- Individual reads (6 parameters): ~1200ms (6 separate requests)

**Recommendation**: Use `readAll()` for 6x faster performance!

## Multi-Device Setup

To use multiple PZEM devices:

1. Connect devices one at a time
2. Change each device to a unique address (0x01, 0x02, 0x03...)
3. Create multiple PZEM objects with different addresses:

```cpp
PZEM004Tv40_R4 pzem1(&Serial1, 0x01);
PZEM004Tv40_R4 pzem2(&Serial1, 0x02);
PZEM004Tv40_R4 pzem3(&Serial1, 0x03);
```

## Compatibility

### Supported Boards

- ✅ **Arduino UNO R4 Minima** (HardwareSerial)
- ✅ **Arduino UNO R4 WiFi** (HardwareSerial)
- ✅ **Arduino UNO R3** (SoftwareSerial)
- ✅ **Arduino Nano** (SoftwareSerial)
- ✅ **Arduino Mega** (HardwareSerial)
- ✅ **Other Arduino boards** with HardwareSerial or SoftwareSerial support

### Not Supported

- ❌ ESP32/ESP8266 (Currently under development, support for these boards will be available in upcoming release)

### Architectures

- **AVR** (avr) - UNO R3, Nano, Mega (uses SoftwareSerial)
- **Renesas RA4M1** (renesas_uno) - UNO R4 (uses HardwareSerial)

## License

This library is released under the [MIT License](https://opensource.org/licenses/MIT).

## Author

**Bharani Dharan Rangaraj**
- Email: bharanidharanrangaraj@gmail.com
- GitHub: [@bharanidharanrangaraj](https://github.com/bharanidharanrangaraj)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Support

If you find this library helpful, please ⭐ star the repository!

For issues and feature requests, please use the [GitHub Issues](https://github.com/bharanidharanrangaraj/PZEM004Tv40_R4/issues) page.

---

**Version**: 1.1.0  
**Last Updated**: January 2026
