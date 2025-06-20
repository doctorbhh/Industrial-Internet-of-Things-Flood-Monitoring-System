# Industrial-Internet-of-Things-Flood-Monitoring-System  


## Overview

The **Industrial Internet of Things Flood Monitoring System** is an IoT-based project designed to monitor water levels and detect potential flood conditions in real-time. Using an ultrasonic sensor and an ESP32 microcontroller, the system measures water levels and transmits data to Adafruit IO via MQTT. When a flood condition is detected (water level below a certain threshold), the system triggers an alert, which can be monitored remotely.

## Features

- **Real-time Water Level Monitoring:** Uses ultrasonic sensors for accurate distance measurement to the water surface.
- **MQTT Connectivity:** Publishes water level and flood status to Adafruit IO feeds.
- **Flood Alerts:** Activates an LED and sends a notification when water level crosses the flood threshold.
- **WiFi-enabled:** Connects to local WiFi for seamless IoT integration.
- **Easy Hardware Integration:** Minimal components required for setup.

## Hardware Requirements

- ESP32 Development Board
- Ultrasonic Sensor (HC-SR04 or similar)
- LED (for flood alert)
- Jumper wires & Breadboard

## Software Requirements

- Arduino IDE
- Libraries:
  - WiFi.h
  - Adafruit_MQTT.h
  - Adafruit_MQTT_Client.h

## Setup and Usage

1. **Hardware Wiring:**
   - Connect the ultrasonic sensor's TRIG to GPIO 5 and ECHO to GPIO 18 on the ESP32.
   - Connect the LED to GPIO 2 through a suitable resistor.

2. **Configure WiFi and MQTT:**
   - Update the `ssid` and `password` in the code for your WiFi.
   - Set your Adafruit IO `AIO_USERNAME` and `AIO_KEY`.

3. **Upload the Code:**
   - Open `sketch.ino` in Arduino IDE.
   - Install required libraries via Library Manager if needed.
   - Upload the sketch to your ESP32.

4. **Monitoring:**
   - View water level and flood status updates on your Adafruit IO dashboard.
   - The onboard LED will light up if a flood condition is detected.

## Code Structure

- **`sketch.ino`:** Main Arduino sketch handling sensor readings, MQTT communication, and flood logic.

### Main Logic Snippet

```c++
void loop() {
  MQTT_connect();

  long distance = measureDistance();
  Serial.print("Measured Distance (cm): ");
  Serial.println(distance);

  // Send water level (0-100)
  waterLevelFeed.publish(distance);

  if (distance < floodThreshold) {
    digitalWrite(ALERT_LED, HIGH);
    floodStatusFeed.publish("ON");
    Serial.println("⚠️ Flood Alert Sent!");
  } else {
    digitalWrite(ALERT_LED, LOW);
    floodStatusFeed.publish("OFF");
    Serial.println("✅ Status Safe");
  }

  delay(5000); // every 5 seconds
}
```

## Customization

- Adjust `floodThreshold` in the code to set your safe water level.
- Add more sensors or outputs (e.g., buzzer, GSM alerts) as needed.

## License

This project is open-source. Please check with the repository owner for licensing details.

## Author

[doctorbhh](https://github.com/doctorbhh)

---

*Feel free to open an issue or PR for improvements or questions!*
