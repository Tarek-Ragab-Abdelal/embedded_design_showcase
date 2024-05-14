# ESP32 DHT22 Sensor Data Logger with WebSocket

## Table of Contents

- [Showcase](#showcase)
  - [Key Features](#key-features)
  - [Design Patterns Used](#design-patterns-used)
  - [Showcase Value](#showcase-value)
- [Functionality](#functionality)
  - [Hardware Setup](#hardware-setup)
  - [Software Setup](#software-setup)
  - [Usage](#usage)
- [Dependencies](#dependencies)
- [License](#license)

## Showcase

Welcome to the ESP32 DHT22 Sensor Data Logger project! This showcase highlights the key features, design patterns, and functionality of the project.

### Key Features

- Real-time monitoring of temperature and humidity data.
- WebSocket communication for remote access and visualization.
- Modular and scalable architecture using design patterns.

### Design Patterns Used

#### Singleton Pattern

The `SensorManager` class is implemented as a Singleton to ensure only one instance of the sensor manager exists throughout the program's lifecycle. This provides global access to sensor functionality and prevents multiple instances from conflicting with each other.

#### Observer Pattern

The Observer pattern is employed to handle sensor data updates efficiently. The `ISensorObserver` interface defines the contract for sensor data observers, while concrete observer classes (`Logger` and `WebSocketsClient`) implement specific behavior for logging and broadcasting sensor data, respectively. Observers register themselves with the `SensorManager` to receive notifications whenever sensor data changes, allowing for flexible and decoupled event handling.

### Showcase Value

- **Modularity**: The project's architecture promotes modularity, making it easy to add new sensors or observers without modifying existing code. This enhances code reusability and maintainability, especially in IoT applications where hardware configurations may vary.
- **Scalability**: The project can scale to support multiple sensors or different sensor types effortlessly. By extending the existing architecture, developers can accommodate diverse use cases and expand the project's functionality as needed.
- **Real-time Monitoring**: WebSocket communication enables real-time monitoring of sensor data, facilitating remote access and visualization of temperature and humidity readings. This feature is particularly useful for monitoring environmental conditions in smart home, agriculture, or industrial applications.

## Functionality

### Hardware Setup

1. Connect the DHT22 sensor to the ESP32 board as per the wiring instructions.
2. Ensure the ESP32 board is powered on and properly connected.

### Software Setup

1. Install the required libraries using the Arduino Library Manager:
   - `DHT` library: https://github.com/adafruit/DHT-sensor-library
   - `WebSocketsServer` library: https://github.com/Links2004/arduinoWebSockets
   - `WiFi` library: Included with the Arduino IDE or PlatformIO.
   - `Adafruit_Sensor` library: https://github.com/adafruit/Adafruit_Sensor
2. Open the provided sketch in the Arduino IDE or PlatformIO.
3. Replace `ssid` and `password` with your WiFi credentials.
4. Compile and upload the sketch to your ESP32 board.

### Usage

1. Open the Serial Monitor to view the IP address of the ESP32 and WebSocket server.
2. Connect clients to the WebSocket server using the provided URL to receive real-time sensor updates.
3. Monitor temperature and humidity data from connected clients in real-time.

## Dependencies

- [Arduino Core for ESP32](https://github.com/espressif/arduino-esp32)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [WebSockets](https://github.com/Links2004/arduinoWebSockets)
- [Adafruit Sensor Library](https://github.com/adafruit/Adafruit_Sensor)

## License

This project is licensed under the [MIT License](LICENSE).

### Support Me

<p style="display: inline-block; margin-right: 0.25rem;"><a style="padding: 10px;" href="https://www.buymeacoffee.com/tarekragab"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" width="150"/></a>
<a style="padding: 10px;" href="https://www.ko-fi.com/tarekragab"><img src="https://storage.ko-fi.com/cdn/kofi2.png?v=3" width="150"/></a></p>
</ul>
