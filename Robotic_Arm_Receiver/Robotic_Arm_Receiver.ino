#include <Servo.h>
#include <SPI.h>
#include "RF24.h"
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

#define BASE_SERVO 3
#define SHOULDER_SERVO 5
#define ELBOW_SERVO 6
#define GRIPPER_SERVO 9

#define BASE_START_POS 60
#define SHOULDER_START_POS 90
#define ELBOW_START_POS 90
#define GRIPPER_START_POS 40

//Define the Maximum possible position for the servos
#define BASE_MAX_POS 180
#define SHOULDER_MAX_POS 180
#define ELBOW_MAX_POS 120
#define GRIPPER_MAX_POS 180

//Define the Maximum possible position for the servos
#define BASE_MIN_POS 0
#define SHOULDER_MIN_POS 50
#define ELBOW_MIN_POS 0
#define GRIPPER_MIN_POS 0

#define CE_PIN 7
#define CSN_PIN 8

Servo base;
Servo shoulder;
Servo elbow;
Servo gripper;

SoftwareSerial Bluetooth(4, 10);

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);
//address through which the RF modules communicate.
const byte address[6] = "00001";

SimpleTimer timer(1000);

bool grip_open = true;

void servo_RF_control(char directionX, char directionY, int angleX, int angleY, char grip){
  int posX, posY, posG;

  switch (directionX) {
    case 'R':
      posX = base.read() - 1;
      if (posX < BASE_MIN_POS) posX = BASE_MIN_POS;
      base.write(posX);

      // posX = base.read() - angleX*2;
      // if (posX < BASE_MIN_POS) posX = BASE_MIN_POS;

      // for (int i = base.read(); i >= posX; i--) {
      //   base.write(i);
      //   delay(10);
      // }
     
      break;

    case 'L':
      posX = base.read() + 1;
      if(posX > BASE_MAX_POS) posX = BASE_MAX_POS;
      base.write(posX);

      // posX = base.read() + angleX*2;
      // if (posX > BASE_MAX_POS) posX = BASE_MAX_POS;
      
      // for (int i = base.read(); i <= posX; i++) {
      //   base.write(i);
      //   delay(10);
      // }

      break;
    
  }

  switch (directionY) {
    case 'F':
      posY = shoulder.read() + 1;
      if (posY > SHOULDER_MAX_POS) posY = SHOULDER_MAX_POS;
      shoulder.write(posY);

      // posY = shoulder.read() + angleY*2;
      // if (posY > SHOULDER_MAX_POS) posY = SHOULDER_MAX_POS;
      
      // for (int i = shoulder.read(); i <= posY; i++) {
      //   shoulder.write(i);
      //   delay(10);
      // }

      break;

    case 'B':
      posY = shoulder.read() - 1;
      if (posY < SHOULDER_MIN_POS) posY = SHOULDER_MIN_POS;
      shoulder.write(posY);

      // posY = shoulder.read() - angleY*2;
      // if (posY < SHOULDER_MIN_POS) posY = SHOULDER_MIN_POS;
      
      // for (int i = shoulder.read(); i >= posY; i--) {
      //   shoulder.write(i);
      //   delay(10);
      // }

      break;

    case 'U':
      posY = elbow.read() - 1;
      if (posY < ELBOW_MIN_POS) posY = ELBOW_MIN_POS;
      elbow.write(posY);

      // posY = elbow.read() - angleY*2;
      // if (posY < ELBOW_MIN_POS) posY = ELBOW_MIN_POS;
      
      // for (int i = elbow.read(); i >= posY; i--) {
      //   elbow.write(i);
      //   delay(10);
      // }

      break;

    case 'D':
    posY = elbow.read() + 1;
    if(posY > ELBOW_MAX_POS) posY = ELBOW_MAX_POS;
    elbow.write(posY);

    // posY = elbow.read() + angleY*2;
    //   if (posY > ELBOW_MAX_POS) posY = ELBOW_MAX_POS;
      
    //   for (int i = elbow.read(); i <= posY; i++) {
    //     elbow.write(i);
    //     delay(10);
    //   }

      break;
  }

  switch (grip) {
    case 'C':
      // posG = gripper.read() - 1;
      if (grip_open) {
        for (int i = GRIPPER_MAX_POS; i>=GRIPPER_MIN_POS; i--){
          gripper.write(i);
          delay(15);
        }
        grip_open = false;
      }
      
      break;
    
    case 'O':
      // posG = gripper.read() + 1;
      if(!grip_open){
        for (int i = GRIPPER_MIN_POS; i<=GRIPPER_MAX_POS; i++){
          gripper.write(i);
          delay(15);
        }
        grip_open = true;
      }
      break;
  }

  delay(20);
  
}

