bool getPosition(void) ;
void serialFlush(void) ;

#include <SoftwareSerial.h>

SoftwareSerial GPS(10, 11); // RX, TX

float latitude, longitude, speed;

void setup() {
  Serial.begin(9600);
  GPS.begin(9600);
}

void loop() {
  if (getPosition()) {
    Serial.print(latitude, 6);
    Serial.print(",");
    Serial.print(longitude, 6);
    Serial.print(" | Speed (Km) : ");
    Serial.print(speed, 2);
    Serial.println();
    delay(2000);
  }
}

bool getPosition() {
  if (GPS.available()) {
    String line = "";
    while(GPS.available()) {
      char c = GPS.read();
      if (c == '\r') {
        if (line.indexOf("$GPRMC") >= 0) {
          // Serial.println(line);
          String dataCut[13];
          int index = 0;
          for (int dataStart=0;dataStart<line.length();) {
            dataCut[index] = line.substring(dataStart+1, line.indexOf(',', dataStart+1));
            // Serial.println(dataCut[index]);
            dataStart = line.indexOf(',', dataStart+1);
            index++;
          }
          if (dataCut[2] == "A") {
            int dotPos = 0;
            dotPos = dataCut[3].indexOf('.');
            String latDeg = dataCut[3].substring(0, dotPos-2);
            String latMin = dataCut[3].substring(dotPos-2, dotPos+10);
            dotPos = dataCut[5].indexOf('.');
            String lngDeg = dataCut[5].substring(0, dotPos-2);
            String lngMin = dataCut[5].substring(dotPos-2, dotPos+10);
            latitude = (latDeg.toFloat() + (latMin.toFloat() / 60.0)) * (dataCut[4] == "N" ? 1 : -1);
            longitude = (lngDeg.toFloat() + (lngMin.toFloat() / 60.0)) * (dataCut[6] == "E" ? 1 : -1);
            speed = dataCut[7].toFloat() * 1.652;
            
            return true;
          } else {
            Serial.println("Error : No fix now.");
          }
          serialFlush();
        }
        line = "";
      } else if (c == '\n') {
        // pass
      } else {
        line += c;
      }
      delay(1);
    }
  }
  return false;
}

void serialFlush() {
  while(Serial.available()) Serial.read();
}
