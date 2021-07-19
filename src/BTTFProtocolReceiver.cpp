#include <BTTFProtocolReceiver.h>

#define RX_BUF_SIZE 4

BTTFProtocolReceiver::BTTFProtocolReceiver(SoftwareSerial *rxSer, void (*speedCb)(uint8_t), void (*timewarpCb)(void)) 
{
    this->rxSer = rxSer;
    this->speedCallback = speedCb;
    this->timewarpCallback = timewarpCb;
    this->rxBuf = (char*)calloc(RX_BUF_SIZE, sizeof(char)); // Messages are three chars, plus CRLF is 4
    this->rxBufSize = 0;
}

void BTTFProtocolReceiver::doReceive() 
{
    if(rxSer->available() > 0){
        // We got a char, store it.
        char x = rxSer->read();
        if(x == '\n'){
            // Command complete, do check it and act accordingly!
            if(this->isSpeed()){
                this->speedCallback(this->getSpeed());
            } else if(this->isTimewarp()){
                this->timewarpCallback();
            } else {
                // Seems we got shit!
                // Log to serial and go on...
                Serial.print(F("Unknown command: \""));
                Serial.print(rxBuf);
                Serial.println(F("\""));
            }
            this->clearBuffer();
        } else {
            if(rxBufSize >= RX_BUF_SIZE){
                // Overflow, clear buffer and go on.
                // Should not really happen actually, so let's debug to serial and be done.
                Serial.print(F("RX Buffer overflow: \""));
                Serial.print(rxBuf);
                Serial.println(F("\""));
                Serial.flush();
                this->clearBuffer();
            }
            this->rxBuf[rxBufSize++] = x;
        }
    }
}


boolean BTTFProtocolReceiver::isSpeed() 
{
    return rxBuf[0] == 'S'
        && rxBuf[1] >= 48
        && rxBuf[1] <= 57
        && rxBuf[2] >= 48
        && rxBuf[2] <= 57;
}

boolean BTTFProtocolReceiver::isTimewarp() 
{
    return rxBuf[0] == 'T'
        && rxBuf[1] == 'W'
        && rxBuf[2] == '!';
}

uint8_t BTTFProtocolReceiver::getSpeed() 
{
    uint8_t ret = (rxBuf[1] - 48);
    ret *= 10;
    ret += (rxBuf[2] - 48);
    return ret;
}

void BTTFProtocolReceiver::clearBuffer() 
{
    memset(this->rxBuf, 0, RX_BUF_SIZE);
    rxBufSize = 0;
}
