#include "mbed.h"
#include "MMA8451Q.h"
#include "USBSerial.h"

#define MMA8451_I2C_ADDRESS (0x1d<<1)


PinName const SDA = PTE25;
PinName const SCL = PTE24;


const float samplingFrequency = 1.0f;
const float period = 1.0f / samplingFrequency; 

MMA8451Q acc(SDA, SCL, MMA8451_I2C_ADDRESS);

USBSerial usb; // For testing only
Serial uart(USBTX, USBRX, 9600);

float accValues[3];

void sendFloat(float *value, Stream *serial)
{
    char * address = (char *) value;
    
    for (int i = sizeof(float)/sizeof(char) - 1; i >= 0; --i)
        serial->putc(address[i]);
}

void sendAC(Stream *serial)
{
    serial->putc('x');
    sendFloat(&accValues[0], serial);
    serial->putc('y');
    sendFloat(&accValues[1], serial);
    serial->putc('z');
    sendFloat(&accValues[2], serial);
}

int main(void)
{
    while(true)
    {
        acc.getAccAllAxis(accValues);
        
        // For testing only
        sendAC(&usb);
        usb.printf("\r\n");
        
        sendAC(&uart);
        
        wait(period);
    }
}
