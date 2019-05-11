/* IOXhop - www.ioxhop.com */

#include "DHT.h"
#include "ESP8266WiFi.h"
#include "WiFiClient.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"

#define DHTPIN 0
#define DHTTYPE DHT11
const char* ssid = "...";
const char* password = "...";

// HTML Page
String IndexPage =
"<!DOCTYPE HTML>\n"
"<html>\n"
"<head>\n"
"<meta charset=\"utf-8\">\n"
"<title>ESP-DHT By IOXhop</title>\n"
"<script>\n"
"function loadXMLDoc() {\n"
"  var xmlhttp;\n"
"  if (window.XMLHttpRequest) { \n"
"    xmlhttp=new XMLHttpRequest();\n"
"  } else { \n"
"    xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\");\n"
"  }\n"
"  \n"
"  xmlhttp.onreadystatechange = function() {\n"
"    if (xmlhttp.readyState==4 && xmlhttp.status==200) {\n"
"      var objJSON = JSON.parse(xmlhttp.responseText);\n"
"      var d = new Date();\n"
"      document.getElementById(\"update\").innerHTML = d.getHours() + \":\" + d.getMinutes() + \":\" + d.getSeconds();\n"
"      document.getElementsByTagName(\"output\")[0].innerHTML=objJSON.temp+\" *C\";\n"
"      setTimeout(function(){ loadXMLDoc() }, 1000);\n"
"    }\n"
"  }\n"
"  xmlhttp.open(\"GET\", \"./temperature\", true);\n"
"  xmlhttp.send();\n"
"}\n"
"\n"
"loadXMLDoc();\n"
"</script>\n"
"</head>\n"
"\n"
"<body style=\"font-family: Arial, Helvetica, sans-serif;\">\n"
"<h1 style=\"text-align: center;\">E-TECH Temp Node</h1>\n"
"<h2 style=\"text-align: center;\">Temperature</h2>\n"
"<output style=\"text-align: center; font-size: 220px; display: block;\">0</output>\n"
"<p style=\"text-align: center;\">Update: <span id=\"update\">00:00</span></p>\n"
"</body>\n"
"</html>\n"
;

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {
  Serial.begin(115200);
  //WiFi.begin(ssid, password);
  dht.begin();
  Serial.println("");

  int t = dht.readTemperature();
}

void loop() {
  server.handleClient();
}
