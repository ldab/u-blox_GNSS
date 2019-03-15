/******************************************************************************
u-blox_GNSS.h
u-blox GNSS Arduino
Leonardo Bispo
Mar 03, 2019
https://github.com/ldab/u-blox_GNSS

Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __ublox_GNSS__
#define __ublox_GNSS__

#include "stdint.h"

#if defined(ARDUINO)
	#if ARDUINO >= 100
    #include "Arduino.h"
		#include "SoftwareSerial.h"
	#else
    #include "WProgram.h"
	#endif
#endif

#ifdef GNSS_DEBUG
	namespace {
  		template<typename T>
  		static void DBG(T last) {
    	GNSS_DEBUG.println(last);
		}
		
		template<typename T, typename... Args>
		static void DBG(T head, Args... tail) {
			GNSS_DEBUG.print(head);
			GNSS_DEBUG.print(' ');
			DBG(tail...);
		}
	}
#else
  	#define DBG(...)
#endif

// Power Save Mode
typedef enum
{
	CONTINOUS,
	PSM_1HZ,
	ON_OFF
	//...
} psmMode_t;

// FIX Flags
typedef enum
{
	NO_FIX,
	DEAD_RECKONING,
	FIX_2D,
	FIX_3D,
	GNSS_AND_DEAD_RECKONING,
	TIME_ONLY
	//...
} fixType_t;

class GNSS
{
public:
	// Assign serial interface to be used with the module

	GNSS( SoftwareSerial& ss );
	
	// Empty begin() starts module on default, non-power saving mode, or PSM_1HZ.
	bool init( psmMode_t m = CONTINOUS );

	// Can also enter On, Off opperation by specifying parameters.
	bool init( psmMode_t m, uint32_t sleep, uint32_t onTime );

	// Send message and check Acknoledge message
	bool sendUBX( byte *msg, uint32_t size);

	// UBX-NAV-PVT -> Get coordinates, fix and accuracy;
	bool getCoodinates( float &lon, float &lat, fixType_t &fix, float &acc, float acc_min = 50 );

	// UBX-RXM-PMREQ -> put the module in backup forever (off) wake by RX
	void off( void );

	// UBX-CFG-CFG -> Reset module to factory settings;
	void factoryRST( void );

	// UBX-CFG-CFG -> Save Configuration to the non-volatile message
	bool saveCFG( void );

private:

	#define UART_TIMEOUT 	1000
	#define FIX_TIMEOUT		60000

	bool gnss_init = false;

	HardwareSerial* stream;
	SoftwareSerial*	s_stream;
	// Read Serial RX buffer just to clean it
	void clearUART( void );

	// Convert 32 bits variable to 4x char
	void bits_char( uint32_t c, uint8_t *_c );

	// UBX Checksum
	bool crc( byte *msg, uint32_t size);

	byte _saveCFG[21] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                  		0x00, 0x00, 0x00, 0x07, 0x21, 0xAF};

};

#endif
