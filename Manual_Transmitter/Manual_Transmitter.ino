#include <LoRa.h>
#include <stdint.h>

// Device pin settings for the T-Beam (do not edit)
#define CS_PIN 18
#define RST_PIN 23
#define IRQ_PIN 26

// Macros for static device configuration
/*=== Possible Bandwidths ===
    - 7.8E3 | 7.8 kHz
    - 10.4E3 | 10.4 kHz
    - 15.6E3 | 15.6 kHz
    - 20.8E3 | 20.8 kHz
    - 31.25E3 | 31.25 kHz
    - 41.7E3 | 41.7 kHz
    - 62.5E3 | 62.5 kHz
    - 125E3 | 125 kHz
    - 250E3 | 250 kHz
    - 500E3 | 500 kHz
*/
#define BANDWIDTH 500E3

/*=== Possible Spreading Factors ===
    - 6 (only when using the implicit header so ignore in this case)
    - 7
    - 8
    - 9
    - 10
    - 11
    - 12
*/
#define SPREADING_FACTOR 7

/*=== Coding Rate ===
    - 5 | (4:5)
    - 6 | (4:6)
    - 7 | (4:7)
    - 8 | (4:8)
*/
#define CODING_RATE 5

/*=== Higher Power Transmit PA Levels ===
    - 14
    - 15
    - 16
    - 17
    - 18
    - 19
    - 20 
*/
#define TX_POWER 14

/*=== Frequency ===
    - 902 - 928 MHz
*/
#define FREQ 915E6

uint8_t message[256];
int message_len = 0;

void setup() {
  Serial.begin(115200); // Make sure this baud rate matches your serial console
  while(!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);

  if(!LoRa.begin(FREQ))
  {
    Serial.println("LoRa init failed. Check your connections");
    while(true);
  }

  LoRa.idle();

  LoRa.disableCrc();
  
  // Setting the aforementioned configurable parameters
  LoRa.setTxPower(TX_POWER, 1);
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
}

void loop() 
{
    //Serial.println("Press enter to send a message");
    clear_buffer();
    message_len = 0;

    while(message_len != 255)
    {
        if(Serial.available() > 0)
        {
            message[message_len++] = (uint8_t) Serial.read();
            if(message[message_len - 1] == '\n' 
                or message[message_len - 1] == '\r')
            {
                message_len -= 1;
                break;
            }
        }
    }

    // Send the message
    LoRa.beginPacket(false);
    LoRa.write(message, message_len);
    LoRa.endPacket(false);
}

void clear_buffer()
{
    while(Serial.available() > 0)
        Serial.read();
}
  