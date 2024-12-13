#include <WiFi.h>
#include <WiFiUDP.h>
#include <OSCBundle.h>
#include <Wire.h>

#define DEBUG
//#define SLEEP

// WiFi network name and password:
const char * networkName = "mallette_pedagogique";
const char * networkPswd = "scrimeLABRI2021";

//IP address to send UDP data to:
// either use the ip address of the server or
// a network broadcast address
const char * udpAddress = "192.168.1.255";
const int udpPort = 8000;
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

//Are we currently connected?
boolean connected = false;

int accel_x_axis, accel_y_axis, accel_z_axis ;
byte z_button ; // bouton Z
byte c_button ; // bouton c
byte buffer[6]; // Buffer contenant les 6 prÃ©cieux octets qui nous intÃ©resse
byte cnt = 0; // index courant de buffer
byte led = 2; //onboard led gpio2

int previousmillis = 0;
int sleepPin = 1;

//The udp library class
WiFiUDP udp;

void setup() {
  // Initilize hardware serial:
  Serial.begin(115200);
  Serial.println();

  pinMode(LED_BUILTIN, OUTPUT);

#ifdef SLEEP
  pinMode(sleepPin, INPUT);
  digitalWrite(sleepPin, LOW);
#endif

  //Connect to the WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(networkName);
  WiFi.begin(networkName, networkPswd);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  //connectToWiFi(networkName, networkPswd);

  udp.beginPacket(udpAddress, udpPort);
  udp.printf("Seconds since boot: %lu", millis() / 1000);
  udp.endPacket();

  Wire.begin(21, 22);
  Wire.beginTransmission (0x52);
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  Wire.beginTransmission (0x52);
  Wire.write (0xFB);
  Wire.write (0x00);
  Wire.endTransmission ();
}

void handshake()
{
  Wire.begin(21, 22);
  Wire.beginTransmission (0x52);
  Wire.write (0x00);
  Wire.endTransmission ();
  delay(10);
}

void parse() {
  int joy_x_axis = buffer[0]; // joystick axe x (0-255)
  int joy_y_axis = buffer[1]; // joystick axe y (0-255)
  int accel_x_axis = buffer[2] * 4 ; // accÃ©lÃ©romÃ¨tre axe x
  int accel_y_axis = buffer[3] * 4 ; // accÃ©lÃ©romÃ¨tre axe y
  int accel_z_axis = buffer[4] * 4 ; // accÃ©lÃ©romÃ¨tre axe z
  if ((buffer[5] >> 0) & 1) z_button = 0;
  else z_button = 1;
  if ((buffer[5] >> 1) & 1) c_button = 0;
  else c_button = 1;
  if ((buffer[5] >> 2) & 1)
    accel_x_axis += 2;
  if ((buffer[5] >> 3) & 1)
    accel_x_axis += 1;
  if ((buffer[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((buffer[5] >> 5) & 1)
    accel_y_axis += 1;
  if ((buffer[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((buffer[5] >> 7) & 1)
    accel_z_axis += 1;

  accel_x_axis = accel_x_axis - 512;
  accel_y_axis = accel_y_axis - 512;
  accel_z_axis = accel_z_axis - 512;

  float Pitch = 180 * atan2(accel_y_axis, sqrt(pow(accel_x_axis, 2) + pow(accel_z_axis, 2))) / M_PI;
  float Roll = 180 * atan2(accel_x_axis, sqrt(pow(accel_y_axis, 2) + pow(accel_z_axis, 2))) / M_PI;

  if (Pitch > 0 && accel_z_axis < 0) {
    Pitch = 180 - Pitch;
  }

  if (Pitch < 0 && accel_z_axis < 0) {
    Pitch = -180 - Pitch;
  }

  if (Roll > 0 && accel_z_axis < 0) {
    Roll = 180 - Roll;
  }

  if (Roll < 0 && accel_z_axis < 0) {
    Roll = -180 - Roll;
  }


#ifdef DEBUG
  Serial.print ("NUNCHUCK ");
  Serial.print (joy_x_axis, DEC);
  Serial.print ("\t");
  Serial.print (joy_y_axis, DEC);
  Serial.print ("\t");
  Serial.print (accel_x_axis, DEC);
  Serial.print ("\t");
  Serial.print (accel_y_axis, DEC);
  Serial.print ("\t");
  Serial.print (accel_z_axis, DEC);
  Serial.print ("\t");
  Serial.print (z_button, DEC);
  Serial.print ("\t");
  Serial.print (c_button, DEC);
  Serial.print ("\t");
  Serial.print (Pitch, DEC);
  Serial.print ("\t");
  Serial.print (Roll, DEC);
  Serial.print ("\t");
  Serial.println();
#endif
  OSCBundle bndl; //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
  bndl.add("/blackNun/Pitch").add((float)Pitch / 180);
  bndl.add("/blackNun/Roll").add((float)Roll / 180);
  bndl.add("/blackNun/Zaccel").add((float)accel_z_axis / 1023);
  bndl.add("/blackNun/Xjoy").add((float)joy_x_axis / 255);
  bndl.add("/blackNun/Yjoy").add((float)joy_y_axis / 255);
  bndl.add("/blackNun/Cbutt").add((float)c_button);
  bndl.add("/blackNun/Zbutt").add((float)z_button);
  udp.beginPacket(udpAddress, udpPort);
  bndl.send(udp);
  udp.endPacket();
  bndl.empty();
}

void loop() {

#ifdef SLEEP
  int sleep = digitalRead(sleepPin);
  while (sleep == 1) {
    digitalWrite(LED_BUILTIN, LOW);
    ESP.deepSleep(3600000);
  }
#endif

  while (WiFi.status() != 3 ) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
  }

  digitalWrite(LED_BUILTIN, HIGH);

  Wire.begin(21, 22);
  Wire.requestFrom (0x52, 6);
  delay(10);
  while (Wire.available ())
  {
    buffer[cnt] = Wire.read();
    cnt++;
  }
  if (cnt >= 5) {
    parse();
  }
  cnt = 0;
  handshake();
  delay(10);

  //  Serial.println(millis()-previousmillis);
  //  previousmillis = millis();
}

void connectToWiFi(const char * ssid, const char * pwd) {
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      //When connected set
      Serial.print("WiFi connected! IP address: ");
      Serial.println(WiFi.localIP());
      //initializes the UDP state
      //This initializes the transfer buffer
      udp.begin(WiFi.localIP(), udpPort);
      connected = true;
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("WiFi lost connection");
      connected = false;
      break;
    default: break;
  }
}
