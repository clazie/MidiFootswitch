#include <Arduino.h>
#include <Bounce.h>  // Bounce library makes button change detection easy

struct tSwitch{
  const int pin;
  int value;
  int state;
  int oldstate;
  int debounce;
};

const int channel = 1;
const int debouncecount = 10;

tSwitch switches[] = {
  { 2, 1, HIGH, HIGH },
  { 3, 2, HIGH, HIGH },
  { 4, 3, HIGH, HIGH },
  { 5, 4, HIGH, HIGH },
  { 6, 5, HIGH, HIGH },
  { 7, 6, HIGH, HIGH },
  { 8, 7, HIGH, HIGH },
  { 9, 8, HIGH, HIGH },
};

void CheckSwitch(int nr){
  switches[nr].state = digitalRead(switches[nr].pin);
  if(switches[nr].state != switches[nr].oldstate){
    if (switches[nr].state) {
      switches[nr].debounce++;
      if(switches[nr].debounce > debouncecount){
        usbMIDI.sendControlChange(switches[nr].value, 0, channel);
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print(nr);
        Serial.println(" Off");
        switches[nr].debounce = 0;
        switches[nr].oldstate = switches[nr].state;
      }
      delay(1);
    }
    else{
      switches[nr].debounce++;
      if(switches[nr].debounce > debouncecount){
        usbMIDI.sendControlChange(switches[nr].value, 99, channel);
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.print(nr);
        Serial.println(" On");
        switches[nr].debounce = 0;
        switches[nr].oldstate = switches[nr].state;
      }
      delay(1);
    }
  }
}

void setup() {
  usbMIDI.begin();
  Serial.begin(115200);
  for( int i=0; i<8; i++){
    pinMode(switches[i].pin, INPUT_PULLUP);
  }

  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
    // MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
  }
  
  for( int i=0; i<8; i++){
    CheckSwitch(i);
  }
}