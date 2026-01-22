import asyncio
import json
import struct
from bleak import BleakClient
import paho.mqtt.client as mqtt

# ================= CONFIG =================
ARDUINO_ADDRESS = "32:62:8B:C4:49:5A"
MQTT_BROKER = "test.mosquitto.org" 
MQTT_PORT = 1883
MQTT_TOPIC = "arduino/sensors"

TEMP_UUID  = "2a6e"
HUM_UUID   = "2a6f"
PRESS_UUID = "2a6d"

# ================= MQTT ===================
mqtt_client = mqtt.Client()
mqtt_client.connect(MQTT_BROKER, MQTT_PORT, 60)
mqtt_client.loop_start()

# ================= BLE ====================
async def main():
    print("------------Connexion a arduino Sense en cours-----------------")
    async with BleakClient(ARDUINO_ADDRESS, timeout=50.0) as client:
        print("------------------------CONNECTE!--------------------------")

        await asyncio.sleep(1)

        while True:
            #Lire les donnees
            temp  = await client.read_gatt_char(TEMP_UUID)
            hum   = await client.read_gatt_char(HUM_UUID)
            press = await client.read_gatt_char(PRESS_UUID)

            #Decoder les donnees en 'float'
            temperature = struct.unpack('<f', temp)[0]
            humidite    = struct.unpack('<f', hum)[0]
            pression    = struct.unpack('<f', press)[0]

            #Creation dictionnaire pour publication MQTT
            data = {
                "temperature": round(temperature, 2),
                "humidite": round(humidite, 2),
                "pression": round(pression, 2)
            }

            #Publier sur MQTT
            mqtt_client.publish(MQTT_TOPIC, json.dumps(data))
            print("MQTT -->", data)

            await asyncio.sleep(2)

asyncio.run(main())
