#include <HardwareSerial.h>

#define LORA_RX 20
#define LORA_TX 21

HardwareSerial LoRa(1);

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("=== RX Solar Panel ===");

  // UART LoRa
  LoRa.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);
}

void loop() {
  if (LoRa.available()) {
    String data = LoRa.readStringUntil('\n');
    data.trim();  // buang \r\n

    if (data.length() > 0) {
      Serial.print("RX Data: ");
      Serial.println(data);

      // === PARSING ===
      int vpIndex = data.indexOf("VP=");
      int tpIndex = data.indexOf(",TP=");

      if (vpIndex >= 0 && tpIndex >= 0) {
        float vPanel = data.substring(vpIndex + 3, tpIndex).toFloat();
        float tPanel = data.substring(tpIndex + 4).toFloat();

        Serial.print("Panel Voltage: ");
        Serial.print(vPanel);
        Serial.println(" V");

        Serial.print("Panel Temp   : ");
        Serial.print(tPanel);
        Serial.println(" C");
        Serial.println("----------------------");
      }
    }
  }
}
