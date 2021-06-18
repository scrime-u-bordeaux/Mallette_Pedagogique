#include <i2c_t3.h>

int v = 0;
int r = 0;
int b = 0;
int h = 0;
int t = 0;

int refv = 0;
int refr = 0;
int refb = 0;
int refh = 0;
int reft = 0;

// Function prototypes
void print_scan_status(uint8_t target, uint8_t all);

uint8_t found, target, all;

void setup()
{

  Serial.begin(115200);
  while (!Serial) {       // Leonardo: wait for serial monitor
    //Serial.println(F("\nI2C Scanner"));
  }
  Serial.print("\t");
  pinMode(LED_BUILTIN, OUTPUT);   // LED
  // pull pin 11 low for a more verbose result (shows both ACK and NACK)
  delay(200);
  // Setup for Master mode, pins 18/19, external pullups, 400kHz, 10ms default timeout
  Wire.begin(I2C_MASTER, 0x00, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);
  Wire.setDefaultTimeout(10000); // 10ms
  delay(10);
  Wire.beginTransmission (0x0D); // Séquence d'initialisation nunchuck2 data non-codées
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  delay(10);
  Wire.beginTransmission (0x0D);
  Wire.write (0xFB);
  Wire.write ((byte) 0x00);
  Wire.endTransmission ();
  delay(10);
}

void loop()
{
  int count = 0;
  int values[23];
  Wire.requestFrom(0x0D, 23);
  while (Wire.available())
  {
    values[count] = Wire.read();
    count++;

  }
  delay(10);
  Wire.beginTransmission (0x0D);
  Wire.write ((byte)0x00);
  Wire.endTransmission ();
  delay(10);

  if (values[19] == 1) {
    v = 1;
  }
  else {
    v = 0;
  }
  if (values[19] == 2) {
    r = 1;
  }
  else {
    r = 0;
  }
  if (values[19] == 4) {
    b = 1;
  }
  else {
    b = 0;
  }

  if ((v == 1) && (v != refv)) {
    Serial.println("Je suis le bouton vert");
    usbMIDI.sendNoteOn(59, 127, 1);
    refv = v;
  }
  if ((v == 0) && (v != refv)) {
    Serial.println("Je ne suis plus le bouton vert");
    usbMIDI.sendNoteOff(59, 127, 1);
    refv = v;
  }

  if ((r == 1) && (r != refr)) {
    Serial.println("Je suis le bouton rouge");
    usbMIDI.sendNoteOn(60, 127, 1);
    refr = r;
  }
  if ((r == 0) && (r != refr)) {
    Serial.println("Je ne suis plus le bouton rouge");
    usbMIDI.sendNoteOff(60, 127, 1);
    refr = r;
  }

  if ((b == 1) && (b != refb)) {
    Serial.println("Je suis le bouton bleu");
    usbMIDI.sendNoteOn(61, 127, 1);
    refb = b;
  }
  if ((b == 0) && (b != refb)) {
    Serial.println("Je ne suis plus le bouton bleu");
    usbMIDI.sendNoteOff(61, 127, 1);
    refb = b;
  }

  if (values[20] > 0 && values[21] == 0) {
    Serial.print("Je tourne dans le sens antitrigo à une vitesse de ");
    int val1 = map(values[20],0,90,0,127);
    val1 = constrain(val1,0,127)-1;
    Serial.print(val1);
    usbMIDI.sendControlChange(62,val1,1);
    Serial.println();
  }
  if (values[20] > 0 && values[21] == 255) {
    Serial.print("Je tourne dans le sens trigo à une vitesse de ");
    int val2 = map(255-values[20],0,90,0,127);
    val2 = constrain(val2,0,127);
    Serial.print(val2);
    usbMIDI.sendControlChange(63,val2,1);
    Serial.println();
  }
  if (values[20] == 0) {
    Serial.println("Je ne tourne pas");
  }

}
