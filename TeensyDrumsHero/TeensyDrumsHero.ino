
#include <i2c_t3.h>
#include <Bounce.h>

//#define DEBUG
#define DEFINITIVE

#define MaskRed     (1<<6)
#define MaskBlue    (1<<3)
#define MaskGreen   (1<<4)
#define MaskYellow  (1<<5)
#define MaskKick    (1<<2)
#define MaskLess    (1<<4)
#define MaskPlus    (1<<2)
#define MaskOrange  (1<<7)

int p = 0;
int l = 0;
int refp = 0;
int refl = 0;

int refred = 0;
int refblue = 0;
int refgreen = 0;
int refyellow = 0;
int reforange = 0;
int refkick = 0;

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
  Wire.beginTransmission (0x52); // Séquence d'initialisation nunchuck2 data non-codées
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  delay(10);
  Wire.beginTransmission (0x52);
  Wire.write (0xFB);
  Wire.write ((byte) 0x00);
  Wire.endTransmission ();
  //pinMode(13, OUTPUT);
  //digitalWrite(13,0);
  delay(10);
}

void loop()
{
  int count = 0;
  int values[6];
  Wire.requestFrom(0x52, 6);
  while (Wire.available())
  {
    values[count] = Wire.read();
    count++;

  }
  values[0] -= 32;
  delay(5);
  Wire.beginTransmission (0x52);
  Wire.write ((byte)0x00);
  Wire.write ((byte)0x00);
  Wire.endTransmission ();
  delay(5);

#ifdef DEBUG
  Serial.print(values[0], DEC); Serial.print("\t"); //joyX
  Serial.print(values[1], DEC); Serial.print("\t"); //joyY
  Serial.print(values[2], DEC ); Serial.print("\t");
  Serial.print(values[3], BIN); Serial.print("\t");
  Serial.print(values[4], BIN); Serial.print("\t");
  Serial.print(values[5], BIN); Serial.print("\t");
  Serial.println();
  delay(100);
#endif



#ifdef DEFINITIVE

  // Traitement des boutons

  if (MaskPlus & ~values[3]) {
    p = 1;
  }
  else {
    p = 0;
  }

  if (MaskLess & ~values[3]) {
    l = 1;
  }
  else {
    l = 0;
  }

  if ((p == 1) && (p != refp)) {
    Serial.println("Je suis le bouton plus");
    usbMIDI.sendNoteOn(59, 127, 1);
    refp = p;
  }

  if ((p == 0) && (p != refp)) {
    Serial.println("Je ne suis plus le bouton plus");
    usbMIDI.sendNoteOff(59, 127, 1);
    refp = p;
  }

  if ((l == 1) && (l != refl)) {
    Serial.println("Je suis le bouton moins");
    usbMIDI.sendNoteOn(58, 127, 1);
    refl = l;
  }

  if ((l == 0) && (l != refl)) {
    Serial.println("Je ne suis plus le bouton moins");
    usbMIDI.sendNoteOff(58, 127, 1);
    refl = l;
  }

  // Traitement des percs

  if ((MaskRed & ~values[4]) && (values[1] != 255)) {
    Serial.println("Je suis le tom rouge");
    usbMIDI.sendNoteOn(60, (255 - values[2])/2, 1);
    refred = 1;
  }

  if ((MaskRed & ~values[4]) && (refred == 1)) {
    Serial.println("Je ne suis plus le tom rouge");
    usbMIDI.sendNoteOff(60, (255 - values[2])/2, 1);
    refred = 0;
  }

  if ((MaskBlue & ~values[4]) && (values[1] != 255)) {
    Serial.println("Je suis le tom bleu");
    usbMIDI.sendNoteOn(61, (255 - values[2])/2, 1);
    refblue = 1;
  }

  if ((MaskBlue & ~values[4]) && (refblue == 1)) {
    Serial.println("Je ne suis plus le tom bleu");
    usbMIDI.sendNoteOff(61, (255 - values[2])/2, 1);
    refblue = 0;
  }

  if ((MaskGreen & ~values[4]) && (values[1] != 255)) {
    Serial.println("Je suis le tom vert");
    usbMIDI.sendNoteOn(62, (255 - values[2])/2, 1);
    refgreen = 1;
  }

  if ((MaskGreen & ~values[4]) && (refgreen == 1)) {
    Serial.println("Je ne suis plus le tom vert");
    usbMIDI.sendNoteOff(62, (255 - values[2])/2, 1);
    refgreen = 0;
  }

  if ((MaskYellow & ~values[4]) && (values[1] != 255)) {
    Serial.println("Je suis la symbale jaune");
    usbMIDI.sendNoteOn(63, (255 - values[2])/2, 1);
    refyellow = 1;
  }

  if ((MaskYellow & ~values[4]) && (refyellow == 1)) {
    Serial.println("Je ne suis plus la symbale jaune");
    usbMIDI.sendNoteOff(63, (255 - values[2])/2, 1);
    refyellow = 0;
  }

  if ((MaskOrange & ~values[4]) && (values[1] != 255)) {
    Serial.println("Je suis la symbale orange");
    usbMIDI.sendNoteOn(64, (255 - values[2])/2, 1);
    reforange = 1;
  }

  if ((MaskOrange & ~values[4]) && (reforange == 1)) {
    Serial.println("Je ne suis plus la symbale orange");
    usbMIDI.sendNoteOff(64, (255 - values[2])/2, 1);
    reforange = 0;
  }

  if ((MaskKick & ~values[4]) && (values[1] != 255)) {
    Serial.println("Je suis le kick");
    usbMIDI.sendNoteOn(65, (255 - values[2])/2, 1);
    refkick = 1;
  }

  if ((MaskKick & ~values[4]) && (refkick == 1)) {
    Serial.println("Je ne suis plus le kick");
    usbMIDI.sendNoteOff(65, (255 - values[2])/2, 1);
    refkick = 0;
  }

  // Traitement du joystick  

  if (values[0] != 0) {
    Serial.println(values[0]);
  }

  usbMIDI.sendControlChange(57, values[0], 1);
#endif

}
