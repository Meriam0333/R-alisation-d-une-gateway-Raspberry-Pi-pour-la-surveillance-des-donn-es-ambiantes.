# Réalisation-d'une-gateway-Raspberry-Pi-pour-la-surveillance-des-données-ambiantes.

Description :

Ce projet permet de récupérer les données de capteurs (température, humidité, pression)
depuis la carte Arduino nano 33 BLE Sense via BLE. Le Raspberry Pi agit comme passerelle 
(gateway) pour recevoir ces données, les publier sur MQTT, les stocker dans Firebase 
Realtime Database,et les visualiser en temps réel avec Node-RED Dashboard. 

Fonctionnalités principales :
1. Lecture des capteurs Arduino via BLE par le Raspberry Pi.
2. Publication des mesures sur MQTT depuis le Pi.
3. Node-RED sur le Pi :
   - Function nodes pour séparer les capteurs.
   - Firebase-modify nodes pour stocker les mesures avec timestamp/date.
   - Dashboard nodes pour visualisation en gauges et charts.
4. Historique complet dans Firebase avec horodatage (UTC+1).

Installation / Déploiement :
1. Installer Node-RED sur Raspberry Pi :
   sudo apt install -y nodejs npm
   sudo npm install -g --unsafe-perm node-red
2. Installer les palettes nécessaires :
   - node-red-dashboard
   - node-red-contrib-firebase
3. Configurer Firebase Realtime Database et générer les credentials.
4. Brancher le flow Node-RED et configurer les nodes :
   - MQTT-in pour capteurs
   - Function node pour formater données
   - Firebase-out pour stocker chaque capteur
   - Dashboard nodes pour visualiser

Usage :
- Lancer Node-RED : node-red-start
- Accéder au dashboard : http://<IP_PI>:1880/ui
- Les mesures sont stockées automatiquement dans Firebase.

Remarques :
- Le Raspberry Pi agit comme gateway BLE pour l’Arduino.
- Les timestamps sont en UTC+1.
- Les push dans Firebase permettent de garder un historique continu.

