#include <LoRa.h>
#include <stdint.h>

#define CS_PIN 18
#define RST_PIN 23
#define IRQ_PIN 26

#define PACKET_SIZE 255

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

/*=== Transmit Power Levels ===
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


uint8_t data_packet[PACKET_SIZE];

void setup() {
  Serial.begin(115200);
  while(!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);

  if(!LoRa.begin(FREQ))
  {
    Serial.println("LoRa init failed. Check your connections");
    while(true);
  }

  LoRa.disableCrc();
  LoRa.setTxPower(TX_POWER);
  LoRa.setSignalBandwidth(BANDWIDTH);
}

void loop() {
    clear_buffer();
    while(Serial.available() == 0);
    char input = (char) Serial.read(); // Clear
    if(input == 'e')
      while(true);
    else if(input == 'r')
    {
      configure_radio();
    }
    else
    {
      generate_random_packet();
      LoRa.beginPacket(false); // By adding true we are implying that we want the implicit header style
      LoRa.write(data_packet, PACKET_SIZE);
      LoRa.endPacket(false);
    }
}

void generate_random_packet()
{
  Serial.print(String(PACKET_SIZE) + " ");

  for(int i = 0; i < PACKET_SIZE; i++)
  {
    data_packet[i] = (uint8_t) random(0, 15);
    Serial.print(data_packet[i], HEX);
  }

  Serial.println("");
}

void configure_radio()
{
    // Put the radio into standby to update
    LoRa.idle();

    // Set the Coding Rate
    Serial.println("Enter number of redundancy bits: 5, 6, 7, 8");
    clear_buffer();
    while(Serial.available() == 0);
    int coding_rate = Serial.parseInt();
    Serial.print("Setting coding rate to ");
    Serial.println(coding_rate);
    LoRa.setCodingRate4(coding_rate);
    
    Serial.println("Enter spreading factor: 6, 7, 8, 9, 10, 11, 12");
    clear_buffer();
    while(Serial.available() == 0);
    int spreading_factor = Serial.parseInt();
    LoRa.setSpreadingFactor(spreading_factor);
    Serial.print("Set spreading factor to ");
    Serial.println(LoRa.getSpreadingFactor(), HEX);
    Serial.print("Current bandwidth: ");
    Serial.print(LoRa.getSignalBandwidth(), HEX);
}

void clear_buffer()
{
    while(Serial.available() > 0)
        Serial.read();
}
  