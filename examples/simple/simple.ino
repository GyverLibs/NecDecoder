// приём команд с пульта
// приёмник подключать на прерывание по FALLING

#include <NecDecoder.h>
NecDecoder ir;

// в прерывании вызываем tick()
void irIsr() {
    ir.tick();
}

void setup() {
    Serial.begin(115200);
    // подключил на D2, прерывание 0
    attachInterrupt(0, irIsr, FALLING);
}

void loop() {
    // если пакет успешно принят
    if (ir.available()) {
        Serial.print("0x");

        // адрес + команда (16 бит)
        Serial.println(ir.readData(), HEX);

        // команда (8 бит)
        // Serial.println(ir.readCommand(), HEX);

        // весь пакет (32 бита)
        // Serial.println(ir.readPacket(), HEX);
    }
}