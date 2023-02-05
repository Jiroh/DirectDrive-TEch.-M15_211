# DirectDrive-Tech.-M15_211-tools is prepared for testing drive of DirectDrive Tech. M15_212.
1) "main.cpp" is prepare for atmega_rossserial upload by platformio for teleop_twist. Hardware is mcp2515 on Arduino Pro nano.
    Reference to https://github.com/asukiaaa/kagotos
    *1 reverse one motor rotation to install motors back to back.
2) "m15_potentio_arduino.ino" is just playing with change speed of m15 motor. Arduino Duemilanov is prepared with mcp2515.
3) "set_id2_m15_arduino.ino" is used to change motor CAN ID from 1 to 2.
4) "pwm_m15_01.ino" is prepared for convert two PWM signals to two M15_212 motors that expect to cobtrol m15_212 motors via ardurover. Unit is consist from mcp2515 can-bus interface and Arduino-nano. refrence to https://github.com/rasheeddo/BrushlessDriveWheels
