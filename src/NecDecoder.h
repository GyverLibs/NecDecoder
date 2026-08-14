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
    enum ParseState : uint8_t {
        NEC_start = 0,
        NEC_end = 32,
        NEC_repeat = NEC_end + NEC_SKIP_REPEATS,
        NEC_idle,
    };
    enum class ReadyState : uint8_t {
        Idle,
        Read,
        Available,
        Repeat,
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
                _parse = ParseState::NEC_start;
                _buf = 0;
                return;

            // WAIT REPEAT
            case (_NEC_WAIT_REP1 - _NEC_FRAME_TOL)...(_NEC_WAIT_REP1 + _NEC_FRAME_TOL):
            case (_NEC_WAIT_REP2 - _NEC_FRAME_TOL)...(_NEC_WAIT_REP2 + _NEC_FRAME_TOL):
                if (_parse >= ParseState::NEC_end) return;
                break;

            // REPEAT
            case (_NEC_START_REPEAT - _NEC_FRAME_TOL)...(_NEC_START_REPEAT + _NEC_START_DATA) / 2:
                _ready = ReadyState::Repeat;
                _repeated = true;

                if (_parse >= ParseState::NEC_end && _parse < ParseState::NEC_repeat) {
                    ++_parse;
                    return;
                }
                if (_parse == ParseState::NEC_repeat) {
                    _ready = ReadyState::Available;
                    return;
                }
                break;

            // DATA
            case (_NEC_HIGH - _NEC_BIT_TOL)...(_NEC_HIGH + _NEC_BIT_TOL):
                bit = 1;
            case (_NEC_LOW - _NEC_BIT_TOL)...(_NEC_LOW + _NEC_BIT_TOL):
                if (_parse < ParseState::NEC_end) {
                    if (bit) _buf |= 1ul << _parse;
                    if (++_parse == ParseState::NEC_end) {
                        uint8_t address = _buf;
                        uint8_t addressInv = _buf >> 8;
                        uint8_t command = _buf >> 16;
                        uint8_t commandInv = _buf >> 24;
                        if (uint8_t(address ^ addressInv) == 0xff &&
                            uint8_t(command ^ commandInv) == 0xff) {
                            _data = (uint16_t(address) << 8) | command;
                            _repeated = false;
                            _ready = ReadyState::Available;
                        } else {
                            _parse = ParseState::NEC_idle;
                        }
                    }
                    return;
                }
                break;
        }
        _parse = ParseState::NEC_idle;
    }

    // Есть данные для чтения (новые или повтор)
    // anyRepeat - если false, то игнорируется повтор при сбое последнего чтения
    bool available(bool anyRepeat = false) {
        switch (_ready) {
            case ReadyState::Available:
                _ready = ReadyState::Read;
                return true;

            case ReadyState::Repeat:
                _ready = anyRepeat ? ReadyState::Read : ReadyState::Idle;
                return anyRepeat;

            default: break;
        }
        return false;
    }

    // Возвращает true, если принят флаг повтора команды
    bool isRepeated() {
        return _repeated;
    }

    // Прочитать данные (адрес + команда)
    uint16_t readData() {
        return _readData();
    }

    // Прочитать адрес
    uint8_t readAddress() {
        return _readData() >> 8;
    }

    // Прочитать команду
    uint8_t readCommand() {
        return _readData();
    }

    // Прочитать весь пакет NEC
    uint32_t readPacket() {
        uint16_t data = _readData();
        return (uint32_t(data & 0xff00) << 16) | (uint32_t(~data & 0xff00) << 8) | ((data & 0xff) << 8) | (~data & 0xff);
    }

    // таймаут после последнего сигнала с пульта. Сработает однократно
    bool timeout(uint16_t ms) {
        if (_ready == ReadyState::Read) {
            noInterrupts();
            uint32_t t = _tmr;
            interrupts();
            if (micros() - t >= ms * 1000ul) {
                _ready = ReadyState::Idle;
                return true;
            }
        }
        return false;
    }

   private:
    volatile uint32_t _tmr = 0;
    uint32_t _buf = 0;
    volatile uint16_t _data = 0;
    volatile uint8_t _parse = ParseState::NEC_idle;
    volatile ReadyState _ready = ReadyState::Idle;
    volatile bool _repeated = false;

    uint16_t _readData() {
#ifdef __AVR__
        uint8_t sreg = SREG;
        cli();
        uint16_t data = _data;
        SREG = sreg;
        return data;
#else
        return _data;
#endif
    }
};
