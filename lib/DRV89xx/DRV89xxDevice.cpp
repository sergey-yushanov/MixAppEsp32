#include "DRV89xxDevice.h"

DRV89xxDevice::DRV89xxDevice(int cs_pin, int fault_pin, int sleep_pin)
{
    _motorDriver = new DRV89xx(cs_pin, fault_pin, sleep_pin);
}

void DRV89xxDevice::begin()
{
    _motorDriver->begin();
}

// устанавливаем значение бита в байте
uint8_t setByteBit(uint8_t number, byte shift, byte value)
{
    number ^= (-value ^ number) & (1UL << shift);
    return number;
}

void DRV89xxDevice::writeBit(byte address, byte shift, byte value)
{
    switch (address)
    {
    case DRV89xx_OP_CTRL_1:
        _opCtrl1 = setByteBit(_opCtrl1, shift, value);
        break;
    case DRV89xx_OP_CTRL_2:
        _opCtrl2 = setByteBit(_opCtrl2, shift, value);
        break;
    case DRV89xx_OP_CTRL_3:
        _opCtrl3 = setByteBit(_opCtrl3, shift, value);
        break;
    default:
        break;
    }
}

void DRV89xxDevice::writeRegisters()
{
    SPI.beginTransaction(_motorDriver->get_spi_settings());

    _motorDriver->writeRegister(DRV89xx_OP_CTRL_1, _opCtrl1);
    _motorDriver->writeRegister(DRV89xx_OP_CTRL_2, _opCtrl2);
    _motorDriver->writeRegister(DRV89xx_OP_CTRL_3, _opCtrl3);

    SPI.endTransaction();
}