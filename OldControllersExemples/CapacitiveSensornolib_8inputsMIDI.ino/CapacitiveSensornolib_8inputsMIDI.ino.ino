uint32_t csc;
byte out[8] = {0, 2, 4, 6, 8, 10, 24, 26};
byte in[8] = {1, 3, 5, 7, 9, 11, 25, 27};
float lastcs[8];
float cs[8];
long midiCC[8];
int calib[8];
int i;
void setup()
{
  pinMode(13, OUTPUT);
  //Serial.begin(115200);
//  while (!Serial) {
//    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
//    delay(100);               // wait for a second
//    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
//    delay(100);
//  }
//  Serial.println("go");
  for (i = 0; i < 8; i++) {
    pinMode(in[i], INPUT);
    pinMode(out[i], OUTPUT);
    digitalWrite(out[i], LOW);
  }
  delay(1000);
  //Serial.println("go");
  calibration();
}
void capsens(int i) {
  csc = 0;
  noInterrupts();
  pinMode(in[i], INPUT);
  digitalWrite(out[i], HIGH);
  while (digitalReadFast(in[i]) == LOW) {
    csc = csc + 1;
  }
  interrupts();
  digitalWrite(out[i], LOW);
  pinMode(in[i], OUTPUT); digitalWrite(in[i], LOW);
  cs[i] = csc;

}
void calibration() {
  for (i = 0; i < 8; i++) {
    calib[i] = 0;
  }
  for (int n = 0; n < 100; n++) {
    for (int m = 0; m < 8; m++) {
      capsens(m);
      calib[m] = calib[m] + cs[m];
    }
    delay(10);
  }
  for (i = 0; i < 8; i++) {
    calib[i] = calib[i] / 100;
  }
}
void loop()
{
  for (i = 0; i < 8; i++) {
    capsens(i);
    cs[i] = 0.9 * lastcs[i] + 0.1 * cs[i];
    //Serial.print("\t");
    //Serial.print(cs[i]);
    lastcs[i] = cs[i];
  }
  //Serial.println();
  for (i = 0; i < 8; i++) { ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!i < nb_cs;
    midiCC[i] = (cs[i] - 1.1 * calib[i]) * 512 / calib[i];
    midiCC[i] = constrain( midiCC[i], 0, 127);
    //Serial.print("\t");
    //Serial.print(cs[i]);
    //Serial.print(midiCC[i]);
    usbMIDI.sendControlChange(101 + i, midiCC[i], 1); //Channel 1
  }
  delay(10);                             // arbitrary delay to limit data to serial port
}
