// V2 Update
// Update R (Process noise) measured by SD^2


#include "TinyGPS.h" ;


// ----------------------- Global variables ---------------------------//

String inputMessage[3]; // A string to hold incoming data
boolean IsMessageReady = false; // Whether the string is complete
String nmea[3][15]; // GPS Data Set 1st[] -> device ID , 2nd[]Dataset
int pos; // Next element
int stringplace = 0 ; // Next char
String tempMsg[3] ;

double resultLat[3] ; 
double resultLng[3] ; 
double resultAlt[3] ;

// -------------------------- Time Watcher --------------------------//

static const unsigned long loopInterval = 2000; // refresh every 2 second
static unsigned long lastRefreshTime = 0; 

// ------------------------- Setup -------------------------- // 

void setup(){  
  Serial.begin(9600);
 // Serial.println("Initializing GPS");
  Serial1.begin(9600);  // Connect to the GPS module
  Serial2.begin(9600);
  Serial3.begin(9600);
  delay(30000);
  // --------------- Read GPS 1 --------------- //
  Serial1.read();
  if (Serial1.find("GNGGA,")) {      
    tempMsg[0] = Serial1.readStringUntil('\n'); // Collect the data set from GNGGA    
    // Collecting Data from GPS#1
    stringplace = 0; // reset char count
    pos = 0; // reset element count
    for (int i = 0; i < tempMsg[0].length(); i++) {   
      if (tempMsg[0].substring(i, i + 1) == ",") {
        nmea[0][pos] = tempMsg[0].substring(stringplace, i);
        stringplace = i + 1;
        pos++;
      }
      if (i == tempMsg[0].length() - 1) {
        nmea[0][pos] = tempMsg[0].substring(stringplace, i);
      }
    }
  }

  // ----------- Read GPS 2 ------------- //  
    Serial2.read();  
    if (Serial2.find("GPGGA,")) {     
      tempMsg[1] = Serial2.readStringUntil('\n'); // Collect the data set from GNGGA     
      // Collecting Data from GPS#2
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[1].length(); i++) {
        if (tempMsg[1].substring(i, i + 1) == ",") {
          nmea[1][pos] = tempMsg[1].substring(stringplace, i);
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[1].length() - 1) {
          nmea[1][pos] = tempMsg[1].substring(stringplace, i);
        }
      }
    }

  // ----------- Read GPS 3 ------------- //  
    Serial3.read();  
    if (Serial3.find("GPGGA,")) {     
      tempMsg[2] = Serial3.readStringUntil('\n'); // Collect the data set from GNGGA     
      // Collecting Data from GPS#3
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[2].length(); i++) {
        if (tempMsg[2].substring(i, i + 1) == ",") {
          nmea[2][pos] = tempMsg[2].substring(stringplace, i);
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[2].length() - 1) {
          nmea[2][pos] = tempMsg[2].substring(stringplace, i);
        }
      }
    }

  // Convert raw coordination to readable lat lon
  for(int n=0 ; n<3 ; n++){
    nmea[n][1] = ConvertLat(n);
    nmea[n][3] = ConvertLng(n);
  }
}

// ------------------------------------------ Loop ------------------------------------------ //

void loop(){

  unsigned long currentMillis = millis();

  if(currentMillis - lastRefreshTime >= loopInterval){
    Serial.print(millis()); // Time when loop start
    Serial.print(",  ");
    
  // --------------- Read GPS 1 --------------- //
    Serial1.read();
    if (Serial1.find("GNGGA,")) {      
      tempMsg[0] = Serial1.readStringUntil('\n'); // Collect the data set from GNGGA    
      // Collecting Data from GPS#1
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[0].length(); i++) {   
        if (tempMsg[0].substring(i, i + 1) == ",") {
          nmea[0][pos] = tempMsg[0].substring(stringplace, i);
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[0].length() - 1) {
          nmea[0][pos] = tempMsg[0].substring(stringplace, i);
        }
      }
    }

  // ----------- Read GPS 2 ------------- //  
    Serial2.read();  
    if (Serial2.find("GPGGA,")) {     
      tempMsg[1] = Serial2.readStringUntil('\n'); // Collect the data set from GNGGA     
      // Collecting Data from GPS#2
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[1].length(); i++) {
        if (tempMsg[1].substring(i, i + 1) == ",") {
          nmea[1][pos] = tempMsg[1].substring(stringplace, i);
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[1].length() - 1) {
          nmea[1][pos] = tempMsg[1].substring(stringplace, i);
        }
      }
    }

  // ----------- Read GPS 3 ------------- //  
    Serial3.read();  
    if (Serial3.find("GPGGA,")) {     
      tempMsg[2] = Serial3.readStringUntil('\n'); // Collect the data set from GNGGA     
      // Collecting Data from GPS#3
      stringplace = 0; // reset char count
      pos = 0; // reset element count
      for (int i = 0; i < tempMsg[2].length(); i++) {
        if (tempMsg[2].substring(i, i + 1) == ",") {
          nmea[2][pos] = tempMsg[2].substring(stringplace, i);
          stringplace = i + 1;
          pos++;
        }
        if (i == tempMsg[2].length() - 1) {
          nmea[2][pos] = tempMsg[2].substring(stringplace, i);
        }
      }
    }
  
    // Convert raw coordination to readable lat lon
    for(int n=0 ; n<3 ; n++){
      nmea[n][1] = ConvertLat(n);
      nmea[n][3] = ConvertLng(n);
    }

