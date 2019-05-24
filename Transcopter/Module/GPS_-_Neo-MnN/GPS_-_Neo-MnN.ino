//The UNO needs the software emulator of the serial port
#include <AltSoftSerial.h>

// Global variables
String inputMessage = "";        // A string to hold incoming data
boolean IsMessageReady = false;  // Whether the string is complete
String nmea[15];
int pos;
int stringplace = 0;

// gpsSerial(receive from GPS,transmit to the GPS module)
AltSoftSerial gpsSerial(8,9); // RX,TX 

void setup(){
  // Keep the User informed
  Serial.begin(9600);
  Serial.println("Initializing GPS");
  // Connect to the GPS module
  gpsSerial.begin(9600);
 }

void loop(){
  while (gpsSerial.available() && IsMessageReady == false)
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
    nmea[1] = ConvertLat();
    nmea[3] = ConvertLng();
    
//         Serial.print(nmea[1]+","+nmea[3]+","+nmea[8]);
         Serial.println(""); 
         Serial.println(tempMsg);  
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
