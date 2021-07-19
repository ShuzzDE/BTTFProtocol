#include <BTTFProtocolSender.h>

boolean BTTFProtocolSender::sendSpeed(uint8_t speed) 
{
    if(speed > 88) return false; // >88mph is not allowed, signal error!
    txSer->print('S');
    txSer->print(speed/10);
    txSer->print(speed%10);
    txSer->print('\n');
    return true;
}

boolean BTTFProtocolSender::triggerTimewarp() 
{
    txSer->print('T');
    txSer->print('W');
    txSer->print('!');
    txSer->print('\n');
    return true;
}

BTTFProtocolSender::BTTFProtocolSender(SoftwareSerial *ser) 
{
    this->txSer = ser;
}
