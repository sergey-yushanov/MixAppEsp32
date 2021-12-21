#include "DRV89xx.h"
#include "DRV89xxRegister.h"

DRV89xx::DRV89xx(int cs_pin, int fault_pin, int sleep_pin) : _spi_settings(4000000, MSBFIRST, SPI_MODE1), _cs_pin(cs_pin), _fault_pin(fault_pin), _sleep_pin(sleep_pin) {}

void DRV89xx::begin()
{
    Serial.println("DRV8910 begin called");
    if (begin_called_)
        return;
    begin_called_ = true;
    Serial.println("and run");

    // Setup pins
    pinMode(_cs_pin, OUTPUT);
    digitalWrite(_cs_pin, HIGH);

    if (_sleep_pin)
    {
        pinMode(_sleep_pin, OUTPUT);
        digitalWrite(_sleep_pin, HIGH); // enable chip
    }
    if (_fault_pin)
        pinMode(_fault_pin, INPUT);

    // Configure device
    _config_cache[(int)DRV89xxRegister::OLD_CTRL_1] = 0b11111111; // Disable open load detection on channels 1-8
    _config_cache[(int)DRV89xxRegister::OLD_CTRL_2] = 0b00000011; // Disable errors from open load detection, open load detect on channels 9-12
    // _config_cache[(int)DRV89xxRegister::OLD_CTRL_3] = 0b00000000; // set Overcurrent protection to the most forgiving setting
    // _config_cache[(int)DRV89xxRegister::SR_CTRL_1] = 0b11111111; // Set slew rate to 2.5us vrs default 0.6us on half bridges (1-8)
    // _config_cache[(int)DRV89xxRegister::SR_CTRL_2] = 0b00001111; // Set slew rate to 2.5us vrs default 0.6us on half bridges (9-12)
    // _config_cache[(int)DRV89xxRegister::PWM_FREQ_CTRL] = 0xFF;   // Set all 4 PWM channels to 2kHz (max speed), TODO: Possibly make this a configurable option?
    _config_cache[(int)DRV89xxRegister::OP_CTRL_1] = 0b00000000;
    _config_cache[(int)DRV89xxRegister::OP_CTRL_2] = 0b00000000;
    _config_cache[(int)DRV89xxRegister::OP_CTRL_3] = 0b00000000;
    // TODO: Possibly make FW_CTRL_1+ an option?
    writeConfig();
}

byte DRV89xx::writeRegister(byte address, byte value)
{
    digitalWrite(_cs_pin, LOW);
    uint16_t ret = SPI.transfer16((address << 8) | value);
    digitalWrite(_cs_pin, HIGH);
    delayMicroseconds(1); // Give the chip a chance to write
    return ret;
}

byte DRV89xx::readRegister(byte address)
{
    digitalWrite(_cs_pin, LOW);
    uint16_t ret = SPI.transfer16(DRV89xx_REGISTER_READ | (address << 8));
    digitalWrite(_cs_pin, HIGH);
    return ret & 0xFF;
}

void DRV89xx::readErrorStatus(bool print, bool reset)
{
    SPI.beginTransaction(_spi_settings);
    if (digitalRead(_fault_pin) == 0)
    {
        if (print)
            Serial.println("Error detected");
    }
    else
    {
        if (print)
            Serial.println("No DRV errors seen");
    }
    if (print)
    {
        Serial.print("Status: ");
        Serial.println(readRegister(0x00), HEX);
        Serial.print("Overcurrent: ");
        Serial.print(readRegister(0x03), HEX);
        Serial.print(" ");
        Serial.print(readRegister(0x02), HEX);
        Serial.print(" ");
        Serial.println(readRegister(0x01), HEX);
        Serial.print("Open Load: ");
        Serial.print(readRegister(0x06), HEX);
        Serial.print(" ");
        Serial.print(readRegister(0x05), HEX);
        Serial.print(" ");
        Serial.println(readRegister(0x04), HEX);
    }

    SPI.endTransaction();
    if (digitalRead(_fault_pin) == 0 && reset)
    {                                                                  // reset
        _config_cache[(int)DRV89xxRegister::CONFIG_CTRL] = 0b00000001; // clear fault
        writeConfig();                                                 // try writing the config again, just in case
        _config_cache[(int)DRV89xxRegister::CONFIG_CTRL] = 0b00000000; // and go back to normal
    }
}

void DRV89xx::writeConfig()
{
    // Flush the 28 bytes of cache
    SPI.beginTransaction(_spi_settings);
    for (byte i = DRV89xx_CONFIG_WRITE_START; i < DRV89xx_CONFIG_BYTES; i++)
    {
        DRV89xx::writeRegister(i, _config_cache[i]);
    }
    SPI.endTransaction();
}

SPISettings DRV89xx::get_spi_settings()
{
    return _spi_settings;
}