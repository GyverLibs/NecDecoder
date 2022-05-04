# NecDecoder
Lightweight library for NEC IR protocol decoding (Chinese remotes)
- The library does not pick up any interrupts
- Countdown on micros()
- Processing the repeat command (holding the remote control button)
- Processing and reading takes 500b Flash

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found by the name **NecDecoder** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download library](https://github.com/GyverLibs/NecDecoder/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
// read
NecDecoder ir;

// send
NecEncoder enc(pin);
NecEncoder enc(pin, del);
// pin - any digital pin
// del is the number of microseconds to get 38 kHz. Default 10
// (based on 10µs latency + 3µs digitalWrite Arduino execution)
```

<a id="usage"></a>
## Usage
```cpp
// NecDecoder
void tick(); // Call on FALLING edge on IR receiver pin in interrupt
bool available(); // Returns true if a valid packet has been read or repeated (isDecoded() + isRepeated())
uint32_t readPacket(); // Read entire packet (address + ~address + command + ~command)
uint8_t readAddress(); // Read address
uint8_t readCommand(); // Read command
bool isDecoded(); // Returns true if the packet was successfully decoded
bool isRepeated(); // Returns true if command repeat flag is passed

// NecEncoder
void send(uint8_t address, uint8_t data); // send address and data
```

<a id="example"></a>
## Example
See **examples** for other examples!
**Reception**
```cpp
// receiving commands from the console
// connect the receiver to interrupt on FALLING

#include <NecDecoder.h>
NecDecoder ir;

void setup() {
  Serial.begin(9600);
  // connected to D2, interrupt 0
  attachInterrupt(0, irIsr, FALLING);
}

// call tick() in interrupt
void irIsr() {
  ir.tick();
}

void loop() {
  // if the packet was successfully received
  if (ir.available()) {
    // output the whole packet (32 bits)
    Serial.print("0x");
    Serial.println(ir.readPacket(), HEX);

    // only command can be output (8 bits)
    //Serial.println(ir.readCommand(), HEX);
  }
}
```

**Shipping**
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
## Versions
- v1.0
- v1.1 - fixed bugs, added the ability to connect handlers, added controlotok
- v2.0 - brought a lot of optimization, stability, reliability, simplification and convenience (by AlexGyver)
- v2.1 - added sending (NecEncoder)

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!