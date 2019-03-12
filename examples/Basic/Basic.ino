/******************************************************************************
u-blox_GNSS.h
u-blox GNSS Arduino
Leonardo Bispo
Mar 03, 2019
https://github.com/ldab/u-blox_GNSS

Distributed as-is; no warranty is given.
******************************************************************************/

"ublox_GNSS.h"

uint16_t errorsAndWarnings = 0;

LIS3DH myIMU(0x19); //Default address is 0x19.

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000); //wait until serial is open...
  
  if( myIMU.begin() != 0 )
  {
    Serial.print("Error at begin().\n");
  }
  else
  {
    Serial.print("\nbegin() passed.\n");
  }
  
//Setup the accelerometer******************************
  uint8_t dataToWrite = 0; //Start Fresh!
  dataToWrite |= 0x4 << 4; //ODR of 50Hz
  dataToWrite |= 0x7; //Enable all axes

  //Now, write the patched together data
  errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG1, dataToWrite);

  dataToWrite = 0x80;
  errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG4, dataToWrite);

  dataToWrite = 0x80; //ADC enable
  errorsAndWarnings += myIMU.writeRegister(LIS3DH_TEMP_CFG_REG, dataToWrite);

  

//  //Test interrupt configuration profile on int1
//  {
//	dataToWrite = 0x40; //INT1 src
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG3, dataToWrite);
//	dataToWrite = 0x08; //latch output int
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);
//	dataToWrite = 0x40; //
//	//errorsAndWarnings += myIMU.writeRegister(LIS3DH_REFERENCE, dataToWrite);
//	dataToWrite = 0x0A; //High X and high Y only
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_CFG, dataToWrite);
//	dataToWrite = 0x3F; // half amplitude?
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);
//	dataToWrite = 0x01; //duration?
//	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);
//  }

  
  //Test interrupt configuration profile on int2
  {
	dataToWrite = 0x40; //INT2 src
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG6, dataToWrite);
	dataToWrite = 0x08; //latch output int
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_CTRL_REG5, dataToWrite);
	dataToWrite = 0x40; //
	//errorsAndWarnings += myIMU.writeRegister(LIS3DH_REFERENCE, dataToWrite);
	dataToWrite = 0x0A; //High X and high Y only
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_CFG, dataToWrite);
	dataToWrite = 0x3F; // half amplitude?
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_THS, dataToWrite);
	dataToWrite = 0x01; //duration?
	errorsAndWarnings += myIMU.writeRegister(LIS3DH_INT1_DURATION, dataToWrite);
  }

  //Get the ID:
  uint8_t readData = 0;
  Serial.print("\nReading LIS3DH_WHO_AM_I: 0x");
  myIMU.readRegister(&readData, LIS3DH_WHO_AM_I);
  Serial.println(readData, HEX);

}


void loop()
{
  float accel;
  uint8_t readData = 0;

  Serial.print(" Acceleration = ");
  //Read a register into the Acceleration variable.
  if( myIMU.readRegister( &readData, LIS3DH_OUT_X_L ) != 0 )
  {
    errorsAndWarnings++;
  }
  
  Serial.println();
  Serial.print("Total reported Errors and Warnings: ");
  Serial.println(errorsAndWarnings);

  delay(1000);
}