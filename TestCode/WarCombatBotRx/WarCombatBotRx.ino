#include <VirtualWire.h>
char SensorCharMsg[6]; // RF Transmission container
#define SHOOT 12
#define PUMP A1
#define BUZZER 13
#define FLAME_SENSOR A2

uint8_t MOTOR[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i <= 7; i++) {
    pinMode(MOTOR[i], OUTPUT);
  }
  pinMode(PUMP, OUTPUT);
  pinMode(SHOOT, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(FLAME_SENSOR, INPUT);

  vw_set_ptt_inverted(true);
  vw_setup(2000);
  vw_rx_start();


}

void loop()
{
  //----------------------RF_DATA_READ_FUNCTION----------------------//
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen))
  {
    for (int i = 0; i < buflen; i++)
    {
      SensorCharMsg[i] = char(buf[i]);
    }
    SensorCharMsg[buflen] = '\0';
    Serial.println(SensorCharMsg);
  }

  //----------------------DIRECTION_CONDITIONS----------------------//
  if (SensorCharMsg[0] == 'F') {
    Forward();
  }
  else if (SensorCharMsg[0] == 'B') {
    Backward();
  }
  else if (SensorCharMsg[0] == 'L') {
    Left();
  }
  else if (SensorCharMsg[0] == 'R') {
    Right();
  }
  else if (SensorCharMsg[0] == 'S') {
    Stop();
  }

  //----------------------GRIPPER_CONDITIONS----------------------//
  if (SensorCharMsg[1] == 'C') {
    gripperHold();
  }
  else if (SensorCharMsg[1] == 'O') {
    gripperRelease();
  }
  else if (SensorCharMsg[1] == 'N') {
    gripperPause();
  }

  //----------------------BASE_CONDITIONS----------------------//
  if (SensorCharMsg[2] == 'U') {
    baseUp();
  }
  else if (SensorCharMsg[2] == 'D') {
    baseDown();
  }
  else if (SensorCharMsg[2] == 'M') {
    basePause();
  }

  //----------------------WATER_PUMP_FUNCTON----------------------//
  if (SensorCharMsg[3] == 'W') {
    Serial.println("PUMP ON");
    analogWrite(PUMP, 1023);
  }
  else if (SensorCharMsg[3] == 'X') {
    Serial.println("PUMP OFF");
    analogWrite(PUMP, 0);
  }

  //----------------------SHOOT_FUNCTON----------------------//
  if (SensorCharMsg[4] == 'Y') {
    digitalWrite(SHOOT, HIGH);
  }
  else if (SensorCharMsg[4] == 'Z') {
    digitalWrite(SHOOT, LOW);
  }

  //----------------------FLAME_DETECTOR_FUNCTION----------------------//
  flameDetector();


}

//FLAME_DETECTOR FUNCTION
void flameDetector() {

  if (digitalRead(FLAME_SENSOR) == LOW) {
    digitalWrite(BUZZER, HIGH);
  }
  else {
    digitalWrite(BUZZER, LOW);
  }
}


//ARM_GRIPPER FUNCTIONS
void armGripper(uint8_t A, uint8_t B) {
  digitalWrite(MOTOR[4], A);
  digitalWrite(MOTOR[5], B);
}
void gripperHold() {
  Serial.println("OPEN");
  armGripper(1, 0);
}
void gripperRelease() {
  Serial.println("CLOSE");
  armGripper(0, 1);
}
void gripperPause() {
  Serial.println("N");
  armGripper(0, 0);
}

//ARM_BASE FUNCTIONS
void armBase(uint8_t A, uint8_t B) {
  digitalWrite(MOTOR[6], A);
  digitalWrite(MOTOR[7], B);
}
void baseUp() {
  Serial.println("UP");
  armBase(1, 0);
}
void baseDown() {
  Serial.println("DOWN");
  armBase(0, 1);
}
void basePause() {
  Serial.println("M");
  armBase(0, 0);
}

//DIRECTION FUNCTIONS
void motorGo(uint8_t A, uint8_t B, uint8_t C, uint8_t D) {
  digitalWrite(MOTOR[0], A);
  digitalWrite(MOTOR[1], B);
  digitalWrite(MOTOR[2], C);
  digitalWrite(MOTOR[3], D);
}
void Forward() {
  Serial.println("Forward");
  motorGo(1, 0, 0, 1);//Forward
}
void Backward() {
  Serial.println("Backward");
  motorGo(0, 1, 1, 0);//Backward
}
void Left() {
  Serial.println("Left");
  motorGo(0, 1, 0, 1);//Left
}
void Right() {
  Serial.println("Right");
  motorGo(1, 0, 1, 0);//Right
}
void Stop() {
  Serial.println("Stop");
  motorGo(0, 0, 0, 0);//Stop
}
