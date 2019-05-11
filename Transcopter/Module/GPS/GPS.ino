#include <NMEAGPS.h>
#include "SoftwareSerial.h"
#include <SPI.h>
#include <nRF24L01p.h>

SoftwareSerial GPSModule(1,0); // TX, RX
NMEAGPS gps;
nRF24L01p transmitter(7, 6); //CSN,CE

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
  delay(1000);  
  Serial.begin(9600);
  GPSModule.begin(9600);
}
String message;

void loop() {
  Serial.flush();
  GPSModule.flush();
  while (GPSModule.available() > 0) {
    GPSModule.read();
  }
  if (GPSModule.find("GPGGA,")) {
    String tempMsg = GPSModule.readStringUntil('\n');
    for (int i = 0; i < tempMsg.length(); i++) {
      if (tempMsg.substring(i, i + 1) == ",") {
        Serial.println("count: " + i);
        nmea[pos] = tempMsg.substring(stringplace, i);
        stringplace = i + 1;
        pos++;
      }
      if (i == tempMsg.length() - 1) {
        nmea[pos] = tempMsg.substring(stringplace, i);
      }
    }
    updates++;

    nmea[1] = ConvertLat();
    nmea[3] = ConvertLng();

    //Serial.print(nmea[1],DEC);
    
    Serial.print(nmea[1]);
    Serial.print(",");
    Serial.print(nmea[3]);
    Serial.print(",");
    Serial.print(nmea[8]);
    Serial.println("");



    /*
      for(int i = 1 ; i<15;i++){
      if(i!=1&&i!=9&&i!=11&&i!=12)   // จัดระเบียบ
        Serial.println("");
      Serial.print(labels[i]);
      Serial.print(nmea[i]);
      }
      Serial.println(""); // ทำให้ console scoll กลับมาตน.เดิมตอนอัพเดทค่าใหม่
      Serial.println(""); //
      Serial.println(""); //
      Serial.println(""); //
      Serial.println(""); //
    */
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
  transmitter.TXaddress("ALL"); // ตั้งชื่อตำแหน่งให้ตรงกัน ชื่อตั้งได้สูงสุด 5 ตัวอักษร
  transmitter.init();

    transmitter.txPL(nmea[1]+", "+nmea[3]+", "+nmea[8]); // ค่าที่ต้องการส่ง
    transmitter.send(FAST); // สั่งให้ส่งออกไป
    
  }
  else {
    failedUpdates++;
  }
  stringplace = 0;
  pos = 0;

}


String ConvertLat() {
  String posneg = "";
  if (nmea[1] == "S") {
    posneg = "-";
  }
  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[1].length(); i++) {
    if (nmea[1].substring(i, i + 1) == ".") {
      latfirst = nmea[1].substring(0, i - 2);
      latsecond = nmea[1].substring(i - 2).toFloat();
    }
  }
  latsecond = latsecond / 60;
  String CalcLat = "";

  char charVal[9];
  dtostrf(latsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLat += charVal[i];
  }
  latfirst += CalcLat.substring(1);
  latfirst = posneg += latfirst;
  return latfirst;
}

String ConvertLng() {
  String posneg = "";
  if (nmea[3] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[3].length(); i++) {
    if (nmea[3].substring(i, i + 1) == ".") {
      lngfirst = nmea[3].substring(0, i - 2);
      //Serial.println(lngfirst);
      lngsecond = nmea[3].substring(i - 2).toFloat();
      //Serial.println(lngsecond);

    }
  }
  lngsecond = lngsecond / 60;
  String CalcLng = "";
  char charVal[9];
  dtostrf(lngsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;
}

