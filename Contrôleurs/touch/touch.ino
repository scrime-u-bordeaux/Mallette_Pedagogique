int pin[10] = {0, 1, 3, 4, 15, 16, 17, 18, 19, 22};
int touch[10] = {0};
int lastTouch[10] = {0};
int calib[10] = {0};
int midiCC[10] = {0};

void setup() {
  Serial.begin(9600);
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(22, INPUT);
  calibration();
}

void loop() {
  for (int i = 0; i < 10; i++) {
    touch[i] = touchRead(pin[i]);
    touch[i] = touch[i] * 0.9 + lastTouch[i] * 0.1;
    lastTouch[i] = touch[i];
  }
  for (int i = 0; i < 10; i++) {
    midiCC[i] = (touch[i] - 1.1 * calib[i]) * 255 / calib[i];
    midiCC[i] = constrain( midiCC[i], 0, 127);
    usbMIDI.sendControlChange(101 + i, midiCC[i], 1); 
    Serial.print(midiCC[i]);
    Serial.print("\t");
  }
  Serial.println();
  
  delay(20);
}

void calibration() {
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 10; j++) {
      calib[j] = calib[j] + touchRead(pin[j]);
    }
    delay(10);
  }
  for (int i = 0; i < 10; i++) {
    calib[i] = calib[i] / 100;
  }
}
