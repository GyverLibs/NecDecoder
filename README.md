[![latest](https://img.shields.io/github/v/release/GyverLibs/NecDecoder.svg?color=brightgreen)](https://github.com/GyverLibs/NecDecoder/releases/latest/download/NecDecoder.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/NecDecoder.svg)](https://registry.platformio.org/libraries/gyverlibs/NecDecoder)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/NecDecoder?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

> [!NOTE]
> **Для передачи по ИК кастомных данных используйте библиотеку [GyverWire](https://github.com/GyverLibs/GyverWire)**

# NecDecoder
Лёгкая библиотека для декодирования ИК протокола NEC (китайские пульты)
- Библиотека не забирает никакие прерывания
- Отсчет времени на micros()
- Обработка команды повтора (удержание кнопки пульта)
- Обработка и чтение занимает 500б Flash

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

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


<a id="init"></a>
## Инициализация
```cpp
// чтение
NecDecoder ir;

// отправка
NecEncoder enc(pin);
NecEncoder enc(pin, del);
// pin - любой цифровой пин
// del - количество мкс, чтобы получить 38 кгц. По умолч 10
// (из расчёта 10мкс задержка + 3 мкс выполнение digitalWrite Arduino)
```

<a id="usage"></a>
## Использование
```cpp
// NecDecoder
void tick();                        // Вызывать при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника в прерывании
bool available();                   // Возвращает true, если корректный пакет прочитан или повторён (isDecoded() + isRepeated())
uint32_t readPacket();              // Прочитать пакет целиком (адрес + ~адрес + команда + ~команда)
uint8_t readAddress();              // Прочитать адрес
uint8_t readCommand();              // Прочитать команду
bool isDecoded();                   // Возвращает true, если пакет успешно декодирован
bool isRepeated();                  // Возвращает true, если принят флаг повтора команды

// NecEncoder
void send(uint8_t address, uint8_t data);   // отправить адрес и данные
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!  
**Приём**
```cpp
// приём команд с пульта
// приёмник подключать на прерывание по FALLING

#include <NecDecoder.h>
NecDecoder ir;

void setup() {
  Serial.begin(9600);
  // подключил на D2, прерывание 0
  attachInterrupt(0, irIsr, FALLING);
}

// в прерывании вызываем tick()
void irIsr() {
  ir.tick();
}

void loop() {
  // если пакет успешно принят
  if (ir.available()) {
    // выводим весь пакет (32 бита)
    Serial.print("0x");
    Serial.println(ir.readPacket(), HEX);

    // можно вывести только команду (8 бит)
    //Serial.println(ir.readCommand(), HEX);
  }
}
```

**Отправка**
```cpp
#include <NecEncoder.h>
NecEncoder enc(3);

void setup() {
}

void loop() {
  enc.send(0x15, 0xaa);
  delay(1000);
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - исправлены ошибки, добавлена возможность подключения обработчиков, добавлен контроль потока
- v2.0 - завёз очень много оптимизации, стабильноси, надёжности, упрощения и удобства (by AlexGyver)
- v2.1 - добавил отправку (NecEncoder)

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
