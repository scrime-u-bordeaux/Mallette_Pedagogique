#include <WiFi.h>
#include <WiFiUDP.h>
#include <OSCBundle.h>
#include <Wire.h>

//#define DEBUG1
//#define DEBUG2
//#define DEBUG3
#define DEBUG4

#define SDA_0 21
#define SCL_0 22

#define SDA_1 16
#define SCL_1 17

const char * networkName = "WHITEBOX";
const char * networkPswd = "blackbox";
const char * udpAddress = "192.168.1.255";
const int udpPort = 8000;
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)
boolean connected = false;

int accel_x_axis, accel_y_axis, accel_z_axis ;
byte z_button ;
byte c_button ;
byte buffer[6];
byte cnt = 0;
byte led = 2;

int joyx1, joyy1, accx1, accy1, accz1, z1, c1, joyx2, joyy2, accx2, accy2, accz2, z2, c2;

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(LED_BUILTIN, OUTPUT);

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

  udp.beginPacket(udpAddress, udpPort);
  udp.printf("Seconds since boot: %lu", millis() / 1000);
  udp.endPacket();

  Wire.begin(SDA_0 , SCL_0);
  Wire.beginTransmission (0x52);
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  Wire.beginTransmission (0x52);
  Wire.write (0xFB);
  Wire.write (0x00);
  Wire.endTransmission ();

  Wire.begin(SDA_1 , SCL_1);
  Wire.beginTransmission (0x52);
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  Wire.beginTransmission (0x52);
  Wire.write (0xFB);
  Wire.write (0x00);
  Wire.endTransmission ();
  delay(30);
}

void handshake1()
{
  Wire.begin(SDA_0 , SCL_0);
  Wire.beginTransmission (0x52);
  Wire.write (0x00);
  Wire.endTransmission ();
  delay(10);
}

void handshake2()
{
  Wire.begin(SDA_1 , SCL_1);
  Wire.beginTransmission (0x52);
  Wire.write (0x00);
  Wire.endTransmission ();
  delay(10);
}

void parse1() {
  int joy_x_axis = buffer[0]; // joystick axe x (0-255)
  int joy_y_axis = buffer[1]; // joystick axe y (0-255)
  int accel_x_axis = buffer[2] * 4 ;
  int accel_y_axis = buffer[3] * 4 ;
  int accel_z_axis = buffer[4] * 4 ;

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

#ifdef DEBUG1
  Serial.print ("BLACKNUN ");
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
  Serial.println();
#endif

  joyx1 = joy_x_axis;
  joyy1 = joy_y_axis;
  accx1 = accel_x_axis;
  accy1 = accel_y_axis;
  accz1 = accel_z_axis;
  z1 = z_button;
  c1 = c_button;

}

void parse2() {
  int joy_x_axis = buffer[0]; // joystick axe x (0-255)
  int joy_y_axis = buffer[1]; // joystick axe y (0-255)
  int accel_x_axis = buffer[2] * 4 ;
  int accel_y_axis = buffer[3] * 4 ;
  int accel_z_axis = buffer[4] * 4 ;

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

#ifdef DEBUG2
  Serial.print ("WITHENUN ");
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
  Serial.println();
#endif

  joyx2 = joy_x_axis;
  joyy2 = joy_y_axis;
  accx2 = accel_x_axis;
  accy2 = accel_y_axis;
  accz2 = accel_z_axis;
  z2 = z_button;
  c2 = c_button;

}

void loop() {
  while (WiFi.status() != 3 ) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }

  Wire.begin(SDA_0 , SCL_0);
  Wire.requestFrom (0x52, 6);
  delay(5);
  while (Wire.available ())
  {
    buffer[cnt] = Wire.read();
    cnt++;
  }
  if (cnt >= 5) {

    parse1();
  }
  cnt = 0;
  handshake1();
  delay(5);

  Wire.begin(SDA_1 , SCL_1);
  Wire.requestFrom (0x52, 6);
  delay(5);
  while (Wire.available ())
  {
    buffer[cnt] = Wire.read();
    cnt++;
  }
  if (cnt >= 5) {
    parse2();
  }
  cnt = 0;
  handshake2();
  delay (5);

  accx1 = accx1 - 512;
  accy1 = accy1 - 512;
  accz1 = accz1 - 512;
  accx2 = accx2 - 512;
  accy2 = accy2 - 512;
  accz2 = accz2 - 512;

