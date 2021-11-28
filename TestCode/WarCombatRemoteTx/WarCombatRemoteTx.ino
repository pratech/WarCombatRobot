#include <VirtualWire.h>

#define XJOY_PIN A0
#define YJOY_PIN A1
#define XXJOY_PIN A2
#define YYJOY_PIN A3
#define SWITCH_XY 2
#define SWITCH_XXYY 3

bool sleepFlag=true;
int sleepFlagCount=0;
void setup()
{
  Serial.begin(9600);
  pinMode(SWITCH_XY, INPUT_PULLUP);
  pinMode(SWITCH_XXYY, INPUT_PULLUP);
  // VirtualWire setup
  vw_setup(2000);     // Bits per sec
}
void loop()
{
sleepFlag=true;
  char SensorCharMsg[6] = {};
  uint8_t index_SensorCharMsg = 0;

  int joystickXVal = analogRead(XJOY_PIN) ;
  int joystickYVal = analogRead(YJOY_PIN) ;
  int joystickXXVal = analogRead(XXJOY_PIN) ;
  int  joystickYYVal = analogRead(YYJOY_PIN) ;
  //Robot Direction Controller Joystick.
  if (joystickXVal >= 900) {
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'F' ;
    index_SensorCharMsg++;
    //Forward
  }
  else if (joystickXVal <= 300) {
	  sleepFlag=false;
   SensorCharMsg[index_SensorCharMsg] = 'B' ;
    index_SensorCharMsg++; //Backward
  }
  else if (joystickYVal >= 900) {
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'L' ;
    index_SensorCharMsg++; //Left
  }
  else if (joystickYVal <= 300 ) {
	  sleepFlag=false;
  SensorCharMsg[index_SensorCharMsg] = 'R' ;
    index_SensorCharMsg++;//Right
  }
  else {
    SensorCharMsg[index_SensorCharMsg] = 'S' ;
    index_SensorCharMsg++;//Stop
  }

  //Arm Controller Joystick
  if (joystickXXVal >= 900) {
	  sleepFlag=false;
   SensorCharMsg[index_SensorCharMsg] = 'C' ;
    index_SensorCharMsg++; //GripperClose
  }
  else if (joystickXXVal <= 300) {
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'O' ;
    index_SensorCharMsg++;//GripperOpen
  }
  else {
    SensorCharMsg[index_SensorCharMsg] = 'N' ;
    index_SensorCharMsg++; //Stop
  }

  if (joystickYYVal >= 900) 
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'U' ;
    index_SensorCharMsg++;//ArmUp
  }
  else if (joystickYYVal <= 300 ) {
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'D' ;
    index_SensorCharMsg++;//ArmDown
  }
  else {
    SensorCharMsg[index_SensorCharMsg] = 'M' ;
    index_SensorCharMsg++; //Stop
  }

  if (digitalRead(SWITCH_XY) == LOW) {
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'W' ;
    index_SensorCharMsg++;//Pump ON
  }
  else {
    SensorCharMsg[index_SensorCharMsg] = 'X' ;
    index_SensorCharMsg++; //Pump OFF
  }
  if (digitalRead(SWITCH_XXYY) == LOW) {
	  sleepFlag=false;
    SensorCharMsg[index_SensorCharMsg] = 'Y' ;
    index_SensorCharMsg++; //Shoot ON
  } else {
    SensorCharMsg[index_SensorCharMsg] = 'Z' ;
    index_SensorCharMsg++;//Shoot OFF
  }

 SensorCharMsg[index_SensorCharMsg] = '\0' ;

  Serial.println(SensorCharMsg);
  if(sleepFlag)
  {
	sleepFlagCount++;
  }
  else
  {
	sleepFlag=0;
  }
  if(sleepFlagCount>=10)
  {
  }
  else{
  vw_send((uint8_t *)SensorCharMsg, strlen(SensorCharMsg));  // send the message
  vw_wait_tx(); // Wait until the whole message is gone
  }
  Serial.println(SensorCharMsg);
}
