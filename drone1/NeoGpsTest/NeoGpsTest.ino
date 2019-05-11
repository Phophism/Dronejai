#include <NMEAGPS.h>
#include <SPI.h>
#include <nRF24L01p.h>

NMEAGPS gps;
nRF24L01p transmitter(7, 8); //CSN,CE


void setup()
{

  delay(2000);
  
  Serial.begin(9600);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);

  transmitter.channel(90);
  transmitter.TXaddress("ALL"); 
  transmitter.init();
}

void loop()
{
  if (gps.available( Serial )) { // parse received characters

    gps_fix fix = gps.read();    // get the new fix structure

    //  Format some of the fix pieces into a char array

    char  message[ 32 ];         // storage for formatted text message
    char *ptr = &message[0];     // start putting chars at the beginning

    dtostrf( fix.latitude(), 1, 6, ptr );   // write the floating-point latitude into the array
    ptr += strlen(ptr);          // step to end of latitude characters
    
    *ptr++ += ',';               // append a comma
    
    dtostrf( fix.longitude(), 1, 6, ptr );
    ptr += strlen(ptr);
    
    *ptr++ += ',';

    itoa( fix.satellites, ptr, 10 ); // append the satellite count

    //  Message is ready
  
    Serial.println( message );  // see what we are sending

    transmitter.txPL( message ); // send the message
    transmitter.send(FAST); 
  }
}
