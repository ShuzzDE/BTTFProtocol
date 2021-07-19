#ifndef __BTTFPROTOCOLSENDER_H__
#define __BTTFPROTOCOLSENDER_H__

#include <Arduino.h>
#include <SoftwareSerial.h>

class BTTFProtocolSender
{
private:
    SoftwareSerial *txSer;

public:
    BTTFProtocolSender(SoftwareSerial *ser);
    boolean sendSpeed(uint8_t speed);
    boolean triggerTimewarp();
};





#endif // __BTTFPROTOCOLSENDER_H__