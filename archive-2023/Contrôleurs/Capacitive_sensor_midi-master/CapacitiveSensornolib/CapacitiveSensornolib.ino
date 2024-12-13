#include <Smoothed.h>
Smoothed <uint32_t> cs0av;
Smoothed <uint32_t> cs0ex;

uint32_t cs1;
byte out[8] = {29, 2, 4, 6, 8, 10, 24, 26};
byte in[8] = {30, 3, 5, 7, 9, 11, 25, 27};
int i;
uint32_t cs[8];

void setup()
{
  //cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(115200);
  for (i = 0; i < 8; i++) {
    pinMode(in[i], INPUT);
    pinMode(out[i], OUTPUT);
    digitalWrite(out[i], LOW);
  }
  cs0av.begin(SMOOTHED_AVERAGE, 15);
  cs0ex.begin(SMOOTHED_EXPONENTIAL, 10);
  delay(1000);
  Serial.print("go2");
}
void capsens(int i) {
  cs1 = 0;
  noInterrupts();
  pinMode(in[i], INPUT);
  digitalWrite(out[i], HIGH);
  while (digitalReadFast(in[i]) == LOW) {
    cs1 = cs1 + 1;
  }
  interrupts();
  digitalWrite(out[i], LOW);
  pinMode(in[i], OUTPUT); digitalWrite(in[i], LOW);
  cs[i] = cs1;
}
void loop()
{
  long start = millis();
//  for (i = 0; i < 8; i++) {
//    capsens(i);
//  }
  //Serial.print(millis() - start);
 // Serial.print("\t");
 capsens(0);
  cs0av.add(cs[0]);
  cs0ex.add(cs[0]);
  Serial.print(cs[0]);
  Serial.print("\t");
  Serial.print(cs0av.get());
  Serial.print("\t");
   Serial.print(cs0ex.get());
  //   for (i = 0; i < 8; i++) {
  //     Serial.print("\t");
  //  Serial.print(cs[i]);
  //  }
  Serial.println();
  // print sensor output 1

  //    Serial.print(total2);                  // print sensor output 2
  //    Serial.print("\t");
  //                  // print sensor output 3

  delay(10);                             // arbitrary delay to limit data to serial port
}
