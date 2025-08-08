#pragma once
#include <Arduino.h>

#include "NEC_timings.h"

#define _NEC_BIT_TOL 150ul     // допуск бита
#define _NEC_FRAME_TOL 1500ul  // допуск фрейма

#ifndef NEC_SKIP_REPEATS
#define NEC_SKIP_REPEATS 3  // пропуск повторов
#endif

#define _NEC_START_DATA (_NEC_START + _NEC_DATA)
#define _NEC_START_REPEAT (_NEC_START + _NEC_REPEAT)
#define _NEC_WAIT_REP1 (_NEC_FRAME - _NEC_START_DATA - (_NEC_HIGH + _NEC_LOW) * 16)
#define _NEC_WAIT_REP2 (_NEC_FRAME - _NEC_START_REPEAT)

// class
class NecDecoder {
    enum State : uint8_t {
        NEC_start = 0,
        NEC_end = 32,
        NEC_repeat = 33 + NEC_SKIP_REPEATS,
        NEC_idle,
    };

   public:
    // Вызывать в FALLING прерывании ИК приемника
    void tick() {
        uint32_t pulse = micros() - _tmr;  // время импульса
        _tmr += pulse;                     // == (_tmr = micros() )
        bool bit = 0;

        switch (pulse) {
            // START
            case ((_NEC_START_REPEAT + _NEC_START_DATA) / 2 + 1)...(_NEC_START_DATA + _NEC_FRAME_TOL):
                _state = NEC_start;
                return;

            // WAIT REPEAT
            case (_NEC_WAIT_REP1 - _NEC_FRAME_TOL)...(_NEC_WAIT_REP1 + _NEC_FRAME_TOL):
            case (_NEC_WAIT_REP2 - _NEC_FRAME_TOL)...(_NEC_WAIT_REP2 + _NEC_FRAME_TOL):
                if (_state >= NEC_end) return;
                break;

            // REPEAT
            case (_NEC_START_REPEAT - _NEC_FRAME_TOL)...(_NEC_START_REPEAT + _NEC_START_DATA) / 2:
                switch (_state) {
                    case NEC_end ... NEC_repeat - 1:
                        ++_state;
                        return;
                    case NEC_repeat:
                        _ready = true;
                        return;
                }
                break;

            // DATA
            case (_NEC_HIGH - _NEC_BIT_TOL)...(_NEC_HIGH + _NEC_BIT_TOL):
                bit = 1;
            case (_NEC_LOW - _NEC_BIT_TOL)...(_NEC_LOW + _NEC_BIT_TOL):
                if (_state < NEC_end) {
                    _buf = (_buf << 1) | bit;
                    if (++_state == NEC_end) {
                        if (!(((_buf >> 8) & _buf) & 0xFF00FF)) {
                            _data = ((_buf >> 16) & 0xff00) | ((_buf >> 8) & 0xff);
                            _ready = true;
                        } else {
                            _state = NEC_idle;
                        }
                    }
                    return;
                }
                break;
        }
        _state = NEC_idle;
    }

    // Есть данные для чтения (новые или повтор)
    bool available() {
        return _ready ? _ready = false, true : false;
    }

    // Возвращает true, если принят флаг повтора команды
    bool isRepeated() {
        return _state == NEC_repeat;
    }

    // Прочитать данные (адрес + команда)
    uint16_t readData() {
        return _data;
    }

    // Прочитать адрес
    uint8_t readAddress() {
        return _data >> 8;
    }

    // Прочитать команду
    uint8_t readCommand() {
        return _data;
    }

    // Прочитать весь пакет NEC
    uint32_t readPacket() {
        return (uint32_t(_data & 0xff00) << 16) | (uint32_t(~_data & 0xff00) << 8) | ((_data & 0xff) << 8) | (~_data & 0xff);
    }

    // таймаут после последнего сигнала с пульта. Сработает однократно
    bool timeout(uint16_t ms) {
        switch (_state) {
            case NEC_end ... NEC_repeat:
                noInterrupts();
                uint32_t t = _tmr;
                interrupts();
                if (micros() - t >= ms * 1000ul) {
                    _state = NEC_idle;
                    return true;
                }
                break;
        }
        return false;
    }

   private:
    uint32_t _tmr = 0;
    uint32_t _buf = 0;
    uint16_t _data = 0;
    uint8_t _state = NEC_idle;
    bool _ready = false;
};