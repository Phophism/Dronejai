/////////// Not working //////////////


#include <SoftwareSerial.h>
 
#define GPSENABLE 2
SoftwareSerial mySerial(8,9); // RX, TX
byte GPSBuffer[82];
byte GPSIndex=0;
void setup()
{
  pinMode(GPSENABLE, OUTPUT);
  digitalWrite(GPSENABLE, HIGH);
  Serial.begin(9600);
  mySerial.begin(9600);
  mySerial.println("Test Program");
}
 
void loop()
{
  CheckGPS();
}
 
void CheckGPS()
{
  int inByte;
  while (Serial.available() > 0)
  {
    inByte = Serial.read();
 
    if ((inByte =='$') || (GPSIndex >= 80))
    {
      GPSIndex = 0;
    }
 
    if (inByte != '\r')
    {
      GPSBuffer[GPSIndex++] = inByte;
    }
 
    if (inByte == '\n')
    {
      ProcessGPSLine();
      GPSIndex = 0;
    }
  }
}
void ProcessGPSLine()
{
  if ((GPSBuffer[1] == 'G') && (GPSBuffer[2] == 'N') && (GPSBuffer[3] == 'G') && (GPSBuffer[4] == 'G') && (GPSBuffer[5] == 'A'))
  {
    mySerial.println("GNGGA Detected!");
  }
  else
  {
    mySerial.println("It was something else!");
  }
}
