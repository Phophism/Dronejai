#include <NMEAGPS.h>
#include <SPI.h>
#include <nRF24L01p.h>
#include <Wire.h>

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

const unsigned char OSS = 0;  // Oversampling Setting

// Calibration values
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;
// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 

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

  Serial.begin(9600);
  Wire.begin();
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  
  transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
  transmitter.TXaddress("ALL"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  transmitter.init();
  
 
}
String message;

void loop() {
   float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
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

  Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");

  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" M");

  Serial.println();//line break

  delay(1000); //wait a second and get values again.
  
  stringplace = 0;
  pos = 0;

}


