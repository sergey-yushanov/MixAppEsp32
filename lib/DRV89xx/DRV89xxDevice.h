#ifndef DRV89xxDevice_h
#define DRV89xxDevice_h

#include <DRV89xx.h>

#define DRV89xx_OP_CTRL_1 0x08
#define DRV89xx_OP_CTRL_2 0x09
#define DRV89xx_OP_CTRL_3 0x0A

class DRV89xxDevice
{
public:
    DRV89xxDevice(int cs_pin, int fault_pin, int sleep_pin);
    void begin();
    void writeBit(byte address, byte shift, byte value);
    void writeRegisters();

private:
    DRV89xx *_motorDriver;
    byte _opCtrl1 = 0;
    byte _opCtrl2 = 0;
    byte _opCtrl3 = 0;
};

#endif
