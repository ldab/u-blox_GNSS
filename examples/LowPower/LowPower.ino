/******************************************************************************
u-blox_GNSS.h
u-blox GNSS Arduino
Leonardo Bispo
Mar 03, 2019
https://github.com/ldab/u-blox_GNSS

Distributed as-is; no warranty is given.
******************************************************************************/

// Enable Serial debbug on Serial UART to see registers wrote
#define GNSS_DEBUG Serial

// use Software Serial on Uno, Nano, ESP8266
#if defined(ESP8266)
  #include <SoftwareSerial.h>
  SoftwareSerial Serial_GNSS(2, 3); // RX, TX

// Use Hardware Serial on Mega, Leonardo, Micro, MKR
#else
  #define Serial_GNSS Serial1
#endif

#include "ublox_GNSS.h"

float lat, lon, acc;

fixType_t fix = NO_FIX;

GNSS gnss( Serial_GNSS );

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial_GNSS.begin( 9600 );

  // Alternativelly 1Hz power mode can be used
  // if( gnss.begin( PSM_1HZ ) )
  if( gnss.begin( ON_OFF, 280000, 10000) )
  {
    Serial.println("Failed to initialize GNSS module.");
  }

}

void loop()
{
  // Get coordinates with minimum 50m accuracy;

  gnss.getCoodinates(lon, lat, fix, acc, 50000);

  Serial.println("Here you are, lon:" + String(lon) +" lat:" + String(lat));
  Serial.println("calculated error: " + String(acc));
  
  Serial.println("Or try the following link to see on google maps:");
  Serial.print(String("https://www.google.com/maps/search/?api=1&query=") + lat + "," + lon);

  delay(10000);

}