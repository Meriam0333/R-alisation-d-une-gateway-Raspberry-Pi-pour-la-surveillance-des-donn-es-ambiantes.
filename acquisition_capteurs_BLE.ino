#include <ArduinoBLE.h>
#include <Arduino_HS300x.h>
#include <Arduino_LPS22HB.h>

// Définir le service BLE et les caractéristiques
BLEService sensorService("180C"); // Service environnemental

// Caractéristiques pour chaque capteur
BLEFloatCharacteristic tempChar("2A6E", BLERead | BLENotify);
BLEFloatCharacteristic humChar("2A6F", BLERead | BLENotify);
BLEFloatCharacteristic pressChar("2A6D", BLERead | BLENotify);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  // Initialiser les capteurs
  if (!HS300x.begin()) {
    Serial.println("Erreur capteur HS300x!");
    while (1);
  }
  
  if (!BARO.begin()) {
    Serial.println("Erreur capteur LPS22HB!");
    while (1);
  }
  
  // Initialiser BLE
  if (!BLE.begin()) {
    Serial.println("Erreur démarrage BLE!");
    while (1);
  }
  
  // Configuration BLE
  BLE.setLocalName("ArduinoSense");
  BLE.setAdvertisedService(sensorService);
  
  // Ajouter les caractéristiques au service
  sensorService.addCharacteristic(tempChar);
  sensorService.addCharacteristic(humChar);
  sensorService.addCharacteristic(pressChar);
  
  // Ajouter le service
  BLE.addService(sensorService);
  
  // Démarrer l'advertising
  BLE.advertise();
  
  Serial.println("Arduino BLE prêt - En attente de connexion...");
}

void loop() {
  BLE.poll();
  // Attendre une connexion BLE
  BLEDevice central = BLE.central();
  
  if (central) {
    Serial.print("Connecté à: ");
    Serial.println(central.address());
    
    while (central.connected()) {
      // Lire les capteurs
      float temperature = HS300x.readTemperature();
      float humidity = HS300x.readHumidity();
      float pressure = BARO.readPressure() * 10; // Conversion en hPa
      
      // Mettre à jour les caractéristiques BLE
      tempChar.writeValue(temperature);
      humChar.writeValue(humidity);
      pressChar.writeValue(pressure);
      
      // Affichage debug
      Serial.print("T: ");
      Serial.print(temperature);
      Serial.print("°C | H: ");
      Serial.print(humidity);
      Serial.print("% | P: ");
      Serial.print(pressure);
      Serial.println(" hPa");
      
      delay(2000); // Lecture toutes les 2 secondes
    }
    
    Serial.println("Déconnecté");
  }
}
