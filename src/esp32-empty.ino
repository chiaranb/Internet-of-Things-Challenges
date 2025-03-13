#include <WiFi.h>
#include <esp_now.h>
#include <esp_sleep.h>

#define PIN_TRIG 13
#define PIN_ECHO 12
#define TIME_TO_SLEEP 40
#define uS_TO_S_FACTOR 1000000

uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x84, 0xFB, 0x90};
const int thDistance = 50;

esp_now_peer_info_t peerInfo;

void initUltrasonicSensor() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

// Sending callback
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Ok" : "Error");
}

// Receiving callback
void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  Serial.print("Message received: ");
  char receivedString[len];
  memcpy(receivedString, data, len);
  Serial.println(String(receivedString));
}

float getDistance(){
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  int duration = pulseIn(PIN_ECHO, HIGH);
  float distance = duration / 58.0;
  Serial.print("Distance in cm: ");
  Serial.println(distance);
  return distance;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // initialize ultrasonic sensor
  initUltrasonicSensor();

  // initialize ESP-NOW
  WiFi.mode(WIFI_STA);
  esp_now_init();

  // Peer Registration
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // get distance from sensor 
  float distance = getDistance();

  // distance msg
  String msg;
  if(distance <= thDistance) {
    msg = "OCCUPIED";
  } else {
    msg = "FREE";
  }

  esp_now_send(broadcastAddress, (uint8_t*)msg.c_str(), msg.length() +1);

// enable deep sleep 
  Serial.println("Going to sleep now");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("ESP32 sleep every " + String(TIME_TO_SLEEP));
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {
  // put your main code here, to run repeatedly:
}
