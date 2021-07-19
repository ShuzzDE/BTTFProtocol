#include <Arduino.h>
#include <SoftwareSerial.h>
#include <BTTFProtocolReceiver.h>
#include <BTTFProtocolSender.h>

#define RX_1 3
#define TX_1 2


// We need two SoftSer for this test because one must be inverted - and inversion always affects RX AND TX -.-

//SoftwareSerial txSer = SoftwareSerial(RX_2, TX_2);
SoftwareSerial *rxSer; // Need to invert this!

char buffer[64];
uint8_t bufSize = 0;


BTTFProtocolReceiver *bttfReceiver = NULL;
BTTFProtocolSender *bttfSender = NULL;




#define BAUD_RATE 19200


void resetBuffer(){
  memset(&buffer, 0, sizeof(buffer));  // Clear buffer
  bufSize = 0;
}

boolean isReader = true;
uint32_t ts = 0;
uint32_t numMsg = 0;

void speedCallback(uint8_t speed){
  // Serial.print(F("New Speed: "));
  // Serial.print(speed);
  // Serial.println(F("mph"));
  numMsg++;
}
void timewarpCallback(void){
  // Serial.println("TIMEWARP!!!!");
  uint32_t delta = millis() - ts;
  ts = millis();
  numMsg++;
  uint32_t msgPerSec = numMsg * 1000;
  msgPerSec /= delta;
  Serial.println(msgPerSec);
  numMsg = 0;
}

void setup() {
  Serial.begin(115200);
  pinMode(RX_1, INPUT_PULLUP);
  pinMode(TX_1, OUTPUT);
  pinMode(13, OUTPUT);
  resetBuffer();


  pinMode(A0, INPUT_PULLUP);

  delay(100);
  isReader = (digitalRead(A0) == LOW);

  rxSer = &SoftwareSerial(RX_1, TX_1, !isReader);
  rxSer->begin(BAUD_RATE);


  if(isReader){
    Serial.print(F("READER "));
    bttfReceiver = new BTTFProtocolReceiver(rxSer, &speedCallback, &timewarpCallback);
  } else {
    Serial.print(F("WRITER "));
    bttfSender = new BTTFProtocolSender(rxSer);
  }
  Serial.println(F("Startup!"));
}


uint8_t speed = 0;

void loop() {

  if(! isReader){
    uint32_t ms = millis();
    if(ms - 10 >= ts){
      ts = ms;
      digitalWrite(13, HIGH);
      if(speed > 88){
        bttfSender->triggerTimewarp();
        speed = 0;
      } else {
        bttfSender->sendSpeed(speed);
        speed += 8;  
      }
      digitalWrite(13, LOW);
    }
  } else {
    bttfReceiver->doReceive();
    delay(1);
  }
}