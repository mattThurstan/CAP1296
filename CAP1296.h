/*
*Arduino library for Microchip capacitive touch IC - CAP1296
*Uses I2C for communication.
*
*Based on CAP1298 library by Chris Ball
*https://github.com/ChrisBall/CAP1298
*/

/*
*Not intended to be a full implementation, just one suitable for general purpose and for my exact application
*May work for other CAP1XXX chips with I2C only, no LED support
*/

#ifndef CAP1296_h
#define CAP1296_h
#include "Arduino.h"
#include <Wire.h>

#define CAP1296_ADDR 				0x50 		//default address for all CAP1296 chips  ..28 ???  '0101 000' write, '0101 0001' read

/* Register set in hexadecimal order */
#define CAP1296_MAIN 				0x00      	//Main Control
#define CAP1296_GENSTAT 			0x02		//General Status
#define CAP1296_SENSTATUS 			0x03		//Sensor Input Status (where touches on the inputs are stored)
#define CAP1296_NOISEFLAG 			0x0A		//Noise Flag Status
#define CAP1296_SENSINPUTDELTACOUNT1 0X10		//Sensor Input 1 Delta Count
#define CAP1296_SENSINPUTDELTACOUNT2 0X11		//Sensor Input 2 Delta Count
#define CAP1296_SENSINPUTDELTACOUNT3 0X12		//Sensor Input 3 Delta Count
#define CAP1296_SENSINPUTDELTACOUNT4 0X13		//Sensor Input 4 Delta Count
#define CAP1296_SENSINPUTDELTACOUNT5 0X14		//Sensor Input 5 Delta Count
#define CAP1296_SENSINPUTDELTACOUNT6 0X15		//Sensor Input 6 Delta Count
#define CAP1296_SENSITIVITY			0x1F		//Sensitivity Control
#define CAP1296_CONFIG1 			0x20		//Configuration 1 (General) (Configuration 2 is 0x44)	
#define CAP1296_SENSINPUTENABLE		0x21		//Sensor Input Enable
#define CAP1296_SENSINPUTCONFIG1	0x22		//Sensor Input Configuration 1
#define CAP1296_SENSINPUTCONFIG2	0x23		//Sensor Input Configuration 2
#define CAP1296_AVGSAMPCONFIG		0x24		//Averaging and Sampling Configuration
#define CAP1296_CALIBRATE 			0x26		//Calibration Activate and Status
#define CAP1296_INTENABLE 			0x27		//Interrupt Enable
#define CAP1296_RPTRATEENABLE 		0x28		//Repeat Rate Enable
#define CAP1296_SGENABLE 			0x29		//Signal Guard Enable
#define CAP1296_MTCONFIG 			0x2A		//Multiple Touch Configuration
#define CAP1296_MTPATTERNCONFIG		0x2B		//Multiple Touch Pattern Configuration
#define CAP1296_MTPATTERN			0x2D		//Multiple Touch Pattern
#define CAP1296_BASECOUNTOUTOFLIMIT 0x2E		//Base Count Out of Limit
#define CAP1296_RECALCONFIG			0x2F		//Recalibration Configuration
#define CAP1296_SENSINPUTTHRES1		0x30		//Sensor Input 1 Threshold
#define CAP1296_SENSINPUTTHRES2		0x31		//Sensor Input 2 Threshold
#define CAP1296_SENSINPUTTHRES3		0x32		//Sensor Input 3 Threshold
#define CAP1296_SENSINPUTTHRES4		0x33		//Sensor Input 4 Threshold
#define CAP1296_SENSINPUTTHRES5		0x34		//Sensor Input 5 Threshold
#define CAP1296_SENSINPUTTHRES6		0x35		//Sensor Input 6 Threshold
#define CAP1296_SENSINPUTNOISTHRES	0x38		//Sensor Input Noise Threshold
//Standby configuration registers
#define CAP1296_STANDBYCHNL			0x40		//Standby Channel
#define CAP1296_STANDBYCFG 			0x41		//Standby Configuration
#define CAP1296_STANDBYSENS			0x42		//Standby Sensitivity
#define CAP1296_STANDBYTHRES		0x43		//Standby Threshold
#define CAP1296_CONFIG2 			0x44		//Configuration 2 (Configuration 1 is 0x20)
//Base count registers
#define CAP1296_SENSINPUTBASECOUNT1 0x50		//Sensor Input 1 Base Count
#define CAP1296_SENSINPUTBASECOUNT2 0x51		//Sensor Input 2 Base Count
#define CAP1296_SENSINPUTBASECOUNT3 0x52		//Sensor Input 3 Base Count
#define CAP1296_SENSINPUTBASECOUNT4 0x53		//Sensor Input 4 Base Count
#define CAP1296_SENSINPUTBASECOUNT5 0x54		//Sensor Input 5 Base Count
#define CAP1296_SENSINPUTBASECOUNT6 0x55		//Sensor Input 6 Base Count
//Power button registers
#define CAP1296_PWRBT				0x60		//Power Button
#define CAP1296_PWRBTCONFIG			0x61		//Power Button Configuration
//Calibration sensitivity configuration registers
#define CAP1296_CALIBSENSCONFIG1	0x80		//Calibration Sensitivity Configuration 1
#define CAP1296_CALIBSENSCONFIG2	0x81		//Calibration Sensitivity Configuration 2
//Calibration registers
#define CAP1296_SENSINPUTCALIB1		0xB1		//Sensor Input 1 Calibration
#define CAP1296_SENSINPUTCALIB2		0xB2		//Sensor Input 2 Calibration
#define CAP1296_SENSINPUTCALIB3		0xB3		//Sensor Input 3 Calibration
#define CAP1296_SENSINPUTCALIB4		0xB4		//Sensor Input 4 Calibration
#define CAP1296_SENSINPUTCALIB5		0xB5		//Sensor Input 5 Calibration
#define CAP1296_SENSINPUTCALIB6		0xB6		//Sensor Input 6 Calibration
#define CAP1296_SENSINPUTCALIBLSB1	0xB9		//Sensor Input Calibration LSB 1
#define CAP1296_SENSINPUTCALIBLSB2	0xBA		//Sensor Input Calibration LSB 2
//ID registers
#define CAP1296_PRODID 				0xFD		//Product ID (default 0x69)
#define CAP1296_MANUID 				0xFE		//Manufacturer ID (default 0x5D)
#define CAP1296_REV 				0xFF		//Revision

