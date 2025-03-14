#include <WiFi.h>
#include <esp_now.h>
#include <esp_sleep.h>

// Define the pins for the ultrasonic sensor
#define PIN_TRIG 13
#define PIN_ECHO 12
#define TIME_TO_SLEEP 40 // Time of deep sleep in seconds
#define uS_TO_S_FACTOR 1000000 

uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x84, 0xFB, 0x90}; // MAC address of the receiver
const int thDistance = 50; // Threshold distance in cm

esp_now_peer_info_t peerInfo;

// Function to initialize the ultrasonic sensor
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

// Function to get the distance using the ultrasonic sensor
float getDistance() {
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
  unsigned long startMillis, endMillis; // Start and end times

  Serial.begin(115200);

  // Initialize ultrasonic sensor
  Serial.println("Initializing ultrasonic sensor...");
  startMillis = millis();
  initUltrasonicSensor();
  endMillis = millis();
  Serial.println("Ultrasonic sensor initialized. Time elapsed: " + String(endMillis - startMillis) + " ms");

  // Initialize ESP-NOW 
  Serial.println("Initializing ESP-NOW...");
  unsigned long startWiFiMillis = millis();
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_2dBm);
  esp_now_init();

  // Register peer
  Serial.println("Registering ESP-NOW peer...");
  startMillis = millis();
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
  endMillis = millis();
  Serial.println("Peer registered. Time elapsed: " + String(endMillis - startMillis) + " ms");

  // Register callbacks
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Get distance from the sensor
  Serial.println("Getting distance from sensor...");
  startMillis = millis();
  float distance = getDistance();
  endMillis = millis();
  Serial.println("Distance measured. Time elapsed: " + String(endMillis - startMillis) + " ms");

  // Prepare the message
  String msg;
  if (distance <= thDistance) { 
    msg = "OCCUPIED";
  } else {
    msg = "FREE";
  }

  // Send the message
  Serial.println("Sending ESP-NOW message: " + msg);
  startMillis = millis();
  esp_now_send(broadcastAddress, (uint8_t*)msg.c_str(), msg.length() + 1);
  endMillis = millis();
  Serial.println("Message sent. Time elapsed: " + String(endMillis - startMillis) + " ms");

  // Enable deep sleep
  Serial.println("Preparing for deep sleep...");
  unsigned long endWiFiMillis = millis();
  Serial.println("WiFi time on: " + String(endWiFiMillis - startWiFiMillis) + " ms");
  WiFi.mode(WIFI_OFF);
  delay(1000);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Going to sleep now.");
  Serial.flush();
  esp_deep_sleep_start();
}

void loop() {

};
