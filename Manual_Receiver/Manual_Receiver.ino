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

/*=== Automatic Gain Control LNA Gain Setting ===
    - 1 | 0 dB (Relative LNA Gain) | Maximum gain
    - 2 | -6 dB
    - 3 | -12 dB
    - 4 | -24 dB
    - 5 | -26 dB
    - 6 | -48 dB | Minimum gain - AGC has full range of control to set LNA
*/
#define GAIN_LEVEL 6

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);

  LoRa.setPins(CS_PIN, RST_PIN, IRQ_PIN);
  if(!LoRa.begin(915E6))
  {
    Serial.println("LoRa init failed. Check your connections");
    while(true);
  }

  LoRa.disableCrc();
  LoRa.setOCP(240);
  
  // Setting the aforementioned configurable parameters
  LoRa.setSpreadingFactor(SPREADING_FACTOR);
  LoRa.setSignalBandwidth(BANDWIDTH);
  LoRa.setCodingRate4(CODING_RATE);
  LoRa.setGain(GAIN_LEVEL);

  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() 
{

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
    Serial.print((char) LoRa.read());
  }

  Serial.println("");
}
