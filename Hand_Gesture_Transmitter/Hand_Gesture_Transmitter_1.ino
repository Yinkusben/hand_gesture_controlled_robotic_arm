#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"

//CE and CSN pins of the nrf24l01 module
#define CE_PIN 9
#define CSN_PIN 8

// A tact switch is connected to D6
#define ELBOW_SW 6

Adafruit_MPU6050 mpu;

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

//address through which the RF modules communicate.
const byte address[6] = "00001";

//A tact swtich for transferring the Y-axis control between the elbow and shoulder servos
bool elbow = false;

const int gripper_ctrl = A0;

void setup() {
  Serial.begin(115200);

  // Try to initialize the MPU6050 sensor
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Sensor Found!");

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

  pinMode(ELBOW_SW, INPUT_PULLUP); // Default Y-axis control goes to the shoulder

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

  //Check if the elbow switch is pressed
  elbow = (digitalRead(ELBOW_SW) == LOW)? true : false;

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

  /*Building the payload in this format: "X__Y__G"
    We need to convert the acceleration along:
        X-axis into Left and Right Movement (E.g L1)
        Y-axis into UP and DOWM Movement (if ELBOW_SW is pressed)
        Y-axis into FORWARD and BACKWARD MOVEMENT (if ELBOW_SW is not pressed)
  */

  if (X>=2 && X<=10) {
    payload[0] = 'R';
    payload[1] = '0' + map(X, 2, 10, 1, 9);

  }  else if (X<=-2 && X>=-10) {
    payload[0] = 'L';
    payload[1] = '0' + map(X, -2, -10, 1, 9);

  } else {
    payload[0] = 'M';
    payload[1] = '0';
  }

  if (Y>=2 && Y<=10){
    payload[2] = (elbow)? 'U':'F';
    payload[3] = '0' + map(Y, 2, 10, 1, 9);
  
  } else if (Y<=-2 && Y>=-10) {
    payload[2] = (elbow)? 'D':'B';
    payload[3] = '0' + map(Y, -2, -10, 1, 9);

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
