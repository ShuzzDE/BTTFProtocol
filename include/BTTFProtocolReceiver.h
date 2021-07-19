#ifndef __BTTFPROTOCOLRECEIVER_H__
#define __BTTFPROTOCOLRECEIVER_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

class BTTFProtocolReceiver
{
private:
    SoftwareSerial *rxSer;
    char *rxBuf;
    uint8_t rxBufSize;
    void (*speedCallback)(uint8_t);
    void (*timewarpCallback)(void);

    boolean isSpeed();
    boolean isTimewarp();
    uint8_t getSpeed();
    void clearBuffer();

public:
    BTTFProtocolReceiver(SoftwareSerial *rxSer, void (*speedCb)(uint8_t), void (*timewarpCb)(void));
    void doReceive(); // Call this OFTEN!
};




#endif // __BTTFPROTOCOLRECEIVER_H__