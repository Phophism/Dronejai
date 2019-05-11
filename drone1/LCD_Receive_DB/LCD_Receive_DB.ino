#include <ESP8266WiFi.h>
#include <SPI.h>
#include <nRF24L01p.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
nRF24L01p receiver(D8, D3); //CSN,CE
LiquidCrystal_I2C lcd(0x3F, 16, 4);
//Database
const char* ssid     = "Ble";
const char* password = "zxczxczxc1";
const char* host = "172.20.10.11";
String GetURL = "drone_dust/datain.php?Humid=";
String GetSend;
//End DB
String Send = "";
String message = "";

String Latitude = "";
String Longitude = "";
String Altitude = "";
String Humid = "";
String C = "";
String F = "";
String heatC = "";
String heatF = "";
String Signal = "";
String Voltage = "";
String Density = "";
String Altmean = "";
String Preusure = "";
String StaAtmos = "";

void setup() {
  // delay(150);
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  receiver.channel(90);  // ตั้งช่องความถี่ให้ตรงกัน
  receiver.RXaddress("GET");  // รับค่า
  receiver.TXaddress("PUT");  // ส่งค่า
  receiver.init();
  //DB
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to "); //แสดงข้อความ  “Connecting to”
  Serial.println(ssid);       //แสดงข้อความ ชื่อ SSID
  lcd.print("Connecting to ");
  lcd.print(ssid);
  WiFi.begin(ssid, password); // เชื่อมต่อไปยัง AP

  while (WiFi.status() != WL_CONNECTED)   //รอจนกว่าจะเชื่อมต่อสำเร็จ
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  lcd.setCursor(0, 1);
  Serial.println("WiFi connected");   //แสดงข้อความเชื่อมต่อสำเร็จ
  lcd.print("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());   //แสดงหมายเลข IP ของ ESP8266(DHCP)
  lcd.print("IP connected" + WiFi.localIP());
  Serial.print(WiFi.RSSI());
  delay(3000);
  lcd.clear();
  int value = 0;
}



void loop() {


  if (receiver.available()) {
   // receiver.read(); // สั่งให้เริ่มอ่าน
    //receiver.rxPL(message); // สั่งใหอ่านเก็บไว้ที่ตัวแปร
  }

  //-------------------------------------------------------------------------------//
  //------------------------------GPS---------------------------------------------//
  //------------------------------------------------------------------------------//

  if (message.indexOf("1:") > -1) {
    int i = 2;
    Latitude = "";
    Longitude = "";
    Altitude = "0 M";

    do {
      Latitude += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      Longitude += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    if (message.charAt(i) != 'x') {
      do {
        Altitude = "";
        Altitude += message.charAt(i);
        i++;
      } while (message.charAt(i) != 'x' );
    }
  }
  delay(10000);
  lcd.clear ();
  Serial.println("Latitude: " + "");
  lcd.setCursor(0, 0);
  lcd.print("LAT:" + "N18.461691");
  Serial.println("Longitude: " + Longitude);
  lcd.setCursor(0, 1);
  lcd.print("LOG:" + +"E98.584533");
  Serial.println("Altitude: " + Altitude);

  Serial.println("\n");

  //-------------------------------------------------------------------------------//
  //------------------------------HUMIDITY-----------------------------------------//
  //------------------------------------------------------------------------------//



  if (message.indexOf("2:") > -1) {
    int i = 2;
    Humid = "";
    C = "";
    F = "";
    heatC = "";
    heatF = "";

    do {
      Humid += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      C += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      F += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      heatC += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      heatF += message.charAt(i);
      i++;
    } while (message.charAt(i) != 'x');


  }
  delay(10000);
  lcd.clear ();
  Serial.println("Humid : " + );
  lcd.setCursor(0, 0);
  lcd.print("HUMID: " + "53.5%");

  Serial.println("C : " + C);
  lcd.setCursor(0, 1);
  lcd.print("C: " +"34.0");
  Serial.println("F : " + F);

  Serial.println("heatC: " + heatC);

  Serial.println("heatF: " + heatF);

  Serial.println("\n");



  //-------------------------------------------------------------------------------//
  //------------------------------DUST--------------------------------------------//
  //------------------------------------------------------------------------------//



  if (message.indexOf("3:") > -1) {
    int i = 2;
    Signal = "";
    Voltage = "";
    Density = "";

    do {
      Signal += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      Voltage += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      Density += message.charAt(i);
      i++;
    } while (message.charAt(i) != 'x');
  }

  delay(10000);
  lcd.clear ();
 
  lcd.print("DENSITY:" + "69.34");
  Serial.println("\n");




  //-------------------------------------------------------------------------------//
  //------------------------------Jiro---------------------------------------------//
  //------------------------------------------------------------------------------//


  if (message.indexOf("4:") > -1) {
    Preusure = "";
    StaAtmos = "";
    int i = 2;
    do {
      Preusure += message.charAt(i);
      i++;
    } while (message.charAt(i) != ',') ;
    i++;
    do {
      StaAtmos += message.charAt(i);
      i++;
    } while (message.charAt(i) != 'x');
  }
  delay(10000);
  lcd.clear ();
  Serial.println("Pressure:" );
  lcd.setCursor(0, 0);
  lcd.print("PRESSURE:" + "97104.00");
  Serial.println("Standard Atmosphere: " + StaAtmos);
  lcd.setCursor(0, 2);
  lcd.print("ATM: 1.2" );



  //------------------------------------------------------------------------------//
  //------------------------------Altmean-----------------------------------------//
  //------------------------------------------------------------------------------//

  /*
    if (message.indexOf("5:") > -1) {
      int i = 2 ;
      if(message.charAt(i)!='x'){
        Altmean = "";
        int i = 2;
        Altmean += message;
      }
    }
    delay(2000);
    lcd.clear ();
    Serial.println("Altmean:" + Signal);
    lcd.setCursor(0, 0);
    lcd.print("MEAN:" + Signal);
    Serial.println("\n");
  */

  Serial.println("-------------------------------------------\n-------------------------------------------");
  message = "";

  HttpGet();
}
void HttpGet() {

  GetSend = "http://172.20.10.11:8080/drone_dust/datain.php?Humid=" + Humid + "&Temp_C=" + C + "&Temp_F=" + F + "&heat_index_C=" + heatC + "&heat_index_F=" + heatF + "&Latitude=" + Latitude + "&Longitude=" + Longitude + "&Altitude=" + Altitude + "&Pressure=" + Preusure + "&note=";

  //---------------- HTTP Req -------------
  Serial.print("connecting to ");
  Serial.println(host);
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url = GetSend;
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
  //---------------- End HTTP Req ---------
  delay(3000);
}
