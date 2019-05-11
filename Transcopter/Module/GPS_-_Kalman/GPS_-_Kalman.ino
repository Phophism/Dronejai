#include <NMEAGPS.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01p.h>

SoftwareSerial GPSModule(10,11); // TX, RX
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


// initialize Kalman variables
//float R =  1.8971861507799e-11, Q = 1.8971861507799e-11;  //Q = process noise covariance, R = measurement noise covariance
//double Xpe0 = 0.0;  // Xpe0 = prior estimation of signal X at time t=0 (current state)
//double Xe1 = 0.0;  //Xe1 = estimation of X at time t=1 (previous state)
//double Ppe0 = 0.0;  //Ppe0 = prior estimation of "error covariance" at t=0,  
//double P1 = 1, P0 = 0; //P1 = error covariance at t=1, P0 = error covariance at t=0
//double  K = 0.0, Xe0 = 0.0, Z = 0.0; //K = Kalman gain, Xe0 = estimation of signal at t=0, Z = measured signal at t=0


void setup() {
  delay(1000);  
  Serial.begin(9600);
  GPSModule.begin(9600);
}

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
        nmea[pos] = tempMsg.substring(stringplace, i);
        stringplace = i + 1;
        pos++;
      }
      if (i == tempMsg.length() - 1) {
        nmea[pos] = tempMsg.substring(stringplace, i);
      }
    }
    nmea[1] = ConvertLat();
    nmea[3] = ConvertLng();
    
//    Xpe0 = Xe1; //Assumption or prediction 1
//    Ppe0 = P1 + Q; //Assumption or prediction 2
//    K = Ppe0/(Ppe0 + R); // Measurement update or correction of "Kalman gain"
//    Xe0 = Xpe0 + K * ( nmea[1].toFloat() - Xpe0); // Measurement update or correction of "estimated signal"
//    P0 = (1 - K) * Ppe0; // Measurement update or correction of "error covariance"
//    
//    Xe1 = Xe0; //Update: current t=0 becomes t=1 in the next step
//    P1 = P0; //Update: current t=0 becomes t=1 in the next step

    nmea[1]=nmea[1].toFloat();

    Serial.print(nmea[1]);
    Serial.print(",");
//    Serial.print(Xe0 * 10,4);
//    Serial.println("");
 
    
//    Serial.print(nmea[1]);
//    Serial.print(",");
//    Serial.print(nmea[3]);
//    Serial.print(",");
//    Serial.print(nmea[8]);
//    Serial.println("");
  }
  else {
    Serial.println("Can't find GPS signal");
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

