#include <AltSoftSerial.h>

AltSoftSerial gpsSerial(8,9); // RX,TX 

// ----------------------- Global variables ---------------------------//

String inputMessage = "";        // A string to hold incoming data
boolean IsMessageReady = false;  // Whether the string is complete
String nmea[15]; // GPS Data Set
int pos; // Next element
int stringplace = 0; // Next char

double resultLat ; 
double resultLng ; 
double resultAlt ;

// ----------------------- Kalman variables ALT ---------------------//

float R_alt = 20, Q_alt  = 1e-4;  //Q = process noise covariance, R = measurement noise covariance
double Xpe0_alt  = 0;  // Xpe0 = prior estimation of signal X at time t=0 (current state)
double Xe1_alt  = 0;  //Xe1 = estimation of X at time t=1 (previous state)
double Ppe0_alt  = 0.0;  //Ppe0 = prior estimation of "error covariance" at t=0,  
double P1_alt  = 1, P0_alt  = 0; //P1 = error covariance at t=1, P0 = error covariance at t=0
double  K_alt  = 0.0, Xe0_alt  = 0.0, Z_alt  = 0.0; //K = Kalman gain, Xe0 = estimation of signal at t=0, Z = measured signal at t=0

double kalmanAlt ;

// ----------------------- Kalman variables LAT ---------------------//

float R_lat = 4.464e-10, Q_lat = 1e-11;  //Q = process noise covariance, R = measurement noise covariance
double Xpe0_lat = 0.0;  // Xpe0 = prior estimation of signal X at time t=0 (current state)
double Xe1_lat = 0.0;  //Xe1 = estimation of X at time t=1 (previous state)
double Ppe0_lat = 0.0;  //Ppe0 = prior estimation of "error covariance" at t=0,  
double P1_lat = 1, P0_lat = 0; //P1 = error covariance at t=1, P0 = error covariance at t=0
double  K_lat = 0.0, Xe0_lat = 0.0, Z_lat = 0.0; //K = Kalman gain, Xe0 = estimation of signal at t=0, Z = measured signal at t=0

double kalmanLat ;

// ----------------------- Kalman variables LNG ---------------------//

float R_lng = 1.397e-10, Q_lng = 1e-11;  //Q = process noise covariance, R = measurement noise covariance
double Xpe0_lng = 0.0;  // Xpe0 = prior estimation of signal X at time t=0 (current state)
double Xe1_lng = 0.0;  //Xe1 = estimation of X at time t=1 (previous state)
double Ppe0_lng = 0.0;  //Ppe0 = prior estimation of "error covariance" at t=0,  
double P1_lng = 1, P0_lng = 0; //P1 = error covariance at t=1, P0 = error covariance at t=0
double  K_lng = 0.0, Xe0_lng = 0.0, Z_lng = 0.0; //K = Kalman gain, Xe0 = estimation of signal at t=0, Z = measured signal at t=0

double kalmanLng ;

// ------------------------- Setup -------------------------- // 

void setup(){  
  Serial.begin(9600);
 // Serial.println("Initializing GPS");
  gpsSerial.begin(9600);  // Connect to the GPS module

  gpsSerial.read();
  
  if (gpsSerial.find("GNGGA,")) {
    String tempMsg = gpsSerial.readStringUntil('\n'); // Collect the data set from GNGGA
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
  }
  nmea[1] = ConvertLat();
  nmea[3] = ConvertLng();

  Xe1_lat  = strToDouble(nmea[1]); 
  Xe1_lng  = strToDouble(nmea[3]); 
  Xe1_alt  = strToDouble(nmea[8]); 
  
}

// ---------------------- Loop ------------------------- //