//    // Make GPS data ready for calculate via Kalman filter ; n = GPS ID
//    for(int n=0 ; n<3 ; n++){  
//      resultLat[n] = strToDouble(nmea[n][1]); 
//      resultLng[n] = strToDouble(nmea[n][3]); 
//      resultAlt[n] = strToDouble(nmea[n][8]); 
//    }
  
  //-------------------------Print out result---------------------------//
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

//// --------------- String to double ---------------- //
//
//double strToDouble(String str){
//  int digitCount = -1; //x 
//  int decimal = 0 ; // y 
//  int decimalDigit = 0 ;
//  double convertValue = 0; // converted
//  String subString ; // char
//  int digitCondition=0 ; // check if found .
//    for(int i = 0 ; i<str.length() ; i++){
//      subString = str.substring(i,i+1);
//      if(!subString.equals(".")){      
//        convertValue += subString.toInt()*pow(10,digitCount);
//        digitCount -= 1 ;
//        if(digitCondition == 0){
//          decimal += 1 ;
//        }else{
//          decimalDigit +=1 ;  
//        }
//      }else{
//        digitCondition = 1 ;
//      }
//    }
//  convertValue *= pow(10,(decimal));
//  return convertValue;
//}
//-------------------------------Convert Latitude-------------------------------//

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

//-------------------------------Convert Longitude-------------------------------//

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


////-------------------------------Convert Latitude2-------------------------------//
//
//String ConvertLat2() {
//  String posneg = "";
//  if (nmea[1][1] == "S") {
//    posneg = "-";
//  }
//  String latfirst;
//  float latsecond;
//  for (int i = 0; i < nmea[1][1].length(); i++) {
//    if (nmea[1][1].substring(i, i + 1) == ".") {
//      latfirst = nmea[1][1].substring(0, i - 2);
//      latsecond = nmea[1][1].substring(i - 2).toFloat();
//    }
//  }
//  latsecond = latsecond / 60;
//  String CalcLat = "";
//
//  char charVal[9];
//  dtostrf(latsecond, 4, 6, charVal);
//  for (int i = 0; i < sizeof(charVal); i++)
//  {
//    CalcLat += charVal[i];
//  }
//  latfirst += CalcLat.substring(1);
//  latfirst = posneg += latfirst;
//  return latfirst;
//}
//
////-------------------------------Convert Longitude2-------------------------------//
//
//String ConvertLng2() {
//  String posneg = "";
//  if (nmea[1][3] == "W") {
//    posneg = "-";
//  }
//
//  String lngfirst;
//  float lngsecond;
//  for (int i = 0; i < nmea[1][3].length(); i++) {
//    if (nmea[1][3].substring(i, i + 1) == ".") {
//      lngfirst = nmea[1][3].substring(0, i - 2);
//      //Serial.println(lngfirst);
//      lngsecond = nmea[1][3].substring(i - 2).toFloat();
//      //Serial.println(lngsecond);
//
//    }
//  }
//  lngsecond = lngsecond / 60;
//  String CalcLng = "";
//  char charVal[9];
//  dtostrf(lngsecond, 4, 6, charVal);
//  for (int i = 0; i < sizeof(charVal); i++)
//  {
//    CalcLng += charVal[i];
//  }
//  lngfirst += CalcLng.substring(1);
//  lngfirst = posneg += lngfirst;
//
//  return lngfirst;
//}
/// Original Code Here ///

//// Licensed under the GNU General Public License v3.0
//
////The UNO needs the software emulator of the serial port
//#include <SoftwareSerial.h>
//
//// Global variables
//String inputMessage = "";        // A string to hold incoming data
//boolean IsMessageReady = false;  // Whether the string is complete
//
//// gpsSerial(receive from GPS,transmit to the GPS module)
//SoftwareSerial gpsSerial(2,3);
//
//void setup()
//{
//  //Receive from the GPS device (the NMEA sentences) - Green wire
//  pinMode(2, INPUT);   
//
//  //Transmit to the GPS device - Yellow wire
//  pinMode(3, OUTPUT);  
//
//  // Keep the User informed
//  Serial.begin(9600);
//  Serial.println("Initializing GPS");
//  
//  // Connect to the GPS module
//  gpsSerial.begin(9600);
//  
//  // Reserve 200 bytes for the SoftwareSerial string
//  inputMessage.reserve(200);
//}
//
//void loop()
//{
//  while (gpsSerial.available() && IsMessageReady == false) 
//  {
//     // Read the new byte:
//     char nextChar = (char)gpsSerial.read();
//     
//     // Append to the inputSerial1 string
//     inputMessage += nextChar;
//     
//     // If a newline, then set flag so that the main loop will process the string
//     if (nextChar == '\n') {
//       IsMessageReady = true;
//     }
//   }
//   
//    // Does SoftwareSeria1 have a new message?
//   if (IsMessageReady) 
//   {
//     // Print new message on a new line. 
//     // The last character of the SoftwareSerial is a new line
//     Serial.print(inputMessage);
//     
//     // clear the string:
//     inputMessage = "";
//     IsMessageReady = false;
//   }
//}
