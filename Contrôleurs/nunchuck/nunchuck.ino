#include <i2c_t3.h>

//#define DEBUG

double accel_x_axis, accel_y_axis, accel_z_axis ;
byte z_button ; // bouton Z
byte c_button ; // bouton c
byte buffer[6]; // Buffer contenant les 6 prÃ©cieux octets qui nous intÃ©resse
byte cnt = 0; // index courant de buffer
bool refz = LOW;
bool refc = LOW;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission (0x52); // SÃ©quence d'initialisation nunchuck1 data non-codÃ©es
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  Wire.beginTransmission (0x52);
  Wire.write (0xFB);
  Wire.write (0x00);
  Wire.endTransmission ();
}

void handshake() // Handshack, merci captain obvious !
{
  Wire.begin();
  Wire.beginTransmission (0x52);
  Wire.write (0x00);
  Wire.endTransmission ();
  delay(10);
}

void parse() {
  double joy_x_axis = buffer[0]; // joystick axe x (0-255)
  double joy_y_axis = buffer[1]; // joystick axe y (0-255)
  accel_x_axis = buffer[2] * 4 ; // accÃ©lÃ©romÃ¨tre axe x
  accel_y_axis = buffer[3] * 4 ; // accÃ©lÃ©romÃ¨tre axe y
  accel_z_axis = buffer[4] * 4 ; // accÃ©lÃ©romÃ¨tre axe z
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
#ifdef DEBUG
  Serial.print ("BLACKNUN");
  Serial.print ("\t");
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
}

void loop() {
  
  Wire.begin();
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

  accel_x_axis -= 512;
  accel_y_axis -= 512;
  accel_z_axis -= 512;

  float PitchAngle = 180 * atan (accel_x_axis/sqrt(accel_y_axis*accel_y_axis + accel_z_axis * accel_z_axis))/M_PI;
  float RollAngle = 180 * atan (accel_y_axis/sqrt(accel_x_axis*accel_x_axis + accel_z_axis * accel_z_axis))/M_PI;
  
  //float RollAngle = atan2(AccY , AccZ);
  //float PitchAngle = atan2((- AccX) , sqrt(AccY * AccY + AccZ * AccZ));

  /*
    if (AccX >= 126 || AccY >= 126 || AccZ >= 126) {
    Serial.print("JoyX = "); Serial.println(JoyX);              //Joystick axe X
    Serial.print("JoyY = "); Serial.println(JoyY);              //Joystick axe Y
    Serial.print("RollAngle = "); Serial.println(RollAngle);    //Angle de roulis
    Serial.print("PitchAngle = "); Serial.println(PitchAngle);  //Angle d'inclinaison
    Serial.print("AccX = "); Serial.println(AccX);              //Acceleration axe X
    Serial.print("AccY = "); Serial.println(AccY);              //Acceleration axe Y
    Serial.print("AccZ = "); Serial.println(AccZ);              //Acceleration axe Z
    Serial.print("BoutonZ = "); Serial.println(z);              //Bouton Z
    Serial.print("BoutonC = "); Serial.println(c);              //Bouton C
    }
  */

  // Envoi du bouton Z
  if ((z == 1) && (z != refz)) {
    Serial.println("Je suis le bouton Z");
    usbMIDI.sendControlChange(48, z, 1);
    refz = z;
  }

  if ((z == 0) && (z != refz)) {
    Serial.println("Je suis le bouton Z");
    usbMIDI.sendControlChange(48, 0, 1);
    refz = z;
  }

  // Envoi du bouton C
  if ((c == 1) && (c != refc)) {
    Serial.println("Je suis le bouton C");
    usbMIDI.sendControlChange(48, c, 2);
  }

  if ((c == 0) && (c != refc)) {
    Serial.println("Je suis le bouton 0");
    usbMIDI.sendControlChange(48, c, 1);
    refc = c;
  }

  // Envoi de JoyX
  usbMIDI.sendControlChange(48, JoyX, 3);

  // Envoi de JoyY
  usbMIDI.sendControlChange(48, JoyY, 4);

  // Envoi de AccX
  usbMIDI.sendControlChange(48, AccX, 5);

  // Envoi de AccY
  usbMIDI.sendControlChange(48, AccY, 6);

  // Envoi de AccZ
  usbMIDI.sendControlChange(48, AccZ, 7);

  // Envoi de RollAngle
  usbMIDI.sendControlChange(48, RollAngle, 8);

  // Envoi de PitchAngle
  usbMIDI.sendControlChange(48, PitchAngle, 9);

  */
  
  /*
  Serial.print(RollAngle);
  Serial.print("\t");
  Serial.print(PitchAngle);
  Serial.println();
  delay(100);
  */

  //refc = c;
  
}
