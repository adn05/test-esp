#include <stdio.h>
#include "Arduino.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRrecv.h"
#include "IRutils.h"
#include <assert.h>
#include "IRac.h"
#include "IRtext.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

//static const char *TAG = "IR_Dump";

// ==================== start of TUNEABLE PARAMETERS ====================
// An IR detector/demodulator is connected to GPIO pin 7
const uint16_t kRecvPin = 7;

// The Serial connection baud rate.
// i.e. Status message will be sent to the PC at this baud rate.
// Try to avoid slow speeds like 9600, as you will miss messages and
// cause other problems. 115200 (or faster) is recommended.
// NOTE: Make sure you set your Serial Monitor to the same speed.
const uint32_t kBaudRate = 115200;

// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
const uint16_t kCaptureBufferSize = 1024;

// kTimeout is the Nr. of milli-Seconds of no-more-data before we consider a
// message ended.
// This parameter is an interesting trade-off. The longer the timeout, the more
// complex a message it can capture. e.g. Some device protocols will send
// multiple message packets in quick succession, like Air Conditioner remotes.
// Air Coniditioner protocols often have a considerable gap (20-40+ms) between
// packets.
// The downside of a large timeout value is a lot of less complex protocols
// send multiple messages when the remote's button is held down. The gap between
// them is often also around 20+ms. This can result in the raw data be 2-3+
// times larger than needed as it has captured 2-3+ messages in a single
// capture. Setting a low timeout value can resolve this.
// So, choosing the best kTimeout value for your use particular case is
// quite nuanced. Good luck and happy hunting.
// NOTE: Don't exceed kMaxTimeoutMs. Typically 130ms.
const uint8_t kTimeout = 15;

// Set the smallest sized "UNKNOWN" message packets we actually care about.
// This value helps reduce the false-positive detection rate of IR background
// noise as real messages. The chances of background IR noise getting detected
// as a message increases with the length of the kTimeout value. (See above)
// The downside of setting this message too large is you can miss some valid
// short messages for protocols that this library doesn't yet decode.
//
// Set higher if you get lots of random short UNKNOWN messages when nothing
// should be sending a message.
// Set lower if you are sure your setup is working, but it doesn't see messages
// from your device. (e.g. Other IR remotes work.)
// NOTE: Set this value very high to effectively turn off UNKNOWN detection.
const uint16_t kMinUnknownSize = 12;

// How much percentage lee way do we give to incoming signals in order to match
// it?
// e.g. +/- 25% (default) to an expected value of 500 would mean matching a
//      value between 375 & 625 inclusive.
// Note: Default is 25(%). Going to a value >= 50(%) will cause some protocols
//       to no longer match correctly. In normal situations you probably do not
//       need to adjust this value. Typically that's when the library detects
//       your remote's message some of the time, but not all of the time.
const uint8_t kTolerancePercentage = kTolerance; // kTolerance is normally 25%

// ==================== end of TUNEABLE PARAMETERS ====================

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, true);
decode_results results; // Somewhere to store the results

extern "C" void app_main() 
{
    initArduino();

    Serial.begin(kBaudRate, SERIAL_8N1);
    while (!Serial) // Wait for the serial connection to be establised.
        delay(50);
    // Perform a low level sanity checks that the compiler performs bit field
    // packing as we expect and Endianness is as we expect.
    assert(irutils::lowLevelSanityCheck() == 0);

    Serial.printf("\n" D_STR_IRRECVDUMP_STARTUP "\n", kRecvPin);
#if DECODE_HASH
    // Ignore messages with less than minimum on or off pulses.
    irrecv.setUnknownThreshold(kMinUnknownSize);
#endif                                         // DECODE_HASH
    irrecv.setTolerance(kTolerancePercentage); // Override the default tolerance.
    irrecv.enableIRIn();                       // Start the receiver

    while (1)
    {
        // Check if the IR code has been received.
        if (irrecv.decode(&results))
        {
            // Display a crude timestamp.
            uint32_t now = millis();
            Serial.printf(D_STR_TIMESTAMP " : %06u.%03u\n", now / 1000, now % 1000);
            // Check if we got an IR message that was to big for our capture buffer.
            if (results.overflow)
                Serial.printf(D_WARN_BUFFERFULL "\n", kCaptureBufferSize);
            // Display the library version the message was captured with.
            Serial.println(D_STR_LIBRARY "   : v" _IRREMOTEESP8266_VERSION_STR "\n");
            // Display the tolerance percentage if it has been change from the default.
            if (kTolerancePercentage != kTolerance)
                Serial.printf(D_STR_TOLERANCE " : %d%%\n", kTolerancePercentage);
            // Display the basic output of what we found.
            Serial.print(resultToHumanReadableBasic(&results));
            // Display any extra A/C info if we have it.
            String description = IRAcUtils::resultAcToString(&results);
            if (description.length())
                Serial.println(D_STR_MESGDESC ": " + description);
            yield(); // Feed the WDT as the text output can take a while to print.

            // Output the results as source code
            Serial.println(resultToSourceCode(&results));
            Serial.println(); // Blank line between entries
            yield();          // Feed the WDT (again)
        }
    }
}