

#include <NMEAGPS.h>
#include <SPI.h>
#include <nRF24L01p.h>


NMEAGPS gps;
nRF24L01p transmitter(7, 8); //CSN,CE

int updates;
int failedUpdates;
int pos;
int stringplace = 0;
String nmea[15];
String labels[15] {"Time: ", "Latitude: ", "N/S: ", "Longitude: ", "E/W : ", "GPS quality indicator : ", "# of sat. : ", "Horizontal dilution : ", "altitude  : ", "", "WGS84 :", "", "", "diff. ref station ID: "};
/*1    = UTC of Position
  2    = Latitude
  3    = N or S
  4    = Longitude
  5    = E or W
  6    = GPS quality indicator (0=invalid; 1=GPS fix; 2=Diff. GPS fix)
  7    = Number of satellites in use [not those in view]
  8    = Horizontal dilution of position
  9    = Antenna altitude above/below mean sea level (geoid)
  10   = Meters  (Antenna height unit)
  11   = Geoidal separation (Diff. between WGS-84 earth ellipsoid and
       mean sea level.  -=geoid is below WGS-84 ellipsoid)
  12   = Meters  (Units of geoidal separation)
  13   = Age in seconds since last update from diff. reference station
  14   = Diff. reference station ID#
  15   = Checksum
*/

String timeUp;
void setup() {
  delay(2000);
  Serial.begin(9600);
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  
  transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
  transmitter.TXaddress("ALL"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  transmitter.init();

}
String message;
/*
  void printDouble(double val, unsigned int precision){
  Serial.print(int(val));
  Serial.print(".");
  unsigned int frac;
  if(val=0)
    frac = (val-int(val))*pricision;
  else
    frac = (int(val)-val)*precision;
  int frac1 = frac;
  while(frac1 /= 10)
    precision /= 10;
  precision /= 10;
  while([precision/=10)
    Serial.print("0");
  Serial.println(frac,DEC);
  }
*/
void loop() {
  delay(2000);
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
  else {
    failedUpdates++;
  }
  stringplace = 0;
  pos = 0;

}


