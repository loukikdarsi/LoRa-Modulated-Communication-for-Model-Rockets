# Long Range (LoRa) Modulated Communication for Model Rockets

## Project Overview
The primary objective of this project is to develop a communication system capable of transmitting and receiving data over long distances while minimizing interference and signal attenuation.

## What is LoRa?
LoRa modulation, short for Long Range modulation, is a wireless communication technology designed for long-distance data transmission with low power consumption. It uses a unique chirp spread spectrum technique, which spreads the signal over a wide bandwidth. This approach enables robust communication in challenging environments, allowing for improved range and penetration through obstacles.

LoRa is particularly well-suited for applications in the Internet of Things (IoT), where devices often require long-range connectivity while maintaining battery efficiency. Its ability to support multiple devices and transmit small amounts of data over long distances makes it ideal for smart city applications, agricultural monitoring, and various sensor networks.

## Required Software
- Arduino IDE
- RF_Setting from EBYTE
- Hercules

## Components Used
- EBYTE ES32900T30D 868MHz 1W LoRa Module — X2
- Arduino Nano Every / Nano RP2040 Connect
- Adafruit BNO055 9-DoF IMU
- Adafruit BMP280 Pressure Sensor
- Adafruit BME680 Environmental Sensor
- Ublox Neo M8N GPS Module
- W25QXX Serial NOR Flash Memory
- SD Card Reader Module
- SD Card
- USB to UART Adapter
- Other Consumables (Jumper Wires, Breadboard, LEDs, Resistors)

## Circuit Diagram
This is the representation of the ideal circuit diagram for a model rocket’s communication system.

**Note:** A few changes have been made in the actual implementation due to competition constraints:
- The circuit diagram contains a XTSD02G High-Capacity Non-Volatile NAND Flash which has been replaced by an SD Card reader for simplicity reasons. All data transmitted is also stored on the SD Card as a backup.
- The XBEE Radio Module has been replaced by the EBYTE LoRa Module to improve communication range and quality.

### Microcontroller – LoRa Module [EBYTE ES32900T30D 868MHz] Connections:
**For configuring the modules (In Sleep Mode):**
- 5V – 5V
- GND – GND
- TX – RX
- RX – TX
- M0 – 5V
- M1 – 5V

**For communication (In Wake-Up [Transmission] Mode):**
- 5V – 5V
- GND – GND
- TX – RX
- RX – TX
- M0 – GND
- M1 – GND

*For PC to PC connections, connecting the LoRa modules to the computer via a USB – UART Connector is recommended.*

## Implementation
[Repository](https://github.com/loukikdarsi/LoRa-Modulated-Communication-for-Model-Rockets)

## References
- [CDEBYTE E220-900T30D](https://www.cdebyte.com/products/E220-900T30D)
- [CDEBYTE E220-900T30D Downloads](https://www.cdebyte.com/products/E220-900T30D/4#Downloads)
- [Hercules Setup Utility](https://www.hw-group.com/software/hercules-setup-utility)
