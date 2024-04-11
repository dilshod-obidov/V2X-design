#include "AFMotor.h"
#include <Servo.h>

#define echopin A4 // echo pin
#define trigpin A5 // Trigger pin

Servo myservo;

AF_DCMotor motor1(1, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor2(2, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor3(3, MOTOR12_64KHZ); // create motor object, 64KHz pwm
AF_DCMotor motor4(4, MOTOR12_64KHZ); // create motor object, 64KHz pwm
//===============================================================================
//  Initialization
//===============================================================================

int distance_L, distance_F, distance_R;
long distance;

int set = 20;
 
void setup() {
  Serial.begin(9600);           // Initialize serial port
  Serial.println("Start");

  myservo.attach(10);
  myservo.write(90);

  pinMode (trigpin, OUTPUT);
  pinMode (echopin, INPUT );
  
  motor1.setSpeed(150);          // set the motor speed to 0-255
  motor2.setSpeed(150);
  motor3.setSpeed(150);
  motor4.setSpeed(150);
}
//===============================================================================
//  Main
//=============================================================================== 
void loop() {
 distance_F = getDistance();
 Serial.print("Distance: ");
 Serial.println(distance_F);
  if (distance_F > set)
  {
    Serial.println("Forward");
    motor1.run(FORWARD);         // go forward
    motor2.run(FORWARD); 
    motor3.run(FORWARD); 
    motor4.run(FORWARD);
  }
  else
  {
    servoControl();
  }
}


long getDistance(){
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  distance = pulseIn (echopin, HIGH);
  return distance / 29 / 2;
}



void compareDistance(){
  if (distance_L > distance_R)
  {
    motor1.run(BACKWARD);   // go left
    motor2.run(BACKWARD);
    motor3.run(FORWARD); 
    motor4.run(FORWARD); 
    delay(350);
  }
  else if (distance_R > distance_L)
  {
    motor1.run(FORWARD);  // go right
    motor2.run(FORWARD); 
    motor3.run(BACKWARD); 
    motor4.run(BACKWARD);
    delay(350);
  }
  else
  {
  motor1.run(BACKWARD);  // go back 
  motor2.run(BACKWARD);
  motor3.run(BACKWARD); 
  motor4.run(BACKWARD); 
   delay(300);
  motor1.run(BACKWARD);   // go left
  motor2.run(BACKWARD);
  motor3.run(FORWARD); 
  motor4.run(FORWARD); 
    delay(500);
  }
}

void servoControl(){
    Serial.println("Barrier at the front! Stop!");
    motor1.run(RELEASE);         // stop
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
   
    myservo.write(0);
    delay(300);
    distance_R = getDistance();
    delay(100);
    myservo.write(179);
    delay(500);
    distance_L = getDistance();
    delay(100);
    myservo.write(90);
    delay(300);
    compareDistance();
}