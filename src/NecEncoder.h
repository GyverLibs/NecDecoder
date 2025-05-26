#pragma once
#include <Arduino.h>
#include <GyverIO.h>

#include "NEC_timings.h"

// отправка
class NecEncoder {
   public:
    static void send(uint8_t pin, uint8_t addr, uint8_t cmd) {
        gio::init(pin, OUTPUT);
        _pulse38(pin, _NEC_START);
        delayMicroseconds(_NEC_DATA);
        _sendByte(pin, addr);
        _sendByte(pin, ~addr);
        _sendByte(pin, cmd);
        _sendByte(pin, ~cmd);
        _pulse38(pin, _NEC_BIT);
    }

    // legacy
    NecEncoder(uint8_t pin, uint8_t del = 10) : _pin(pin) {}

    void send(uint8_t addr, uint8_t cmd) {
        send(_pin, addr, cmd);
    }

   private:
    static void _pulse38(uint8_t pin, int dur) {
        bool f = 0;
        while (dur > 0) {
            gio::write(pin, f ^= 1);
            delayMicroseconds(13);
            dur -= 13;
        }
        if (f) gio::write(pin, 0);
    }

    static void _sendByte(uint8_t pin, uint8_t data) {
        int i = 8;
        while (i--) {
            _pulse38(pin, _NEC_BIT);
            delayMicroseconds((data & (1 << 7)) ? (_NEC_HIGH - _NEC_BIT) : _NEC_BIT);
            data <<= 1;
        }
    }

    const uint8_t _pin;
};