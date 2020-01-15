#include <RH_ASK.h>
#include <SPI.h>

//data on pin 12

RH_ASK driver;
const short btn = 5;

void setup()
{
    pinMode(btn, INPUT);
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("Unable to establish communication with transmitter.");
}

void loop()
{
  if(btn == HIGH){
    const char *msg = "RAJ";    // if anything other than RAJ (Password) is sent buzzer won't beep.
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);
  }
  else{
    delay(500);
    }
}
