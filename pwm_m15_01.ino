#include <Arduino.h>
#define SERIAL_DEBUG

#define MAX_RPM 100
#define PWM_PIN_L 3
#define PWM_PIN_R 5
#define PWM_L 1100
#define PWM_M 1500
#define PWM_H 1900
#define PWM_DZ 20

#include <SPI.h>     // Library for using SPI Communication
#include <mcp2515.h> // Library for using CAN Communication
MCP2515 mcp2515(10);

struct can_frame canMsg;
struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
struct can_frame canMsg4;
struct can_frame canMsg5;
struct can_frame canMsg6;

int16_t rpmL1;
int16_t rpmR1;

float pwm2rpm(int pwm)
{
  pwm = constrain(pwm, PWM_L, PWM_H);
  float rpm;

  if(abs(pwm-PWM_M) < PWM_DZ)
    rpm = 0.0;
  else if(pwm < PWM_M)
    rpm = (float)(pwm-PWM_M)/(float)(PWM_M-PWM_L);
  else
    rpm = (float)(pwm-PWM_M)/(float)(PWM_H-PWM_M);

  return rpm * (float)MAX_RPM;
}

void setup() {

  canMsg1.can_id  = 0x109; // can terminator setting
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0;     // CAN terminal off, id=1
  canMsg1.data[1] = 0;     // CAN terminal off, id=2
  canMsg1.data[2] = 0;
  canMsg1.data[3] = 0;
  canMsg1.data[4] = 0;
  canMsg1.data[5] = 0;
  canMsg1.data[6] = 0;
  canMsg1.data[7] = 0;

  canMsg2.can_id  = 0x106; // motor feedback method
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x80;     // polling timing, id=1
  canMsg2.data[1] = 0x80;     // polling timing, id=2
  canMsg2.data[2] = 0;
  canMsg2.data[3] = 0;
  canMsg2.data[4] = 0;
  canMsg2.data[5] = 0;
  canMsg2.data[6] = 0;
  canMsg2.data[7] = 0;

  canMsg3.can_id  = 0x105; // set motor mode
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 0x00;     // open loop mode, id=1
  canMsg3.data[1] = 0x00;     // open loop mode, id=2
  canMsg3.data[2] = 0;
  canMsg3.data[3] = 0;
  canMsg3.data[4] = 0;
  canMsg3.data[5] = 0;
  canMsg3.data[6] = 0;
  canMsg3.data[7] = 0;

  canMsg4.can_id  = 0x107; // motor status request, id=1
  canMsg4.can_dlc = 8;
  canMsg4.data[0] = 0x01;  //id=1
  canMsg4.data[1] = 0x01;
  canMsg4.data[2] = 0x02;
  canMsg4.data[3] = 0x04;
  canMsg4.data[4] = 0x55;
  canMsg4.data[5] = 0;
  canMsg4.data[6] = 0;
  canMsg4.data[7] = 0;
  
  canMsg5.can_id  = 0x107; // motor status request, id=2
  canMsg5.can_dlc = 8;
  canMsg5.data[0] = 0x02;  //id=2
  canMsg5.data[1] = 0x01;
  canMsg5.data[2] = 0x02;
  canMsg5.data[3] = 0x04;
  canMsg5.data[4] = 0x55;
  canMsg5.data[5] = 0;
  canMsg5.data[6] = 0;
  canMsg5.data[7] = 0;

  // Send the updated message to the motor
  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);
  mcp2515.sendMessage(&canMsg3);
  mcp2515.sendMessage(&canMsg4);
  mcp2515.sendMessage(&canMsg5);
  mcp2515.sendMessage(&canMsg6);

  SPI.begin(); // Begins SPI communication
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ); // Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();
  
  while (!Serial);
  Serial.begin(115200);                //Begins Serial Communication at 115200 baudrate
  
  pinMode(PWM_PIN_L, INPUT);
  pinMode(PWM_PIN_R, INPUT);
  
  Serial.println("Initialized complete!");

}

void loop() {

  float rpmL = 0.0;
  float rpmR = 0.0;
    
  rpmL = pwm2rpm(pulseIn(PWM_PIN_L, HIGH));
  rpmR = pwm2rpm(pulseIn(PWM_PIN_R, HIGH));
  rpmL1 = map(rpmL, 0, 100, 0, 32767);  // pwm input -100 to 100 mapped -32767 to 32767
  rpmR1 = map(rpmR, 0, 100, 0, -32767);  // pwm input -100 to 100 mapped -32767 to 32767, reverse rotation

  canMsg6.can_id  = 0x32;
  canMsg6.can_dlc = 8;
  canMsg6.data[0] = rpmL1 >> 8;
  canMsg6.data[1] = 0;
  canMsg6.data[2] = rpmR1 >> 8;
  canMsg6.data[3] = 0;
  canMsg6.data[4] = 0;
  canMsg6.data[5] = 0;
  canMsg6.data[6] = 0;
  canMsg6.data[7] = 0;

  // Send the updated message to the motor
  mcp2515.sendMessage(&canMsg6);
  delay(1);

  #ifdef SERIAL_DEBUG
   Serial.print("rpm: ");
   Serial.print(rpmL1 >> 8);
   Serial.print(", ");
   Serial.print(rpmR1 >> 8);
   Serial.println();
  #endif

}
