#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BME680.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include <ArduinoJson.h>

// Initialize sensors
Adafruit_BNO055 bno = Adafruit_BNO055(55);
Adafruit_BMP280 bmp;
Adafruit_BME680 bme;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(4, 3); // GPS RX, TX pins

// SD Card setup
const int chipSelect = 10; // Pin for CS on the SD card module
unsigned int packageID = 1;
File dataFile;

// LoRa communication setup
SoftwareSerial loraSerial(2, 3); // TX, RX for LoRa module

// Global variable to hold JSON string data
String jsonData;

// Function to capture data from sensors
void captureSensorData() {
  StaticJsonDocument<512> doc;
  doc["packageID"] = packageID++;

  // BNO055 data (orientation and acceleration)
  sensors_event_t event;
  bno.getEvent(&event);
  imu::Vector<3> acc = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  JsonObject bnoData = doc.createNestedObject("BNO055");
  bnoData["orientation_x"] = event.orientation.x;
  bnoData["orientation_y"] = event.orientation.y;
  bnoData["orientation_z"] = event.orientation.z;
  bnoData["acceleration_x"] = acc.x();
  bnoData["acceleration_y"] = acc.y();
  bnoData["acceleration_z"] = acc.z();

  // BMP280 data (temperature and pressure)
  float temperatureBMP = bmp.readTemperature();
  float pressureBMP = bmp.readPressure() / 100.0F;
  JsonObject bmpData = doc.createNestedObject("BMP280");
  bmpData["temperature"] = temperatureBMP;
  bmpData["pressure"] = pressureBMP;

  // BME680 data (temperature, humidity, pressure, and gas resistance)
  if (bme.performReading()) {
    JsonObject bmeData = doc.createNestedObject("BME680");
    bmeData["temperature"] = bme.temperature;
    bmeData["humidity"] = bme.humidity;
    bmeData["pressure"] = bme.pressure / 100.0;
    bmeData["gas_resistance"] = bme.gas_resistance / 1000.0;
  } else {
    Serial.println("Failed to read BME680 data.");
  }

  // GPS data (latitude, longitude, altitude, speed)
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  float latitude = gps.location.isValid() ? gps.location.lat() : 0.0;
  float longitude = gps.location.isValid() ? gps.location.lng() : 0.0;
  float altitude = gps.altitude.isValid() ? gps.altitude.meters() : 0.0;
  float speed = gps.speed.isValid() ? gps.speed.kmph() : 0.0;
  JsonObject gpsData = doc.createNestedObject("GPS");
  gpsData["latitude"] = latitude;
  gpsData["longitude"] = longitude;
  gpsData["altitude"] = altitude;
  gpsData["speed"] = speed;

  // Convert JSON to string and store in global jsonData
  serializeJson(doc, jsonData);
  Serial.println("Captured Sensor Data:");
  Serial.println(jsonData);
}

// Function to save data to SD card and transmit over LoRa
void saveDataAndTransmit() {
  // Save JSON data to SD card
  dataFile = SD.open("sensorData.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(jsonData);
    dataFile.close();
    Serial.println("Data stored on SD card.");
  } else {
    Serial.println("Error opening sensorData.txt");
  }

  // Transmit data over LoRa
  if (loraSerial.availableForWrite()) {
    loraSerial.println(jsonData);
    Serial.println("Data transmitted via LoRa.");
  } else {
    Serial.println("LoRa transmission failed.");
  }
}

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);
  loraSerial.begin(9600); // Initialize LoRa module

  // Initialize sensors
  if (!bno.begin()) {
    Serial.println("BNO055 not detected.");
    while (1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 not detected.");
    while (1);
  }

  if (!bme.begin()) {
    Serial.println("BME680 not detected.");
    while (1);
  }
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150);

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
}

void loop() {
  // Step 1: Capture sensor data
  captureSensorData();
  delay(500);  // Short delay to simulate thread timing

  // Step 2: Save data to SD and transmit over LoRa
  saveDataAndTransmit();
  delay(500);  // Short delay before next capture
}
