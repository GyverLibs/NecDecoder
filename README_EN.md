This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![latest](https://img.shields.io/github/v/release/GyverLibs/NecDecoder.svg?color=brightgreen)](https://github.com/GyverLibs/NecDecoder/releases/latest/download/NecDecoder.zip)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/NecDecoder.svg)](https://registry.platformio.org/libraries/gyverlibs/NecDecoder)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/NecDecoder?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

> [!NOTE]
> ** Use the library for IR data transfer[GyverWire](https://github.com/GyverLibs/GyverWire)**

# NecDecoder
Lightweight library for decoding NEC IR protocol (99% of Chinese remotes)
- Library doesn't take interruptions
- Countdown to micros()
- Repeat command processing (holding the remote button)

### Compatibility
Compatible with all Arduino platforms (Arduino features are used)

## Contents
- [Use of use](#usage)
- [Example](#example)
- [Versions](#versions)
- [Installation](#install)
- [Bugs and feedback](#feedback)

<a id="usage"></a>

## Use of use
### NecDecoder
Reading data from the IR receiver:

```cpp
// Call in FALLING to interrupt the IR receiver
void tick();

// Data to read (new or repeated)
// anyRepeat - if false, then the repeat is ignored when the last reading fails
bool available(bool anyRepeat = false);

// Returns true if the command repeat flag is accepted
bool isRepeated();

// timeout after the last signal from the remote. It works once.
bool timeout(uint16_t ms);

// Read the data (address + command)
uint16_t readData();

// Read the address.
uint8_t readAddress();

// Read the command
uint8_t readCommand();

// Read the entire NEC package
uint32_t readPacket();
```

Compilation settings (before connecting the library)

```cpp
#define NEC_SKIP_REPEATS 3  // pass N of the first packets with "repeat"
```

### NecEncoder
Sending data from IR LED:

```cpp
NecEncoder::send(uint8_t pin, uint8_t addr, uint8_t cmd);
```

<a id="example"></a>

## Example
For more examples see **examples**!

### Reception.
```cpp
#include <NecDecoder.h>
NecDecoder ir;

// in interruption, call tick()
void irIsr() {
    ir.tick();
}

void setup() {
    Serial.begin(115200);
    // connect to D2, interrupt 0
    attachInterrupt(0, irIsr, FALLING);
}

void loop() {
    // If the package is successfully accepted
    if (ir.available()) {
        Serial.print("0x");

        // Address + command (16 bits)
        Serial.println(ir.readData(), HEX);

        // command (8 bits)
        // Serial.println(ir.readCommand(), HEX);

        // entire package (32 bits)
        // Serial.println(ir.readPacket(), HEX);
    }
}
```

### Sending.
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

## Versions
- v1.0
- v1.1 - Errors fixed, handler connectivity added, flow control added
- v2.0 - brought a lot of optimization, stability, reliability, simplification and convenience (by AlexGyver)
- v2.1 - added sending (NecEncoder)
- v3.0 - Library rewritten. Reduced weight, improved stability

<a id="install"></a>
## Installation
- The library can be found under the name **NecDecoder** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download the library](https://github.com/GyverLibs/NecDecoder/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
