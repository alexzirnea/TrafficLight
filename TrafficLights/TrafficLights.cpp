#include "TrafficLights.h"
#include <Wire.h>

//CHANGE ADDRESS IN TrafficLights.h !!

const uint8_t terminatorChar = '\n';
uint8_t trafficLightsCounter = 0;
uint8_t frame[2];
uint8_t frame_counter = 0;
uint8_t id_counter = 0;
uint8_t i2c_buffer1 = 0x00;
uint8_t i2c_buffer2 = 0x00;
bool frame_done = false;
bool frame_start = false;

struct traffic_light{
	//Pins below should be from 0 to 7
	uint8_t red_pin;
	uint8_t green_pin;
	uint8_t yellow_pin;
	uint8_t i2c_addr;
	uint8_t ID;
};

traffic_light trafficLights[10];

void TrafficLights::handleI2Cdevice(uint8_t i){
	switch(trafficLights[i].i2c_addr){
		case I2C_ADDR1:
		handleIDcmd(i, &i2c_buffer1);
		break;
		case I2C_ADDR2:
		handleIDcmd(i, &i2c_buffer2);
		break;
		default:
		break;
	}
}

void TrafficLights::handleIDcmd(uint8_t i, uint8_t* i2c_buffer){
	//Mask bits before writing anything
	uint8_t mask = 	1 << trafficLights[i].red_pin &
					1 << trafficLights[i].green_pin &
					1 << trafficLights[i].yellow_pin;
	*i2c_buffer &= ~mask;
	
	switch(frame[1]){
		case OFF:
		//Serial.println("OFF");
		*i2c_buffer |= 0 <<	trafficLights[i].red_pin 	|
					   0 <<	trafficLights[i].green_pin	|
					   0 << trafficLights[i].yellow_pin;
		break;
		
		case RED:
		//Serial.println("RED");
		*i2c_buffer |= 1 <<	trafficLights[i].red_pin 	|
					   0 <<	trafficLights[i].green_pin	|
					   0 << trafficLights[i].yellow_pin;
		break;
		
		case GREEN:
		//Serial.println("GREEN");
		*i2c_buffer |= 0 <<	trafficLights[i].red_pin 	|
					   1 <<	trafficLights[i].green_pin	|
					   0 << trafficLights[i].yellow_pin;
		break;
		
		case YELLOW:
		//Serial.println("YELLOW");
		*i2c_buffer |= 0 <<	trafficLights[i].red_pin 	|
					   0 <<	trafficLights[i].green_pin	|
					   1 << trafficLights[i].yellow_pin;
		break;
		
		case GREEN_YELLOW:
		//Serial.println("GREEN_YELLOW");
		*i2c_buffer |= 0 <<	trafficLights[i].red_pin 	|
					   1 <<	trafficLights[i].green_pin	|
					   1 << trafficLights[i].yellow_pin;
		break;
		
		case YELLOW_RED:
		//Serial.println("YELLOW_RED");
		*i2c_buffer |= 1 <<	trafficLights[i].red_pin 	|
					   0 <<	trafficLights[i].green_pin	|
					   1 << trafficLights[i].yellow_pin;
		break;
		
		case RED_YELLOW:
		//Serial.println("RED_YELLOW");
		*i2c_buffer |= 1 <<	trafficLights[i].red_pin 	|
					   0 <<	trafficLights[i].green_pin	|
					   1 << trafficLights[i].yellow_pin;
		break;
		
		case GREEN_YELLOW_RED:
		//Serial.println("GREEN_YELLOW_RED");
		*i2c_buffer |= 1 <<	trafficLights[i].red_pin 	|
					   1 <<	trafficLights[i].green_pin	|
					   1 << trafficLights[i].yellow_pin;
		break;
		
		default:
		Serial.println("DEFAULT");
		break;
	}
	
	Wire.beginTransmission(trafficLights[i].i2c_addr);                          
	Wire.write(*i2c_buffer);           
	Wire.endTransmission();
	
}

void TrafficLights::handleFrame(){
	for(uint8_t i=0; i<trafficLightsCounter; i++){
		if(frame[0] == trafficLights[i].ID){
			handleI2Cdevice(i);
			return;
		}
	}
}

void TrafficLights::begin(){
	Wire.begin();
}

void TrafficLights::feed(uint8_t data){
	if(data == terminatorChar) {
		frame_counter=0;
		if(frame_start) frame_done = true;
		frame_start = true;
		if(frame_start && frame_done)handleFrame();
	} 
	else {
		frame[frame_counter]=data;
		frame_counter++;
		
	}
}

void TrafficLights::addTrafficLight(uint8_t ID, uint8_t i2c_addr, uint8_t pin1, uint8_t pin2, uint8_t pin3){
	//Light order: Red, Yellow, Green

	trafficLights[trafficLightsCounter].ID = ID;
	trafficLights[trafficLightsCounter].i2c_addr = i2c_addr;
	trafficLights[trafficLightsCounter].red_pin = pin1;
	trafficLights[trafficLightsCounter].yellow_pin = pin2;
	trafficLights[trafficLightsCounter].green_pin = pin3;
	
	trafficLightsCounter++;
	
}