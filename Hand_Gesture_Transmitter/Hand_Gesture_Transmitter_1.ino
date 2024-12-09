#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"

#define CE_PIN 9
#define CSN_PIN 8

Adafruit_MPU6050 mpu;

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);
//address through which the RF modules communicate.
const byte address[6] = "00001";

const int elbow_sw = 6;
const int gripper_ctrl = A0;
bool elbow = false;

void setup() {
  Serial.begin(115200);

  // Try to initialize the MPU6050 sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  //MPU6050 sensor Config
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // initialize the RF transceiver on the SPI bus
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  Serial.println(F("Found RF Module!"));
  radio.setPALevel(RF24_PA_LOW);
  //set the radio address
  radio.openWritingPipe(address);
  //Set module as transmitter
  radio.stopListening();

  pinMode(elbow_sw, INPUT_PULLUP);

}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float X = a.acceleration.x;
  float Y = a.acceleration.y;
  float Z = a.acceleration.z;

  int gripper = digitalRead(gripper_ctrl);
  // Serial.println(gripper);

  elbow = (digitalRead(elbow_sw) == LOW)? true : false;

  /* Print out the values */
  // Serial.print("Acceleration X: ");
  // Serial.print(X);
  // Serial.print(", Y: ");
  // Serial.print(Y);
  // Serial.print(", Z: ");
  // Serial.print(Z);
  // Serial.println(" m/s^2");

  // String data;
  char payload[5];
  bool report = false;

  if (X>=2 && X<=10) {
    payload[0] = 'R';
    payload[1] = '0' + map(X, 2, 10, 1, 9);
    // payload[1] = (X>=9.5)? '9' : '0'+round(X);
    // int index = round(X);
    // data = 'R' + String(index);
    // data.toCharArray(payload, sizeof(payload));
    // Serial.println(payload);
    // report = radio.write(&payload, sizeof(payload));
  }  else if (X<=-2 && X>=-10) {
    payload[0] = 'L';
    payload[1] = '0' + map(X, -2, -10, 1, 9);
    // payload[1] = (X<=-9.5)? '9' : '0'+static_cast<int>(-round(X));
    // int index = static_cast<int>(-round(X));
    // data = 'L' + String(index);
    // data.toCharArray(payload, sizeof(payload));
    // Serial.println(payload);
  } else {
    payload[0] = 'M';
    payload[1] = '0';
  }

  if (Y>=2 && Y<=10){
    payload[2] = (elbow)? 'U':'F';
    payload[3] = '0' + map(Y, 2, 10, 1, 9);
    // payload[3] = (Y>=9.5)? '9' : '0'+round(Y);
  
  } else if (Y<=-2 && Y>=-10) {
    payload[2] = (elbow)? 'D':'B';
    payload[3] = '0' + map(Y, -2, -10, 1, 9);
    // payload[3] = (Y<=-9.5)? '9' : '0'+static_cast<int>(-round(Y));
  } else {
    payload[2] = 'M';
    payload[3] = '0';
  }

  if (gripper == 0){
    payload[4] = 'C';
  } else {
    payload[4] = 'O';
  }

  report = radio.write(&payload, sizeof(payload));

  if (report)
    Serial.println(F("Transmission successful! "));
  else
    Serial.println(F("Transmission Failed! "));

  // Serial.println(payload);
 /// delay(1000);
}
