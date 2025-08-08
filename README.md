[![latest](https://img.shields.io/github/v/release/GyverLibs/NecDecoder.svg?color=brightgreen)](https://github.com/GyverLibs/NecDecoder/releases/latest/download/NecDecoder.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/NecDecoder.svg)](https://registry.platformio.org/libraries/gyverlibs/NecDecoder)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/NecDecoder?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

> [!NOTE]
> **Для передачи по ИК кастомных данных используйте библиотеку [GyverWire](https://github.com/GyverLibs/GyverWire)**

# NecDecoder
Лёгкая библиотека для декодирования ИК протокола NEC (99% китайских пультов)
- Библиотека не забирает прерывания
- Отсчет времени на micros()
- Обработка команды повтора (удержание кнопки пульта)

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Установка](#install)
- [Баги и обратная связь](#feedback)

<a id="usage"></a>

## Использование
### NecDecoder
Чтение данных с ИК приёмника:

```cpp
// Вызывать в FALLING прерывании ИК приемника
void tick();

// Есть данные для чтения (новые или повтор)
// anyRepeat - если false, то игнорируется повтор при сбое последнего чтения
bool available(bool anyRepeat = false);

// Возвращает true, если принят флаг повтора команды
bool isRepeated();

// таймаут после последнего сигнала с пульта. Сработает однократно
bool timeout(uint16_t ms);

// Прочитать данные (адрес + команда)
uint16_t readData();

// Прочитать адрес
uint8_t readAddress();

// Прочитать команду
uint8_t readCommand();

// Прочитать весь пакет NEC
uint32_t readPacket();
```

Настройки компиляции (перед подключением библиотеки)

```cpp
#define NEC_SKIP_REPEATS 3  // пропуск N первых пакетов с "повтором"
```

### NecEncoder
Отправка данных с ИК светодиода:

```cpp
NecEncoder::send(uint8_t pin, uint8_t addr, uint8_t cmd);
```

<a id="example"></a>

## Пример
Остальные примеры смотри в **examples**!  

### Приём
```cpp
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
```

### Отправка
```cpp
#include <NecEncoder.h>
#define LED_PIN 3

void setup() {
}

void loop() {
    NecEncoder::send(LED_PIN, 0x15, 0xaa);
    delay(1000);
}
```

<a id="versions"></a>

## Версии
- v1.0
- v1.1 - исправлены ошибки, добавлена возможность подключения обработчиков, добавлен контроль потока
- v2.0 - завёз очень много оптимизации, стабильноси, надёжности, упрощения и удобства (by AlexGyver)
- v2.1 - добавил отправку (NecEncoder)
- v3.0 - библиотека переписана. Уменьшен вес, улучшена стабильность

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **NecDecoder** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/NecDecoder/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код