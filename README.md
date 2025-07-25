# Internet of Things Challenges

This repository contains solutions and detailed reports for a series of Internet of Things (IoT) challenges and homework assignments, focusing on various aspects of IoT system design, implementation, communication protocols, and energy optimization.

## Challenges
- **Challenge 1**: Wokwi and Power Consumption
- **Challenge 2**: Packet Sniffing
- **Challenge 3**: Node-Red & LoRaWAN
- **Homework**: Low-Cost IoT System for Forklift Tracking and Monitoring, 802.15.4 and RFID

### [Challenge 1](./challenge1)
- Designed a parking node on Wokwi using an ultrasonic sensor and ESP-NOW.
- Implemented deep sleep + duty cycle logic.
- Estimated energy consumption and battery lifetime using real measurements.
- Suggested optimizations to reduce power usage.
- Solved a theoretical exercise on sink positioning in wireless sensor networks.

**Tools**: Wokwi, CSV data analysis

### [Challenge 2](./challenge2)
- Analyzed a PCAP file to study CoAP, MQTT, MQTT-SN messages.
- Compared energy consumption of Wi-Fi devices using CoAP vs MQTT.
- Proposed strategies to minimize energy usage with an MQTT broker.

**Tools**: Wireshark, PCAP analysis, energy modeling

### [Challenge 3](./challenge3)
1. Node-RED Workflow
    - Created a full Node-RED MQTT flow: message generation, logging, filtering, plotting.
    - Processed CSV input based on dynamic IDs.
    - Handled MQTT ACKs and posted counter data to ThingSpeak.

2. LoRaWAN Theory
    - Calculated spreading factor to ensure 70% delivery success.
    - Designed a LoRa-based system using Arduino MKR WAN 1310 + DHT22 + ThingSpeak.
    - Simulated and reproduced LoRaWAN scalability results using LoRaSim.

**Tools**: Node-RED, Mosquitto, ThingSpeak, LoRaSim

### [Homework](./homework)
1. Forklift IoT System
    - Designed a low-cost system for real-time forklift monitoring (location, impact).
    - Described full hardware/software stack + backend architecture.

2. IEEE 802.15.4 Analysis
    - Analyzed beacon-enabled network for camera nodes.
    - Computed output rate PMF, CFP slot scheduling, and duty cycle constraints.

3. RFID with Dynamic Frame ALOHA
    - Evaluated ALOHA efficiency (η) for N tags across frame sizes.