#ifdef DEBUG3
  Serial.print ("BLACKNUN ");
  Serial.print(joyx1);
  Serial.print("\t");
  Serial.print(joyy1);
  Serial.print("\t");
  Serial.print(accx1);
  Serial.print("\t");
  Serial.print(accy1);
  Serial.print("\t");
  Serial.print(accz1);
  Serial.print("\t");
  Serial.print(z1);
  Serial.print("\t");
  Serial.print(c1);
  Serial.print("\t");
  Serial.print ("WITHENUN ");
  Serial.print(joyx2);
  Serial.print("\t");
  Serial.print(joyy2);
  Serial.print("\t");
  Serial.print(accx2);
  Serial.print("\t");
  Serial.print(accy2);
  Serial.print("\t");
  Serial.print(accz2);
  Serial.print("\t");
  Serial.print(z2);
  Serial.print("\t");
  Serial.print(c2);
  Serial.println();
#endif

  float accx = (accx1 + accx2) / 2.0;
  float accy = (accy1 + accy2) / 2.0;
  float accz = (accz1 + accz2) / 2.0;

  float PitchAngle1 = 180 * atan2(accy1, sqrt(pow(accx1, 2) + pow(accz1, 2))) / M_PI;
  float RollAngle1  = 180 * atan2(accx1, sqrt(pow(accy1, 2) + pow(accz1, 2))) / M_PI;

  float PitchAngle2 = 180 * atan2(accy2, sqrt(pow(accx2, 2) + pow(accz2, 2))) / M_PI;
  float RollAngle2  = 180 * atan2(accx2, sqrt(pow(accy2, 2) + pow(accz2, 2))) / M_PI;

  float PitchGlobal = (PitchAngle1 + PitchAngle2) / 2.0;
  float RollGlobal = (RollAngle1 + RollAngle2) / 2.0; 

  if (PitchGlobal > 0 && accz < 0) {
    PitchGlobal = 180 - PitchGlobal;
  }

  if (PitchGlobal < 0 && accz < 0) {
    PitchGlobal = -180 - PitchGlobal;
  }

  if (RollGlobal > 0 && accz < 0) {
    RollGlobal = 180 - RollGlobal;
  }

  if (RollGlobal < 0 && accz < 0) {
    RollGlobal = -180 - RollGlobal;
  }

  float Torsion = PitchAngle2 - PitchAngle1;
  float Flexion = RollAngle2  - RollAngle1;

#ifdef DEBUG4
  Serial.print("PitchGlobal : ");
  Serial.print(PitchGlobal);
  Serial.print("\t \t");
  Serial.print("RollGlobal : ");
  Serial.print(RollGlobal);
  Serial.print("\t \t");
  Serial.print("accx : ");
  Serial.print(accx);
  Serial.print("\t \t");
  Serial.print("accy : ");
  Serial.print(accy);
  Serial.print("\t \t");
  Serial.print("accz : ");
  Serial.print(accz);
  Serial.print("\t \t");
  Serial.print("Torsion : ");
  Serial.print(Torsion);
  Serial.print("\t \t");
  Serial.print("Flexion : ");
  Serial.print(Flexion);
  Serial.print("\t \t");
  Serial.println();
#endif


  OSCBundle bndl; //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
  bndl.add("Zaccel").add((float)accz / 1023);
  bndl.add("Torsion").add((float)Torsion / 180);
  bndl.add("Flexion").add((float)Flexion / 180);
  bndl.add("PitchGlobal").add((float)PitchGlobal / 180);
  bndl.add("RollGlobal").add((float)RollGlobal / 180);
  bndl.add("LeftXjoy").add((float)joyx1 / 255);
  bndl.add("LeftYjoy").add((float)joyy1 / 255);
  bndl.add("Bluebutton").add((float)c1);
  bndl.add("Yellowbutton").add((float)z1);
  bndl.add("RightXjoy").add((float)joyx2 / 255);
  bndl.add("RightYjoy").add((float)joyy2 / 255);
  bndl.add("Redbutton").add((float)c2);
  bndl.add("Greenbutton").add((float)z2);
  udp.beginPacket(udpAddress, udpPort);
  bndl.send(udp);
  udp.endPacket();
  bndl.empty();

  delay(5);
}
