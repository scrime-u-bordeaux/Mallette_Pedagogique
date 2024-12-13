#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
extern "C" {
#include<user_interface.h>
}
const char* ssid = "EspAccesPoint";
const char* password = "azertyuiop";

IPAddress local_IP(192, 168, 0, 2);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP Udp;
unsigned int localUdpPort = 8000; // local port to listen on
OSCErrorCode error;
unsigned long nextmillis = 0;
#define BUILTIN_LED 2

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  while (WiFi.softAPgetStationNum() == 0)
  {
    analogWrite(BUILTIN_LED, 1023);
    delay(100);
    analogWrite(BUILTIN_LED, 0);
    delay(100);
    Serial.print(".");
  }
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());
  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
  Serial.printf("MAC address = %s\n", WiFi.softAPmacAddress().c_str());
  //client_status();
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.softAPIP().toString().c_str(), localUdpPort);
}

void led(OSCMessage &msg) {
  int ledState = 1023 - 1023 * msg.getFloat(0);
  analogWrite(BUILTIN_LED, ledState);
}

void loop() {
  if (!wifi_softap_get_station_num() ) { // Blink build in led if deconnected
    analogWrite(2, 1023);
    delay(100);
    analogWrite(2, 0);
    delay(100);
  }
  OSCBundle bundle;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      bundle.dispatch("/axo1", led);
     
    }
    else {
      error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}