void servo_BT_control(char servo, int pos){
  int prev_pos, new_pos;

  switch (servo) {
    case 'B':
    Serial.println("Hi");
      prev_pos = base.read();

      (pos > BASE_MAX_POS)? new_pos = BASE_MAX_POS : new_pos = pos;
      (pos < BASE_MIN_POS)? new_pos = BASE_MIN_POS : new_pos = pos;

      if (prev_pos < new_pos){
        for (int i=prev_pos; i<=pos; i++){
          base.write(i);
          delay(20);
        }
      } else {
        for (int i=prev_pos; i>=new_pos; i--){
          base.write(i);
          delay(20);
        }
      }
      break;

    case 'S':
      prev_pos = shoulder.read();

      (pos > SHOULDER_MAX_POS)? new_pos = SHOULDER_MAX_POS : new_pos = pos;
      (pos < SHOULDER_MIN_POS)? new_pos = SHOULDER_MIN_POS : new_pos = pos;

      if (prev_pos < new_pos){
        for (int i=prev_pos; i<=new_pos; i++){
          shoulder.write(i);
          delay(20);
        }
      } else {
        for (int i=prev_pos; i>=new_pos; i--){
          shoulder.write(i);
          delay(20);
        }
      }
      break;

    case 'E':
      prev_pos = elbow.read();

      (pos > ELBOW_MAX_POS)? new_pos = ELBOW_MAX_POS : new_pos = pos;
      (pos < ELBOW_MIN_POS)? new_pos = ELBOW_MIN_POS : new_pos = pos;

      if (prev_pos < new_pos){
        for (int i=prev_pos; i<=new_pos; i++){
          elbow.write(i);
          delay(20);
        }
      } else {
        for (int i=prev_pos; i>=new_pos; i--){
          elbow.write(i);
          delay(20);
        }
      }
      break;

    case 'G':
      prev_pos = gripper.read();

      (pos > GRIPPER_MAX_POS)? new_pos = GRIPPER_MAX_POS : new_pos = pos;
      (pos < GRIPPER_MIN_POS)? new_pos = GRIPPER_MIN_POS : new_pos = pos;

      if (prev_pos < new_pos){
        for (int i=prev_pos; i<=new_pos; i++){
          gripper.write(i);
          delay(20);
        }
      } else {
        for (int i=prev_pos; i>=new_pos; i--){
          gripper.write(i);
          delay(20);
        }
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);  

  base.attach(BASE_SERVO);
  shoulder.attach(SHOULDER_SERVO);
  elbow.attach(ELBOW_SERVO);
  gripper.attach(GRIPPER_SERVO);

  if(!radio.begin())
    Serial.println(F("radio hardware is not responding!!"));
  else
    Serial.println(F("Found RF Module!"));

  radio.setPALevel(RF24_PA_MAX);
  //set the address of the tranceiver
  radio.openReadingPipe(0, address);
  //Set module as receiver
  radio.startListening();

  Bluetooth.begin(9600);

  //Move the servos to their starting position
  base.write(BASE_START_POS);
  shoulder.write(SHOULDER_START_POS);
  elbow.write(ELBOW_START_POS);
  gripper.write(GRIPPER_START_POS);

}

void loop() {
  //Read the data if available from the RF transceiver buffer
  if (radio.available())
  {
    char RF_data[5];
    radio.read(&RF_data, sizeof(RF_data));
    Serial.print("Payload: ");
    Serial.println(RF_data);

    char directionX = RF_data[0];
    int angleX = atoi(RF_data + 1);
    // Serial.println(angleX);

    char directionY = RF_data[2];
    int angleY = atoi(RF_data + 3);
    // Serial.println(angleY);

    char grip = RF_data[4];

    servo_RF_control(directionX, directionY, angleX, angleY, grip);

  }

  //Read the data if available from the RF transceiver buffer
  if (!timer.isReady() && Bluetooth.available()) {
      Bluetooth.readString();
  }
  
  if (timer.isReady() && Bluetooth.available()) {
      String BT_data = Bluetooth.readString();
      Serial.println(BT_data);
      char servo = BT_data.charAt(0);
      int pos = BT_data.substring(1, BT_data.length()).toInt();

      servo_BT_control(servo, pos);
      timer.reset();
  }
}