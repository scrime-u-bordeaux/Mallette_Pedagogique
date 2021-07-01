/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266/ESP32

  Example for receiving open sound control (OSC) bundles on the ESP8266/ESP32
  Send integers '0' or '1' to the address "/led" to turn on/off the built-in LED of the esp8266.

  This example code is in the public domain.

  --------------------------------------------------------------------------------------------- */
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

char ssid[] = "NUMERICABLE-A7A6";          // your network SSID (name)
char pass[] = "DD0258CE42F43E8AE7EACD4D4D";                    // your network password
uint8_t serialData[4] = { 0 };
unsigned long nextmillis = 50;
// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
//const IPAddress outIp(192, 168, 0, 255);     // remote IP of your computer
//const unsigned int outPort = 8888;         // remote port to receive OSC
const unsigned int localPort = 8000;        // local port to listen for OSC packets (actually not used for sending)
OSCErrorCode error;
int ledState;              // LOW means led is *on*
#define BUILTIN_LED 2


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, ledState);    // turn *on* led

  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, 1);
    delay(100);
    digitalWrite(BUILTIN_LED, 0);
    delay(100);
    Serial.print(".");
  }
  //    IPAddress ip(192, 168, 0, 11); ////////////////////////////////////////////
  //    IPAddress routeur(192, 168, 0, 1);
  //    IPAddress subnet(255, 255, 255, 0);
  //    WiFi.config(ip, routeur, subnet);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
#ifdef ESP32
  Serial.println(localPort);
#else
  Serial.println(Udp.localPort());
#endif

}


void led(OSCMessage &msg) {
  uint32_t msg32 = msg.getFloat(0) * 0xffffffff;
  int ledState = 1023 - 1023 * msg.getFloat(0);
  analogWrite(2, ledState);
 /* pack into buf string */
        serialData[0] = 255 * msg.getFloat(0);//msg32 >> 24;
        serialData[1] = msg32 >> 16;
        serialData[2] = msg32 >> 8;
        serialData[3] = msg32;
 
        /* convert the string back to a uint32_t */
    //  uint32_t  y = (serialData[0] <<  24) | (serialData[1] << 16) | (serialData[2] << 8) | serialData[3]; Serial.println(y);


}

void led2(OSCMessage &msg) {
  int ledState = 1023 - 1023 * msg.getFloat(0);
  analogWrite(2, ledState);
  Serial.print("/led: ");
  Serial.println(ledState);
  delay(500);
}
void loop() {
  OSCBundle bundle;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      bundle.fill(Udp.read());
    }
    if (!bundle.hasError()) {
      // bundle.dispatch("/blackNun/Cbutt", led2);
      bundle.dispatch("/axo1", led);

    } else {
      error = bundle.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  if (WiFi.status() != 3 ) {
    analogWrite(2, 1023);
    delay(100);
    analogWrite(2, 0);
    delay(100);
  }
  if(millis()>=nextmillis){
    nextmillis = nextmillis + 40;
    //Serial.println("data : ");
//    for (int i = 0; i <sizeof(serialData)-1; i++){
//      Serial.write(serialData[i]);
//    }
    Serial.write(serialData[0]);
  }

}



