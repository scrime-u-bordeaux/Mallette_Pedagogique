#include <MIDI.h>

void setup() {
  Serial1.begin(31250);
  Serial2.begin(31250);
  Serial3.begin(31250);
  Serial4.begin(31250);
  Serial5.begin(31250);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

  if (Serial1.available() >= 3) {
    byte type_channel = Serial1.read();
    if (type_channel >= 127) {
      int numero = Serial1.read();
      int velocite = Serial1.read();
      int Type = type_channel >> 4;
      int channel = type_channel & B00001111 ;
      channel = channel + 1;
      Serial.println(String("Type: ") + Type + ", Channel = " + channel + ", Numero = " + numero + ", Vélocité = " + velocite);
      if (Type == 11) { // Control Change
        usbMIDI.sendControlChange(numero, velocite, channel);
      }
      if (Type == 9) { // Note On
        usbMIDI.sendNoteOn(numero, velocite, channel);
      }
      if (Type == 8) { // Note Off
        usbMIDI.sendNoteOff(numero, velocite, channel);
      }
      if (Type == 14) { // Pitch Bend
        usbMIDI.sendPitchBend(velocite, channel);
      }
    }
  }

  if (Serial2.available() >= 3) {
    byte type_channel = Serial2.read();
    if (type_channel >= 127) {
      int numero = Serial2.read();
      int velocite = Serial2.read();
      int Type = type_channel >> 4;
      int channel = type_channel & B00001111 ;
      channel = channel + 1;
      Serial.println(String("Type: ") + Type + ", Channel = " + channel + ", Numero = " + numero + ", Vélocité = " + velocite);
      if (Type == 11) { // Control Change
        usbMIDI.sendControlChange(numero, velocite, channel);
      }
      if (Type == 9) { // Note On
        usbMIDI.sendNoteOn(numero, velocite, channel);
      }
      if (Type == 8) { // Note Off
        usbMIDI.sendNoteOff(numero, velocite, channel);
      }
      if (Type == 14) { // Pitch Bend
        usbMIDI.sendPitchBend(velocite, channel);
      }
    }
  }

  if (Serial3.available() >= 3) {
    byte type_channel = Serial3.read();
    if (type_channel >= 127) {
      int numero = Serial3.read();
      int velocite = Serial3.read();
      int Type = type_channel >> 4;
      int channel = type_channel & B00001111 ;
      channel = channel + 1;
      Serial.println(String("Type: ") + Type + ", Channel = " + channel + ", Numero = " + numero + ", Vélocité = " + velocite);
      if (Type == 11) { // Control Change
        usbMIDI.sendControlChange(numero, velocite, channel);
      }
      if (Type == 9) { // Note On
        usbMIDI.sendNoteOn(numero, velocite, channel);
      }
      if (Type == 8) { // Note Off
        usbMIDI.sendNoteOff(numero, velocite, channel);
      }
      if (Type == 14) { // Pitch Bend
        usbMIDI.sendPitchBend(velocite, channel);
      }
    }
  }

  if (Serial4.available() >= 3) {
    byte type_channel = Serial4.read();
    if (type_channel >= 127) {
      int numero = Serial4.read();
      int velocite = Serial4.read();
      int Type = type_channel >> 4;
      int channel = type_channel & B00001111 ;
      channel = channel + 1;
      Serial.println(String("Type: ") + Type + ", Channel = " + channel + ", Numero = " + numero + ", Vélocité = " + velocite);
      if (Type == 11) { // Control Change
        usbMIDI.sendControlChange(numero, velocite, channel);
      }
      if (Type == 9) { // Note On
        usbMIDI.sendNoteOn(numero, velocite, channel);
      }
      if (Type == 8) { // Note Off
        usbMIDI.sendNoteOff(numero, velocite, channel);
      }
      if (Type == 14) { // Pitch Bend
        usbMIDI.sendPitchBend(velocite, channel);
      }
    }
  }

  if (Serial5.available() >= 3) {
    byte type_channel = Serial5.read();
    if (type_channel >= 127) {
      int numero = Serial5.read();
      int velocite = Serial5.read();
      int Type = type_channel >> 4;
      int channel = type_channel & B00001111 ;
      channel = channel + 1;
      Serial.println(String("Type: ") + Type + ", Channel = " + channel + ", Numero = " + numero + ", Vélocité = " + velocite);
      if (Type == 11) { // Control Change
        usbMIDI.sendControlChange(numero, velocite, channel);
      }
      if (Type == 9) { // Note On
        usbMIDI.sendNoteOn(numero, velocite, channel);
      }
      if (Type == 8) { // Note Off
        usbMIDI.sendNoteOff(numero, velocite, channel);
      }
      if (Type == 14) { // Pitch Bend
        usbMIDI.sendPitchBend(velocite, channel);
      }
    }
  }
}