void loop(){
  
  while (gpsSerial.available() && IsMessageReady == false)
    gpsSerial.read();

  stringplace = 0; // reset char count
  pos = 0; // reset element count
  
  if (gpsSerial.find("GNGGA,")) {
    String tempMsg = gpsSerial.readStringUntil('\n'); // Collect the data set from GNGGA
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
  }
  nmea[1] = ConvertLat();
  nmea[3] = ConvertLng();
    
  resultLat = strToDouble(nmea[1]); 
  resultLng = strToDouble(nmea[3]); 
  resultAlt = strToDouble(nmea[8]); 

//----------------------------- Kalman Altitude ----------------------//

  Z_alt  = resultAlt ;
  Xpe0_alt  = Xe1_alt ; //Assumption or prediction 1
  Ppe0_alt  = P1_alt  + Q_alt ; //Assumption or prediction 2
  K_alt  = Ppe0_alt /(Ppe0_alt  + R_alt ); // Measurement update or correction of "Kalman gain"
  Xe0_alt  = Xpe0_alt  + K_alt  * (Z_alt  - Xpe0_alt); // Measurement update or correction of "estimated signal"
  P0_alt  = (1 - K_alt ) * Ppe0_alt ; // Measurement update or correction of "error covariance"
  kalmanAlt = Xe0_alt ;
  Xe1_alt  = Xe0_alt ; //Update: current t=0 becomes t=1 in the next step
  P1_alt  = P0_alt ; //Update: current t=0 becomes t=1 in the next step
  delay(10);

  //------------------------ Kalman Latitude -----------------------------//

  Z_lat  = resultLat ;
  Xpe0_lat  = Xe1_lat ; //Assumption or prediction 1
  Ppe0_lat  = P1_lat  + Q_lat ; //Assumption or prediction 2
  K_lat  = Ppe0_lat /(Ppe0_lat  + R_lat ); // Measurement update or correction of "Kalman gain"
  Xe0_lat  = Xpe0_lat  + K_lat  * (Z_lat  - Xpe0_lat); // Measurement update or correction of "estimated signal"
  P0_lat  = (1 - K_lat ) * Ppe0_lat ; // Measurement update or correction of "error covariance"
  kalmanLat = Xe0_lat ;
  Xe1_lat  = Xe0_lat ; //Update: current t=0 becomes t=1 in the next step
  P1_lat  = P0_lat ; //Update: current t=0 becomes t=1 in the next step
  delay(10);
  
  //------------------------ Kalman Longitude -----------------------------//
  
  Z_lng  = resultLng ;
  Xpe0_lng  = Xe1_lng ; //Assumption or prediction 1
  Ppe0_lng  = P1_lng  + Q_lng ; //Assumption or prediction 2
  K_lng  = Ppe0_lng /(Ppe0_lng  + R_lng ); // Measurement update or correction of "Kalman gain"
  Xe0_lng  = Xpe0_lng  + K_lng  * (Z_lng  - Xpe0_lng); // Measurement update or correction of "estimated signal"
  P0_lng  = (1 - K_lng ) * Ppe0_lng ; // Measurement update or correction of "error covariance"
  kalmanLng = Xe0_lng ;
  Xe1_lng  = Xe0_lng ; //Update: current t=0 becomes t=1 in the next step
  P1_lng  = P0_lng ; //Update: current t=0 becomes t=1 in the next step
  delay(10);

 //-------------------------Print out result---------------------------//
    
//  Serial.print(resultLat,6);
//  Serial.print(" , ");
//  Serial.print(resultLong,6);

  Serial.print(resultLat,6);
  Serial.print(",");
  Serial.print(Xe0_lat, 6);
  Serial.print(",");
  Serial.print(resultLng,6);
  Serial.print(",");
  Serial.print(Xe0_lng, 6);
  Serial.print(",");
  Serial.print(resultAlt,1);
  Serial.print(",");
  Serial.println(Xe0_alt, 4);
}

// --------------- String to double ---------------- //

double strToDouble(String str){
  int digitCount = -1; //x
  int decimal = 0 ; // y 
  int decimalDigit = 0 ;
  double convertValue = 0; // converted
  String subString ; // char
  int digitCondition=0 ; // check if found .
    for(int i = 0 ; i<str.length() ; i++){
      subString = str.substring(i,i+1);
      if(!subString.equals(".")){      
        convertValue += subString.toInt()*pow(10,digitCount);
        digitCount -= 1 ;
        if(digitCondition == 0){
          decimal += 1 ;
        }else{
          decimalDigit +=1 ;  
        }
      }else{
        digitCondition = 1 ;
      }
    }
  convertValue *= pow(10,(decimal));
  return convertValue;
}
//-------------------------------Convert Latitude-------------------------------//

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

//-------------------------------Convert Longitude-------------------------------//

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
