#include "TinyGPS.h" ;

String inputMessage[3]; // A string to hold incoming data
boolean IsMessageReady = false; // Whether the string is complete
String nmea[3][15]; // GPS Data Set 1st[] -> device ID , 2nd[]Dataset
int pos; // Next element
int stringplace = 0 ; // Next char
String tempMsg[3] ;

double resultLat[3] ; 
double resultLng[3] ; 
double resultAlt[3] ;

static const unsigned long loopInterval = 2000; // refresh every 2 second
static unsigned long lastRefreshTime = 0; 

void setup(){  
  Serial.begin(9600);
 // Serial.println("Initializing GPS");
  Serial1.begin(9600);  // Connect to the GPS module
  Serial2.begin(9600);
  Serial3.begin(9600);
  delay(30000);
}

void loop(){

  unsigned long currentMillis = millis();

  if(currentMillis - lastRefreshTime >= loopInterval){
    Serial.print(millis()); // Time when loop start
    Serial.print(",  ");
    Serial1.read();
    Serial2.read();  
    Serial3.read();  
  // --------------- Read GPS 1 --------------- //

    if (Serial1.find("GNGGA,")) {      
      tempMsg[0] = Serial1.readStringUntil('\n'); // Collect the data set from GNGGA    
      // Collecting Data from GPS#1
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[0].length(); i++) {   
        if (tempMsg[0].substring(i, i + 1) == ",") {
          if(pos==1 || pos==3 ||pos==8){
            nmea[0][pos] = tempMsg[0].substring(stringplace, i);
          }
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[0].length() - 1) {
          nmea[0][pos] = tempMsg[0].substring(stringplace, i);
        }
      }
    }
    if (Serial2.find("GPGGA,")) {     
      tempMsg[1] = Serial2.readStringUntil('\n'); // Collect the data set from GNGGA     
      // Collecting Data from GPS#2
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[1].length(); i++) {
        if (tempMsg[1].substring(i, i + 1) == ",") {
          if(pos==1 || pos==3 ||pos==8) {    
            nmea[1][pos] = tempMsg[1].substring(stringplace, i);
          }
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[1].length() - 1) {
          nmea[1][pos] = tempMsg[1].substring(stringplace, i);
        }
      }
    }
    if (Serial3.find("GPGGA,")) {     
      tempMsg[2] = Serial3.readStringUntil('\n'); // Collect the data set from GNGGA     
      // Collecting Data from GPS#3
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[2].length(); i++) {
        if (tempMsg[2].substring(i, i + 1) == ",") {
          if(pos==1 || pos==3 ||pos==8){
            nmea[2][pos] = tempMsg[2].substring(stringplace, i);
          }
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[2].length() - 1) {
          nmea[2][pos] = tempMsg[2].substring(stringplace, i);
        }
      }
    }
    for(int n=0 ; n<3 ; n++){
      nmea[n][1] = ConvertLat(n);
      nmea[n][3] = ConvertLng(n);
    }
  
    for(int n=0 ; n<3 ; n++){
      Serial.print(nmea[n][1]);
      Serial.print(",");
      Serial.print(nmea[n][3]);
      Serial.print(",");
      Serial.print(nmea[n][8]);
      Serial.print(", ");
    }
    Serial.println("");
    lastRefreshTime = currentMillis;
  }
}

String ConvertLat(int deviceNum) {
  String posneg = "";
  if (nmea[deviceNum][1] == "S") {
    posneg = "-";
  }
  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[deviceNum][1].length(); i++) {
    if (nmea[deviceNum][1].substring(i, i + 1) == ".") {
      latfirst = nmea[deviceNum][1].substring(0, i - 2);
      latsecond = nmea[deviceNum][1].substring(i - 2).toFloat();
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

String ConvertLng(int deviceNum) {
  String posneg = "";
  if (nmea[deviceNum][3] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[deviceNum][3].length(); i++) {
    if (nmea[deviceNum][3].substring(i, i + 1) == ".") {
      lngfirst = nmea[deviceNum][3].substring(0, i - 2);
      //Serial.println(lngfirst);
      lngsecond = nmea[deviceNum][3].substring(i - 2).toFloat();
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
