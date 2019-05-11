#include <NMEAGPS.h>
#include "SoftwareSerial.h"
#include <SPI.h>
#include <nRF24L01p.h>
#include "cactus_io_DHT22.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



#define DHT22_PIN 2 
#define BMP085_ADDRESS 0x77
String altMean = "" ;
int count=0;
int countFinal = 0;

#define MY_SIZE 20

const unsigned char OSS = 0;
SoftwareSerial GPSModule(10,11); // TX, RX
LiquidCrystal_I2C lcd(0x3F, 16, 2);// 0x27 for a 16 chars and 2 line display
NMEAGPS gps;
nRF24L01p transmitter(7,8); //CSN,CE
DHT22 dht(DHT22_PIN);


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

int measurePin = A0;    // dust
int ledPower = 3;       // dust
int samplingTime = 280; // dust
int deltaTime = 40;     // dust
int sleepTime = 9680;   // dust

float voMeasured = 0;   // `voltage
float calcVoltage = 0;  // calculate voltage to ug/m3
float dustDensity = 0;  /// density



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


void setup() {
  delay(3000);

  lcd.begin();
  lcd.backlight();
  lcd.print("    LOADING");
  delay(500);  
  lcd.clear();
  lcd.print("   .LOADING.");
  delay(500);
  lcd.clear();  
  lcd.print("  ..LOADING..");
  delay(500);
  lcd.clear();  
  lcd.print(" ...LOADING...");
  delay(2000);
  lcd.print("WELCOME");
 
  Serial.begin(9600);
  GPSModule.begin(9600);
  dht.begin();
  pinMode(ledPower,OUTPUT);
   Wire.begin();
   bmp085Calibration();
   

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  transmitter.channel(90); // ตั้งช่องความถี่ให้ตรงกัน
  transmitter.TXaddress("GET"); // ส่งค่า
  transmitter.RXaddress("PUT");  // รับค่า
  transmitter.init();

}

String message;


void loop() {
  
//------------------------------------------------------------------------------------\\
//---------------------------------------GPS------------------------------------------\\
//------------------------------------------------------------------------------------\\

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
    updates++;

    nmea[1] = ConvertLat();
    nmea[3] = ConvertLng();
/*
    Serial.println("GPS & Altitude");
    Serial.println("Latitude\tLongigude\tAltitude(m)");
    Serial.print(nmea[1]);
    Serial.print("\t");
    Serial.print(nmea[3]);
    Serial.print("\t");
    Serial.print(nmea[8]);
    Serial.println();
    Serial.println(); 
 */   
  }
  else {
    failedUpdates++;
  }
  
  stringplace = 0;
  pos = 0;


//-----------------------------------------------------------------------\\
//--------------------------------HUMIDITY-------------------------------\\
//-----------------------------------------------------------------------\\

  dht.readHumidity();
  dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(dht.humidity) || isnan(dht.temperature_C)) {
    Serial.println("DHT sensor read failure!");
    return;
  }
  
  Serial.println("DHT22 Humidity - Temperature Sensor");
  Serial.println("RH\t\tTemp (C)\tTemp (F)\tHeat Index (C)\t Heat Index (F)");
  Serial.print(dht.humidity); Serial.print(" %\t\t");
  Serial.print(dht.temperature_C); Serial.print(" *C\t");
  Serial.print(dht.temperature_F); Serial.print(" *F\t");
  Serial.print(dht.computeHeatIndex_C()); Serial.print(" *C\t");
  Serial.print(dht.computeHeatIndex_F()); Serial.println(" *F");

  // Wait a few seconds between measurements. The DHT22 should not be read at a higher frequency of
  // about once every 2 seconds. So we add a 3 second delay to cover this.


//-----------------------------------------------------------------------\\
//---------------------------------DUST----------------------------------\\
//-----------------------------------------------------------------------\\

 digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin); // read the dust value
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);
  // 0 - 5.0V mapped to 0 - 1023 integer values 
 
  calcVoltage = voMeasured * (5.0 / 1024); 
  dustDensity = (0.17 * calcVoltage - 0.1)*1000; 
 /* Serial.println("Signal=" + (String)voMeasured + " ");
  Serial.println("Voltage=" + (String)calcVoltage + " ");
  Serial.println("Density=" + (String)dustDensity + "ug/m3 ");
 */

