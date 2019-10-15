
#ifndef _TRAFFICLIGHTS_H
#define _TRAFFICLIGHTS_H

#include <arduino.h>

#define OFF 			 0x00
#define RED 			 0x01
#define GREEN 			 0x02
#define YELLOW 			 0x03
//Special
#define GREEN_YELLOW 	 0x04
#define YELLOW_RED 		 0x05
#define RED_YELLOW 		 0x06
#define GREEN_YELLOW_RED 0x07

#define I2C_ADDR1 0x01
#define I2C_ADDR2 0x02

class TrafficLights{

private:

void handleIDcmd(uint8_t i, uint8_t* i2c_buffer);
void handleFrame();
void handleI2Cdevice(uint8_t i);

public:

void begin();
void feed(uint8_t data);
void addTrafficLight(uint8_t ID, uint8_t i2c_addr, uint8_t pin1, uint8_t pin2, uint8_t pin3);
	
};


#endif



