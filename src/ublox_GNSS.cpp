/******************************************************************************
u-blox_GNSS.h
u-blox GNSS Arduino
Leonardo Bispo
Mar 03, 2019
https://github.com/ldab/u-blox_GNSS

Distributed as-is; no warranty is given.
******************************************************************************/

#include "ublox_GNSS.h"
#include "stdint.h"

void GNSS::clearUART( void )
{
  while( stream.available() >0 )
    stream.read();
}

void GNSS::bits_char( uint32_t c, uint8_t *_c )
{
  for( int i = 0; i < 3; )
  {
    _c[i] = ( c >> i ) & 0xFF;
    i++;
  }
}

bool GNSS::begin( psmMode_t m )
{
	stream.write(0xFF);          // Send something to wake GNSS
  while( !stream.available() ); // alow some time to turn GNSS ON.

  // Clear NMEA messages
  clearUART();

  // Disable NMEA, UBX-CFG-PRT -> Enable UBX over UART1 and Baud rate 9600
  byte message[] = {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00,
                    0x80, 0x25, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79};

  DBG("UBX-CFG-PRT -> Disable NMEA and enable UBX");

  bool ret = sendUBX(message, sizeof(message));

  // Clear NMEA messages
  clearUART();

  if( m == PSM_1HZ )
  {
    DBG("UBX-CFG-PSM -> Power save mode");
    byte confmessage[] = {0xB5, 0x62, 0x06, 0x86, 0x08, 0x00, 0x00, 0x02, 0x18, 0x01, 0x05, 0x00, 0x00, 0x00,
                          0xB4, 0x11};

    ret &= sendUBX( confmessage, sizeof(confmessage) );
  }

  DBG("UBX-CFG-CFG -> Saving configuration");

  ret &= saveCFG();

	return ret;
}

bool GNSS::begin( psmMode_t m, uint32_t sleep, uint32_t onTime )
{
  if( m != ON_OFF)
    return false;

  begin();

  //uint8_t _sleep[4] = { sleep, sleep >> 8, sleep >> 16, sleep >> 24 };
  uint8_t _sleep[4] = {};
  bits_char(sleep, _sleep);
  
  uint8_t _ontime[4] = {};
  bits_char(onTime, _ontime);

  byte onoff[56] = {0xB5, 0x62, 0x06, 0x3B, 0x30, 0x00, 0x02, 0x06, 0x00, 0x00, 0x00, 0x90, 0x40, 0x01, _sleep[3], _sleep[2],
                  _sleep[1], _sleep[0], _ontime[3], _ontime[2], _ontime[1], _ontime[0], 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 
                  0x78, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
                  0x64, 0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};

  uint8_t CK_A = 0, CK_B = 0;
  for( uint8_t i = 2; i < sizeof(onoff) ; )
  {
    CK_A = CK_A + onoff[i];
    CK_B = CK_B + CK_A;
    i++;
  }

  onoff[54] = CK_A;
  onoff[55] = CK_B;

  DBG("UBX-CFG-PS2 -> Power save mode ON/OFF");
  
  bool ret = sendUBX( onoff, sizeof(onoff));

  DBG("UBX-CFG-CFG -> Saving configuration");

  ret &= saveCFG();

	return ret;
}

bool GNSS::sendUBX( byte *msg, uint8_t size )
{
  stream.write( msg, sizeof(msg));

  byte *_msg;
  int i = 0;

  while( stream.available() > 0)
  {
    _msg[i] = stream.read();
    i++;
  }
  
  if( _msg[3] == 2 )
  {
    // Message Not-Acknowledged
    DBG("Message Not-Acknowledged");
    return false;
  }

  return crc( msg , sizeof(msg) );

}

bool GNSS::getCoodinates( float &lon, float &lat, fixType_t &fix, float &acc, float acc_min )
{
  byte nav_pvt[] = {0xB5, 0x62, 0x01, 0x07};

  stream.write(0xFF);          // Send something to wake GNSS
  while( !stream.available() ); // alow some time to turn GNSS ON.

  // Clear NMEA messages
  clearUART();

  DBG("UBX-NAV-PVT -> Pooling coordinates")

  sendUBX( nav_pvt, sizeof(nav_pvt) );

  uint8_t i = 0;
  byte *res;

  while( stream.available() > 0)
  {

      res[i] = stream.read();
      i++;

  }

  lon = ( (res[27] << 24) + (res[26] << 16) + (res[25] << 8) + res[24] ) / 1e-7;
  lat = ( (res[31] << 24) + (res[30] << 16) + (res[29] << 8) + res[28] ) / 1e-7;
  fix = (fixType_t)res[20];
  acc = ( (res[43] << 24) + (res[42] << 16) + (res[41] << 8) + res[40] ) / 1000;

  DBG("lon: ", lon, "lat: ", lat, " - ", fix, " ~", float(acc/1000.0f));

  return crc( res, sizeof(res) );
}

bool GNSS::crc( byte *msg, uint8_t size)
{
  uint8_t s = size;
  uint8_t CK_A = 0, CK_B = 0;

  for( uint8_t i = 1; i < (s-2) ; i++)
  {
    CK_A = CK_A + msg[i];
    CK_B = CK_B + CK_A;
  }

  return ( CK_A == msg[s-1] && CK_B == msg[s]);

}

bool GNSS::saveCFG()
{
  return sendUBX( _saveCFG, sizeof(_saveCFG));
}