//..bits to set
#define CAP1296_MAIN_INT 			0x01  		//Interrupt bit (default 0)
#define CAP1296_MAIN_COMBO 			0x02		//Combo mode bit  (default 0)
#define CAP1296_MAIN_CGAIN0 		0x04		//Combo mode gain setting bit 1 (default 0)
#define CAP1296_MAIN_CGAIN1 		0x08		//Combo mode gain setting bit 2 (default 0)
#define CAP1296_MAIN_DSLEEP			0x10		//Deep sleep mode bit (default 0)
#define CAP1296_MAIN_STBY			0x20		//Standby mode bit
#define CAP1296_MAIN_GAIN0 			0x40		//Active mode gain setting bit 1 (default 0)
#define CAP1296_MAIN_GAIN1 			0x80		//Active mode gain setting bit 2 (default 0)
#define CAP1296_GENSTAT_ACALFAIL 	0x20		//Calibration failure bit
#define CAP1296_CONFIG1_TIMEOUT		0x80		//Timeout config bit (default 0)
#define CAP1296_CONFIG1_DISDIGNOISE	0x20		//Disable digital noise threshold (default 1 (disabled))
#define CAP1296_CONFIG1_DISANANOISE	0x10		//Disable analog noise threshold (default 0 (enabled))
#define CAP1296_CONFIG1_MAXDUREN 	0x08		//Enable Max duration recalibration
#define CAP1296_CONFIG2_BCOUTRECAL 	0x40		//Base count out-of-limit recalibration bit (default 1)			I THINK I SHOULD TURN THIS OFF FOR MOST USERS (BARE CONTACTS CAUSE OOL)
#define CAP1296_CONFIG2_BLKPWRCONT 	0x20		//Disable power control while sensing (default 0)

class CAP1296 {
    
  public:
	CAP1296(void);
	boolean begin(uint8_t i2caddr = CAP1296_ADDR);
	boolean touchStatusChanged(void);
	void updateTouchData(void);
	boolean isNewTouch(uint8_t cs);
	boolean isNewRelease(uint8_t cs);
	uint8_t getTouchData(void);
	uint8_t getNewTouches(void);
	uint8_t getNewReleases(void);
	void setSensitivity(uint8_t value);
	uint8_t readRegister(uint8_t reg);
	void writeRegister(uint8_t reg, uint8_t value);
	
  private:
	int8_t _i2caddr;
	uint8_t _touchData,_newTouches,_newReleases;
};

#endif