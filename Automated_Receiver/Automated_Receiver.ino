#include <LoRa.h>
#include <stdint.h>

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

/*=== Automatic Gain Control LNA Gain Setting ===
    - 1 | 0 dB (Relative LNA Gain) | Maximum gain
    - 2 | -6 dB
    - 3 | -12 dB
    - 4 | -24 dB
    - 5 | -26 dB
    - 6 | -48 dB | Minimum gain - AGC has full range of control to set LNA
*/
#define GAIN_LEVEL 6

/*=== Frequency ===
    - 902 - 928 MHz
*/
#define FREQ 915E6

/*=== Over Current Protection ===*/
#define OCP_MA 240;

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
  LoRa.setOCP(OCP_MA);
  LoRa.setGain(GAIN_LEVEL);
  LoRa.setSignalBandwidth(BANDWIDTH);
}

void loop()
{
  while(Serial.available() == 0);
    char input = (char) Serial.read(); // Clear
    if(input == 'r')
    {
      clear_buffer();
      configure_radio();
    }
}

void onReceive(int packetSize)
{
  int read_len = LoRa.available();
  
  if(read_len == 0)
    return;
  
  Serial.print(LoRa.packetRssi());
  Serial.print(" ");
  Serial.print(LoRa.packetSnr());
  Serial.print(" ");
  Serial.print(read_len);
  Serial.print(" ");

  for(int i = 0; i < read_len; i++)
  {
    Serial.print(LoRa.read(), HEX);
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

    LoRa.onReceive(onReceive);
    LoRa.receive();
}

void clear_buffer()
{
    while(Serial.available() > 0)
        Serial.read();
}
