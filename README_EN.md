This is an automatic translation, may be incorrect in some places. See sources and examples!

# Necdecoder
Light library for decoding IR NEC protocol (Chinese remote controls)
- The library does not take any interruptions
- Counting time on micros ()
- processing the repetition command (holding the remote control button)
- processing and reading is 500B flash

## compatibility
Compatible with all arduino platforms (used arduino functions)

## Content
- [installation] (# Install)
- [initialization] (#init)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** Necdecoder ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download library] (https://github.com/gyverlibs/necdecoder/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="init"> </a>
## initialization
`` `CPP
// Reading
NECDECODER IR;

// Sending
Necencoder ENC (PIN);
Necencoder ENC (PIN, DEL);
// pin - any digital pin
// del - the number of ISS to get 38 kHz.By the silence 10
// (from the calculation of 10mx delay + 3 μs execution of DigitalWrite Arduino)
`` `

<a id="usage"> </a>
## Usage
`` `CPP
// Necdecoder
VOID Tick ();// Call with a negative (Falling) front on the pin of the IR receiver in interruption
Bool Available ();// Returns True if the correct package is read or repeated (ISDECODED () + isrepeated ())
uint32_t Readpacket ();// read the entire package (address + ~ address + command + ~ team)
Uint8_T Readaddress ();// Read the address
uint8_t Readcommand ();// Read the command
Bool ISDECODED ();// returns True if the package is successfully decoded
Bool ISREPEATED ();// Returns True if the flag of the repetition of the team is adopted

// Necencoder
VOID SEND (UINT8_T Address, Uint8_t Data);// Send the address and data
`` `

<a id="EXAMPLE"> </a>
## Example
The rest of the examples look at ** Examples **!
** Reception **
`` `CPP
// Reception of commands from the remote control
// receiver connect to interruption by falling

#include <necdecoder.h>
NECDECODER IR;

VOID setup () {
  Serial.Begin (9600);
  // Full constructionsilt on d2, interruption 0
  Attachinterrupt (0, Irisr, Falling);
}

// In interruption we call tick ()
VOID Irisr () {
  Ir.tick ();
}

VOID loop () {
  // if the package is successfully accepted
  if (Ir.available ()) {
    // We display the entire package (32 bits)
    Serial.print ("0x");
    Serial.println (IR.Readpacket (), hex);

    // You can only display the command (8 bits)
    //Serial.println (IR.Readcommand (), Hex);
  }
}
`` `

** Sending **
`` `CPP
#include <necencoder.h>
Necencoder ENC (3);

VOID setup () {
}

VOID loop () {
  ENC.SEND (0x15, 0XAA);
  DELAY (1000);
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V1.1 - Fixed errors, added the ability to connect handlers, added stream control
- V2.0 - made up a lot of optimization, stable, reliability, simplification and convenience (by Alexgyver)
- V2.1 - added sending

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code