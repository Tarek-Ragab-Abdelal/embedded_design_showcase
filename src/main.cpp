#include <Arduino.h>
#include <DHT.h>
#include <WebSocketsServer.h>
#include <WiFi.h>

const char *ssid = "Orange-Ragab Abdelal";
const char *password = "Ragab19@61.com";

#define DHTPIN 2
#define DHTTYPE DHT22
#define MAX_OBSERVERS 10

WebSocketsServer webSocketServer(81);

// # ----------------------------------------- ISensorObserver ----------------------------------------- //
/**
 * @brief Interface for sensor data observers.
 */
class ISensorObserver
{
public:
  /**
   * @brief Virtual method to be implemented by observers to handle sensor data changes.
   *
   * @param temperature The temperature value read from the sensor.
   * @param humidity The humidity value read from the sensor.
   */
  virtual void onSensorDataChanged(float temperature, float humidity) = 0;
};

// # ----------------------------------------- Sensor Manager ----------------------------------------- //
/**
 * @brief Singleton class to manage the DHT sensor.
 */
class SensorManager
{
private:
  DHT dht;
  ISensorObserver *observers[MAX_OBSERVERS];
  int observerCount;

  /**
   * @brief Private constructor to ensure Singleton pattern.
   */
  SensorManager() : dht(DHTPIN, DHTTYPE), observerCount(0) {}

  /**
   * @brief Private copy constructor to prevent copying.
   */
  SensorManager(const SensorManager &) = delete;

  /**
   * @brief Private assignment operator to prevent assignment.
   */
  void operator=(const SensorManager &) = delete;

  /**
   * @brief Notify all registered observers with the new sensor data.
   *
   * @param temperature The temperature value read from the sensor.
   * @param humidity The humidity value read from the sensor.
   */
  void notifyObservers(float temperature, float humidity)
  {
    for (int i = 0; i < observerCount; ++i)
    {
      observers[i]->onSensorDataChanged(temperature, humidity);
    }
  }

public:
  /**
   * @brief Get the Singleton instance of the SensorManager.
   *
   * @return SensorManager& Reference to the Singleton instance.
   */
  static SensorManager &getInstance()
  {
    static SensorManager instance;
    return instance;
  }

  /**
   * @brief Initialize the DHT sensor.
   */
  void begin()
  {
    dht.begin();
  }

  /**
   * @brief Read the sensor data and notify observers.
   */
  void readSensor()
  {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    notifyObservers(temperature, humidity);
  }

  /**
   * @brief Add an observer to be notified of sensor data changes.
   *
   * @param observer Pointer to the observer to be added.
   */
  void addObserver(ISensorObserver *observer)
  {
    if (observerCount < MAX_OBSERVERS)
    {
      observers[observerCount++] = observer;
    }
  }
};

// # ----------------------------------------- Observers ----------------------------------------- //
// ----------------------------------------- Logger (Observer) ----------------------------------------- //
/**
 * @brief Concrete observer that logs sensor data to the Serial output.
 */
class Logger : public ISensorObserver
{
public:
  /**
   * @brief Handle sensor data changes by logging them to the Serial output.
   *
   * @param temperature The temperature value read from the sensor.
   * @param humidity The humidity value read from the sensor.
   */
  void onSensorDataChanged(float temperature, float humidity) override
  {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
};

// ----------------------------------------- WebSockets Client (Observer) ----------------------------------------- //
/**
 * @brief Concrete observer that sends sensor data over WebSockets.
 */
class WebSocketsClient : public ISensorObserver
{
public:
  /**
   * @brief Handle sensor data changes by sending them over WebSockets.
   *
   * @param temperature The temperature value read from the sensor.
   * @param humidity The humidity value read from the sensor.
   */
  void onSensorDataChanged(float temperature, float humidity) override
  {
    String data = "Temperature: " + String(temperature) + " °C, Humidity: " + String(humidity) + " %";
    webSocketServer.broadcastTXT(data);
  }
};
// # ----------------------------------------- End of Observers ----------------------------------------- //

// # ----------------------------------------- Sensor Commands ----------------------------------------- //
/**
 * @brief Abstract base class for sensor commands.
 */
class SensorCommand
{
public:
  /**
   * @brief Pure virtual method to execute the command.
   */
  virtual void execute() = 0;
};

/**
 * @brief Concrete command to read sensor data.
 */
class ReadSensorCommand : public SensorCommand
{
public:
  /**
   * @brief Construct a new ReadSensorCommand.
   *
   * @param manager Reference to the SensorManager.
   */
  ReadSensorCommand(SensorManager &manager) : manager(manager) {}

  /**
   * @brief Execute the command to read sensor data.
   */
  void execute() override
  {
    manager.readSensor();
  }

private:
  SensorManager &manager;
};

// # ----------------------------------------- Main Logic ----------------------------------------- //
SensorManager &sensorManager = SensorManager::getInstance();
Logger logger;
WebSocketsClient webSocketsClient;
ReadSensorCommand readSensorCommand(sensorManager);

void setup()
{
  Serial.begin(115200);
  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  sensorManager.begin();
  sensorManager.addObserver(&logger);
  sensorManager.addObserver(&webSocketsClient);

  webSocketServer.begin();

  // Print WebSocket URL
  Serial.print("WebSocket server running at: ws://");
  Serial.print(WiFi.localIP());
  Serial.println(":81");

  // Read sensor data every 2 seconds
  xTaskCreatePinnedToCore(
      [](void *)
      {
        for (;;)
        {
          readSensorCommand.execute();
          vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
      },
      "SensorReaderTask",
      2048,
      NULL,
      1,
      NULL,
      1);
}

void loop()
{
  webSocketServer.loop();
}
