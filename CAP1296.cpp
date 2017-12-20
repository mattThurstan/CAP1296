/*
*Arduino library for Microchip capacitive touch IC - CAP1296
*Uses I2C for communication.
*
*Based on CAP1298 library by Chris Ball
*https://github.com/ChrisBall/CAP1298
*/

#include "Arduino.h"
#include "CAP1296.h"

CAP1296::CAP1296() {
}

boolean CAP1296::begin(uint8_t i2caddr) {
    Wire.begin();
    _i2caddr = i2caddr;
	

	if ((readRegister(CAP1296_PRODID) != 0x69) || (readRegister(CAP1296_MANUID) != 0x5D)) {
		Serial.println("Device ID does not match");
		return false;
	}else{
		Serial.println("Setup OK");
		//configure for use here
		writeRegister(CAP1296_MTCONFIG, 0);		//allow multiple touches
		writeRegister(CAP1296_SENSITIVITY, 0x5F);   //reduce sensitivity to 8x (from default 32x)
		writeRegister(CAP1296_RPTRATEENABLE, 0x00);   //disable repeat interrupts on all pins (this happens when you hold the pin, useful for volume/skipping)
		//writeRegister(CAP1296_CONFIG2, readRegister(CAP1296_CONFIG2) & ~CAP1296_CONFIG2_BCOUTRECAL);		//disable base count out-of-limit function
		writeRegister(CAP1296_CALIBRATE, 0xFF);  //force calibration
		return true;
	}
}

boolean CAP1296::touchStatusChanged(void){
	boolean result=readRegister(CAP1296_MAIN) & CAP1296_MAIN_INT;
	writeRegister(CAP1296_MAIN, readRegister(CAP1296_MAIN) & ~CAP1296_MAIN_INT); //clear main interrupt
	return result;
}

void CAP1296::updateTouchData(){
	uint8_t touchData=readRegister(CAP1296_SENSTATUS);
	_newTouches=  (_touchData ^ touchData) & touchData;
	_newReleases= (_touchData ^ touchData) & _touchData;
	_touchData=touchData;
}

boolean CAP1296::isNewTouch(uint8_t cs){
	return (1<<(cs))&_newTouches;
}

boolean CAP1296::isNewRelease(uint8_t cs){
	return (1<<(cs))&_newReleases;
}

uint8_t CAP1296::getTouchData(void){
	return _touchData;
}
uint8_t CAP1296::getNewTouches(void){
	return _newTouches;
}
uint8_t CAP1296::getNewReleases(void){
	return _newReleases;
}

void CAP1296::setSensitivity(uint8_t value){
	
}

uint8_t CAP1296::readRegister(uint8_t reg){
	Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_i2caddr, 1);
    return Wire.read();
}

void CAP1296::writeRegister(uint8_t reg,uint8_t value){
	Wire.beginTransmission(_i2caddr);
    Wire.write((uint8_t) reg);
    Wire.write((uint8_t) value);
    Wire.endTransmission();
}