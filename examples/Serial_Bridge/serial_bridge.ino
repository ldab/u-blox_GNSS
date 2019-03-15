/******************************************************************************
u-blox_GNSS.h
u-blox GNSS Arduino
Leonardo Bispo
Mar 03, 2019
https://github.com/ldab/u-blox_GNSS

Distributed as-is; no warranty is given.
******************************************************************************/

// use Software Serial on Uno, Nano, ESP8266
#if defined(ESP8266)
  #include <SoftwareSerial.h>
  SoftwareSerial Serial_GNSS(4, 5); // RX, TX

// Use Hardware Serial on Mega, Leonardo, Micro, MKR
#else
  #define Serial_GNSS Serial1
#endif

void setup() {

  Serial.begin( 9600 );

  Serial_GNSS.begin( 9600 );

}

void loop()
{
  // If anything comes in Serial (USB),
  if (Serial.available()) 
  {
    Serial_GNSS.write(Serial.read());   // read it and send it out Serial_GNSS
  }

  // If anything comes in Serial_GNSS
  if (Serial_GNSS.available()) 
  {
    Serial.write(Serial_GNSS.read());   // read it and send it out Serial (USB)
  }

}