#include <Arduino.h>
#include "SPI.h"
#include "mcp2515.h"

struct can_frame canMsg1;
struct can_frame canMsg2;
struct can_frame canMsg3;
struct can_frame canMsg4;
struct can_frame canMsg5;

MCP2515 mcp2515(10);

const int INPUT_PIN = A0;    // input pin for A0
int16_t speed;                   // set hex as speed

void setup() 
{

  
  canMsg1.can_id  = 0x109; // can terminator setting
  canMsg1.can_dlc = 8;
  canMsg1.data[0] = 0;     // terminal on id=1
  canMsg1.data[1] = 0;
  canMsg1.data[2] = 0;
  canMsg1.data[3] = 0;
  canMsg1.data[4] = 0;
  canMsg1.data[5] = 0;
  canMsg1.data[6] = 0;
  canMsg1.data[7] = 0;

  canMsg2.can_id  = 0x106; // motor status period
  canMsg2.can_dlc = 8;
  canMsg2.data[0] = 0x80;     // terminal on id=1
  canMsg2.data[1] = 0;
  canMsg2.data[2] = 0;
  canMsg2.data[3] = 0;
  canMsg2.data[4] = 0;
  canMsg2.data[5] = 0;
  canMsg2.data[6] = 0;
  canMsg2.data[7] = 0;

  canMsg3.can_id  = 0x105; // set motor mode
  canMsg3.can_dlc = 8;
  canMsg3.data[0] = 0x00;     // open mode, id=1
  canMsg3.data[1] = 0;
  canMsg3.data[2] = 0;
  canMsg3.data[3] = 0;
  canMsg3.data[4] = 0;
  canMsg3.data[5] = 0;
  canMsg3.data[6] = 0;
  canMsg3.data[7] = 0;

  canMsg4.can_id  = 0x107; // motor status request
  canMsg4.can_dlc = 8;
  canMsg4.data[0] = 0x01;
  canMsg4.data[1] = 0x01;
  canMsg4.data[2] = 0x02;
  canMsg4.data[3] = 0x04;
  canMsg4.data[4] = 0x55;
  canMsg4.data[5] = 0;
  canMsg4.data[6] = 0;
  canMsg4.data[7] = 0;

  canMsg5.can_id  = 0x32; // motor speed
  canMsg5.can_dlc = 8;
  canMsg5.data[0] = 0x00;
  canMsg5.data[1] = 0x50;
  canMsg5.data[2] = 0;
  canMsg5.data[3] = 0;
  canMsg5.data[4] = 0;
  canMsg5.data[5] = 0;
  canMsg5.data[6] = 0;
  canMsg5.data[7] = 0;

  while (!Serial);
  Serial.begin(115200);                //Begins Serial Communication at 9600 baudrate
  
  SPI.begin();                               //Begins SPI communication
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS,MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();

  mcp2515.sendMessage(&canMsg1);
  mcp2515.sendMessage(&canMsg2);
  mcp2515.sendMessage(&canMsg3);
  mcp2515.sendMessage(&canMsg4); 
  delay(10);
  
  Serial.println("------- m15 setup finished----------");


}

void loop() 
{
  // Read the analog input value
  speed = analogRead(INPUT_PIN);

  // Scale the speed value to a range suitable for your motor 0 to 32767
  speed = map(speed, 0, 1023, 0, 32767);

  // Print the hex string to the serial port
  Serial.println(speed);

  // Update the can message with the new speed value
  canMsg5.data[0] = speed >> 8;
  canMsg5.data[1] = speed & 0x00ff;

  // Send the updated message to the motor
  mcp2515.sendMessage(&canMsg5);
  
  
  delay(100);  // delay for 100 milliseconds
}
