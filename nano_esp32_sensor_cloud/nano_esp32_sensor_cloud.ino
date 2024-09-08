#include <DHT.h>;
#include "thingProperties.h"

#define DHTPIN 4
#define DHTTYPE DHT22
#define SOIL_MOISTURE_SENSOR_0 A6
#define SOIL_MOISTURE_SENSOR_1 A7
DHT dht(DHTPIN, DHTTYPE);

/*

Las variables de los sensores de humedad del suelo y del sensor DHT22 se actualizan cada 60 segundos y se envian cada 120 segundos.
- float dht22_hr;
- float dht22_temp;
- String sensor_msg; // Pensado para enviar un mensaje con los valores de cualquier sensor
- int soil_mci;
- int soil_sparkfun;

 */


const unsigned long msgsend = 60000; // Time between messages in milliseconds (60 second)
unsigned long startTime;
unsigned long lastMsg;

void read_soil_sensors()
{
    soil_sparkfun = analogRead(SOIL_MOISTURE_SENSOR_0);
    soil_mci = analogRead(SOIL_MOISTURE_SENSOR_1);
}

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(9600);
    // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
    delay(1500);
    dht.begin();
    // Defined in thingProperties.h
    initProperties();

    // Connect to Arduino IoT Cloud
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);

    /*
       The following function allows you to obtain more information
       related to the state of network and IoT Cloud connection and errors
       the higher number the more granular information you’ll get.
       The default is 0 (only errors).
       Maximum is 4
   */
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();
}

void loop()
{
    ArduinoCloud.update();
    // Your code here
    startTime = millis();
    if (startTime - lastMsg > msgsend)
    {
        read_soil_sensors();
        dht22_hr = dht.readHumidity();
        dht22_temp = dht.readTemperature();
        sensor_msg = "Soil Moisture 1: " + String(soil_sparkfun) + " Soil Moisture 2: " + String(soil_mci) + " Humidity: " + String(dht22_hr) + " Temperature: " + String(dht22_temp);
        lastMsg = startTime;
    }
}
