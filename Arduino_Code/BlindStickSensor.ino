// Arduino Code for Smart Stick for Blind People
#include <RH_ASK.h>
#include <SPI.h>

//data on pin 11

RH_ASK driver;

// Variables
const short triggerPin = 3;
const short reflectPin = 2;
const short vibraMotor = 5;
const int buzzer = 6;
const int HIGH_OUT = 13;

long duration = 0;
long distance = 0;
int vibrationInt = 0;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(reflectPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(vibraMotor,OUTPUT);
  pinMode(HIGH_OUT,OUTPUT);
  Serial.begin(9600);
  if (!driver.init()){
    Serial.println("init failed");
  }
}

void loop() {
  digitalWrite(HIGH_OUT, HIGH);

  // Triggering Ultrasonic Sensor
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Reading inputs from Ultrasonic Sensor
  duration = pulseIn(reflectPin, HIGH);

  // Calculating Distance
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(100);

  // Checking if obstacle is nearby
  if (distance < 30){
    double nearDist = 30-distance;
    vibrationInt = map(nearDist, 0, 30, 0, 255);
    analogWrite(vibraMotor, vibrationInt);
    delay(200);
  }
  else{
    analogWrite(vibraMotor, 0);
    delay(200);
  }

  // Receiving a signal for Beeper
  uint8_t buf[3];
  uint8_t buflen = sizeof(buf);

  // Checking if something is received
  if (driver.recv(buf, &buflen))
  {
    int i;
    // Message with a good checksum received, dump it.
    Serial.print("Message: ");
    Serial.println(buf[0]);
    Serial.println(buf[1]); 
    Serial.println(buf[2]); 

    // Checking if received matches with desired keyword (Password)
    // 82 = R, 65 = A, 74 = J
    if(buf[0] == 82 && buf[1] == 65 && buf[2] == 74){
      tone(buzzer, 1000); // Send 1KHz sound signal
      delay(1000);
      noTone(buzzer);     // Stop sound
      delay(1000);       
    }
  }
}