//----------------------------------------------------------------------\\
//------------------------PRESSURE & TEMPERATURE------------------------\\
//----------------------------------------------------------------------\\

 float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 
/*
  Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.println("deg C ");

  Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");

  Serial.print("Standard Atmosphere: ");
  Serial.println(atm, 4); //display 4 decimal places

  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" M");
  
  Serial.println();//line break
*/


//-----------------------------------------------------------------------\\
//-------------------------------TRANSMITTER-----------------------------\\
//-----------------------------------------------------------------------\\

float altitudeTemp[MY_SIZE];

altitudeTemp[count] = nmea[8].toFloat();

if(altitudeTemp[count] == NULL){
     altitudeTemp[count] == 0;
    }
    
 if(countFinal >= 19 ){ 
  float v = 0;
  for(int i=0 ; i<= MY_SIZE ; i++){
    v += altitudeTemp[i];  
 }
   v = v/20;
   altMean = (String)v;
 }
 

String locats = "";
String humids = "";
String dusts = "";
String gy_prs = "" ;
locats += (String)nmea[1]; locats +=",";
locats += (String)nmea[3]; locats +=",";
locats += (String)nmea[8]; locats +="x";
humids += (String)dht.humidity ; humids +=",";
humids += (String)dht.temperature_C; humids +=",";
humids += (String)dht.temperature_F; humids +=",";
humids += (String)dht.computeHeatIndex_C(); humids +=",";
humids += (String)dht.computeHeatIndex_F();  humids +="x";
dusts += (String)voMeasured; dusts +=",";
dusts += (String)calcVoltage; dusts +=",";
dusts += (String)dustDensity;  dusts +="x";
gy_prs += (String)pressure; gy_prs += ",";
gy_prs += (String)atm; gy_prs +="x";
altMean = altMean+"x";


Serial.println("\n\n\n");
Serial.println(locats);
Serial.println(humids);
Serial.println(dusts);
Serial.println(altMean);
Serial.println(gy_prs);
Serial.println("\n\n\n");

   transmitter.txPL("1:"+locats); // ค่าที่ต้องการส่ง
   transmitter.send(FAST); // สั่งให้ส่งออกไป
   delay(200);
   transmitter.txPL("2:"+humids); // ค่าที่ต้องการส่ง
   transmitter.send(FAST); // สั่งให้ส่งออกไป
   delay(200);  
   transmitter.txPL("3:"+dusts); // ค่าที่ต้องการส่ง
   transmitter.send(FAST); // สั่งให้ส่งออกไป
   delay(200);
   transmitter.txPL("4:"+gy_prs); // ค่าที่ต้องการส่ง
   transmitter.send(FAST); // สั่งให้ส่งออกไป
   delay(200);
   if(altMean!=0){
       transmitter.txPL("5:"+altMean); // ค่าที่ต้องการส่ง
       transmitter.send(FAST); // สั่งให้ส่งออกไป
   }
  countFinal++;
  count++;
  if(count==20){
    count=0;
  }
   altMean="";
   delay(150);
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
// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature in deg C
float bmp085GetTemperature(unsigned int ut){
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  float temp = ((b5 + 8)>>4);
  temp = temp /10;

  return temp;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up){
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  long temp = p;
  return temp;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;

  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT(){
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP(){

  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  msb = bmp085Read(0xF6);
  lsb = bmp085Read(0xF7);
  xlsb = bmp085Read(0xF8);

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

void writeRegister(int deviceAddress, byte address, byte val) {
  Wire.beginTransmission(deviceAddress); // start transmission to device 
  Wire.write(address);       // send register address
  Wire.write(val);         // send value to write
  Wire.endTransmission();     // end transmission
}

int readRegister(int deviceAddress, byte address){

  int v;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address); // register to read
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1); // read a byte

  while(!Wire.available()) {
    // waiting
  }

  v = Wire.read();
  return v;
}
     
float calcAltitude(float pressure){

  float A = pressure/101325;
  float B = 1/5.25588;
  float C = pow(A,B);
  C = 1 - C;
  C = C /0.0000225577;

  return C;
}




