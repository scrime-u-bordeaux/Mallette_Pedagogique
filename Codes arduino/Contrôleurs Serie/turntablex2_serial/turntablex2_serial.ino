#include <Wire.h>
#include <MIDI.h>
#include <Bounce.h>

//#define DEBUG

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

int v1 = 0;
int r1 = 0;
int b1 = 0;
int h1 = 0;
int t1 = 0;

int refv1 = 0;
int refr1 = 0;
int refb1 = 0;
int refh1 = 0;
int reft1 = 0;

int refval1 = 0;
int refval2 = 0;
int refval3 = 0;
int refval4 = 0;

Bounce boutton = Bounce(17,10);

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

void setup()
{
  //Serial.begin(115200);
  Serial1.begin(31250);
  MIDI.begin();
  Serial.print("\t");
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(17, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);   // LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);

  Wire.begin();
  Wire.beginTransmission (0x0D); // SÃ©quence d'initialisation nunchuck1 data non-codÃ©es
  Wire.write (0xF0);
  Wire.write (0x55);
  Wire.endTransmission ();
  Wire.beginTransmission (0x0D);
  Wire.write (0xFB);
  Wire.write (0x00);
  Wire.endTransmission ();

  Wire1.begin();
  Wire1.beginTransmission (0x0D); // SÃ©quence d'initialisation nunchuck1 data non-codÃ©es
  Wire1.write (0xF0);
  Wire1.write (0x55);
  Wire1.endTransmission ();
  Wire1.beginTransmission (0x0D);
  Wire1.write (0xFB);
  Wire1.write (0x00);
  Wire1.endTransmission ();

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
    //Serial.println("Je suis le bouton vert");
    MIDI.sendNoteOn(61, 127, 1);
    refv = v;
  }
  if ((v == 0) && (v != refv)) {
    //Serial.println("Je ne suis plus le bouton vert");
    MIDI.sendNoteOff(61, 0, 1);
    refv = v;
  }

  if ((r == 1) && (r != refr)) {
    //Serial.println("Je suis le bouton rouge");
    MIDI.sendNoteOn(62, 127, 1);
    refr = r;
  }
  if ((r == 0) && (r != refr)) {
    //Serial.println("Je ne suis plus le bouton rouge");
    MIDI.sendNoteOff(62, 0, 1);
    refr = r;
  }

  if ((b == 1) && (b != refb)) {
    //Serial.println("Je suis le bouton bleu");
    MIDI.sendNoteOn(63, 127, 1);
    refb = b;
  }
  if ((b == 0) && (b != refb)) {
    //Serial.println("Je ne suis plus le bouton bleu");
    MIDI.sendNoteOff(63, 0, 1);
    refb = b;
  }

  if (values[20] > 0 && values[21] == 0) {
    //Serial.print("Je tourne dans le sens antitrigo à une vitesse de ");
    int val1 = map(values[20], 0, 90, 0, 127);
    val1 = val1*0.9+0.1*refval1;
    refval1 = val1;
    val1 = constrain(val1, 0, 127);
    //Serial.print(val1);
    MIDI.sendControlChange(61, val1, 1);
    MIDI.sendControlChange(62, 0, 1);
    //Serial.println();
  }
  if (values[20] > 0 && values[21] == 255) {
    //Serial.print("Je tourne dans le sens trigo à une vitesse de ");
    int val2 = map(255 - values[20], 0, 90, 0, 127);
    val2 = val2*0.9+0.1*refval2;
    refval2 = val2;
    val2 = constrain(val2, 0, 127);
    //Serial.print(val2);
    MIDI.sendControlChange(62, val2, 1);
    MIDI.sendControlChange(61, 0, 1);
    //Serial.println();
  }
  if (values[20] == 0) {
    //Serial.println("Je ne tourne pas");
  }

  int count1 = 0;
  int values1[23];
  Wire1.requestFrom(0x0D, 23);
  while (Wire1.available())
  {
    values1[count1] = Wire1.read();
    count1++;
  }
  delay(10);
  Wire1.beginTransmission (0x0D);
  Wire1.write ((byte)0x00);
  Wire1.endTransmission ();
  delay(10);

  if (values1[19] == 1) {
    v1 = 1;
  }
  else {
    v1 = 0;
  }
  if (values1[19] == 2) {
    r1 = 1;
  }
  else {
    r1 = 0;
  }
  if (values1[19] == 4) {
    b1 = 1;
  }
  else {
    b1 = 0;
  }

  if ((v1 == 1) && (v1 != refv1)) {
    //Serial.println("Je suis le bouton vert");
    MIDI.sendNoteOn(64, 127, 1);
    refv1 = v1;
  }
  if ((v1 == 0) && (v1 != refv1)) {
    //Serial.println("Je ne suis plus le bouton vert");
    MIDI.sendNoteOff(64, 0, 1);
    refv1 = v1;
  }

  if ((r1 == 1) && (r1 != refr1)) {
    //Serial.println("Je suis le bouton rouge");
    MIDI.sendNoteOn(65, 127, 1);
    refr1 = r1;
  }
  if ((r1 == 0) && (r1 != refr1)) {
    //Serial.println("Je ne suis plus le bouton rouge");
    MIDI.sendNoteOff(65, 0, 1);
    refr1 = r1;
  }

  if ((b1 == 1) && (b1 != refb1)) {
    //Serial.println("Je suis le bouton bleu");
    MIDI.sendNoteOn(66, 127, 1);
    refb1 = b1;
  }
  if ((b1 == 0) && (b1 != refb1)) {
    //Serial.println("Je ne suis plus le bouton bleu");
    MIDI.sendNoteOff(66, 0, 1);
    refb1 = b1;
  }

  if (values1[20] > 0 && values1[21] == 0) {
    //Serial.print("Je tourne dans le sens antitrigo à une vitesse de ");
    int val3 = map(values1[20], 0, 90, 0, 127);
    val3 = val3*0.9+0.1*refval3;
    refval3 = val3;
    val3 = constrain(val3, 0, 127);
    //Serial.print(val3);
    MIDI.sendControlChange(64, val3, 1);
    MIDI.sendControlChange(65, 0, 1);
    //Serial.println();
  }
  if (values1[20] > 0 && values1[21] == 255) {
    //Serial.print("Je tourne dans le sens trigo à une vitesse de ");
    int val4 = map(255 - values1[20], 0, 90, 0, 127);
    val4 = val4*0.9+0.1*refval4;
    refval4 = val4;
    val4 = constrain(val4, 0, 127);
    //Serial.print(val4);
    MIDI.sendControlChange(65, val4, 1);
    MIDI.sendControlChange(64, 0, 1);
    //Serial.println();
  }
  if (values1[20] == 0) {
    //Serial.println("Je ne tourne pas");
  }
  
  int potar = 127 - (analogRead(A1) / 8);
  potar = constrain(potar, 0, 127);
  MIDI.sendControlChange(63, potar, 1);
  //Serial.println(potar);

  int fader = analogRead(A0) / 8;
  fader = constrain(fader, 0, 127);
  MIDI.sendControlChange(66, fader, 1);
  //Serial.println(fader);

  boutton.update();
  if(boutton.fallingEdge()){
    MIDI.sendNoteOn(67,127,1);
    //Serial.println("Je suis le bouton noir");
  } else if (boutton.risingEdge()){
    MIDI.sendNoteOff(67,0,1);
    //Serial.println("Je ne suis plus le bouton noir");
  }
 
//Serial.print(values1[19]);
//Serial.print("\t");
//Serial.print(values1[20]);
//Serial.print("\t");
//Serial.print(values1[21]);
//Serial.println();
}
