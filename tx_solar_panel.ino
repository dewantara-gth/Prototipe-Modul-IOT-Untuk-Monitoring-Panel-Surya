#include <HardwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ================== PIN ==================
#define LORA_RX 20
#define LORA_TX 21

#define ONE_WIRE_BUS 4      // DS18B20
#define PANEL_ADC    0      // ADC panel

// ================== OBJECT ==================
HardwareSerial LoRa(1);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("=== TX Solar Panel ===");

  // LoRa UART
  LoRa.begin(9600, SERIAL_8N1, LORA_RX, LORA_TX);

  // Sensor suhu
  sensors.begin();

  // ADC
  analogReadResolution(12); // 0-4095
}

// ================== LOOP ==================
void loop() {
  // === BACA TEGANGAN PANEL ===
  int adcRaw = analogRead(PANEL_ADC);
  float vAdc = adcRaw * (3.3 / 4095.0);
  float vPanel = vAdc * 6.0; // rasio divider (100k:20k)

  // === BACA SUHU ===
  sensors.requestTemperatures();
  float tPanel = sensors.getTempCByIndex(0);

  // === FORMAT DATA ===
  String payload = "VP=" + String(vPanel, 2) +
                   ",TP=" + String(tPanel, 1);

  // === KIRIM LORA ===
  LoRa.println(payload);

  // === LOG SERIAL ===
  Serial.println(payload);

  delay(2000);
}
