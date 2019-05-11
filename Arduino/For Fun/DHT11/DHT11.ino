#include <LiquidCrystal.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LCD I2C
LiquidCrystal_I2C lcd(0x27,16,2);

// DHT Sensor
DHT dht;
float humidity ;
float temperature ;

// Set up for caribrate soil moisture
int dryValue = 0;
int wetValue = 1023;
int friendlyDryValue = 0;
int friendlyWetValue = 100;
int friendlyValue; 

// switch lcd info display
int time=0 ; 

void setup()
{
  delay(2000); 
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(); 
  welcome();
  lcd.clear(); // ล้างหน้าจอ
  dht.setup(2); // data pin 2
}

void loop(){

if(time == 0){ 
  humid();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("  humidity (%)");
  lcd.setCursor(4,1);
  lcd.println( String(humidity) + " %");
  time = 1;
 }else if(time == 1){
  temp();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println(" temperature (C)");
  lcd.setCursor(4,1);
  lcd.println( String(temperature) + " C ");
  time = 2;
 }else{
  delay(500);
  moisture();
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.println(" Moisture ");
  lcd.setCursor(5,1);
  lcd.println( String(friendlyValue) + " %" );
  time = 0;
 }
  delay(800);
  
}

void moisture(){
  int rawValue = analogRead(A0);
  friendlyValue = map(rawValue, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
}

void welcome(){
  lcd.setCursor(4,0);
  lcd.println("Welcome");
  delay(200);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.println(".Welcome.");
  delay(200);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.println("..Welcome..");
  delay(200);
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.println("Welcome");
  delay(200);
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.println(".Welcome.");
  delay(200);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.println("..Welcome..");
  delay(1000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.println("....START!....");
  delay(2000);
}

void humid(){
  delay(dht.getMinimumSamplingPeriod());
  humidity = dht.getHumidity(); // ดึงค่าความชื้น
}

void temp(){
  delay(dht.getMinimumSamplingPeriod());
  temperature = dht.getTemperature(); // ดึงค่าอุณหภูมิ
}

