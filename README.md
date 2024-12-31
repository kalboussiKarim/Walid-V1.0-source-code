Robot Walid_V1.0

## Description

Ce fichier contient le code source du projet **Walid_V1.0**, un robot conçu dans le cadre de mon projet de fin d'études. Ce robot est contrôlé via MQTT à l'aide d'un Raspberry Pi qui communique avec Firebase pour échanger des données avec l'utilisateur grâce à une application mobile (le code de l'application est déjà disponible sur mon [GitHub](https://github.com/kalboussiKarim/Agriculture-assistant-final/tree/master)).

### Fonctionnalités

- **Mouvement :** Le robot peut se déplacer dans toutes les directions.
- **Orientation de la tête :** La tête du robot peut s'incliner vers le haut, le bas, la gauche et la droite.
- **Signaux sonores :** Le robot peut émettre des signaux sonores.
- **Caméra :** Contrôle d'une caméra (marche/arrêt) pour diffuser une vidéo en direct à l'utilisateur via Firebase (le code de l'ESP32-Cam n'est pas inclus ici).
- **Mode veille :** Le robot vérifie toutes les 30 secondes si l'utilisateur l'a activé. Si ce n'est pas le cas, il passe en mode veille pour économiser de l'énergie. Lorsqu'il est activé, il reste en écoute des commandes de l'utilisateur jusqu'à la fin de la session.

## Installation

1. Clonez ce dépôt sur votre machine locale :
   ```bash
   git clone https://github.com/kalboussiKarim/Walid-V1.0-source-code.git  
   ```
2. Remplacez les champs dans le fichier `secrets.h` par vos propres paramètres Wi-Fi et MQTT (SSID, mot de passe, serveur MQTT, etc.).

### Prérequis

Pour utiliser ou modifier ce projet, vous aurez besoin de :

- **IDE Arduino** installé sur votre ordinateur.
- **Gestionnaire de cartes ESP32** configuré dans l'IDE Arduino. Suivez [ce guide](https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0/) pour l'installation.

### Bibliothèques utilisées

Assurez-vous d'avoir installé les bibliothèques suivantes dans l'IDE Arduino :

- **WiFi.h** : pour connecter l'ESP32 au routeur.
- **PubSubClient.h** : pour établir une connexion avec le broker MQTT.
- **ESP32Servo.h** : pour contrôler les servomoteurs.

## Contact

Si vous souhaitez obtenir le schéma du circuit de ce robot, vous pouvez me contacter par email à :  
**kalboussikarim3@gmail.com**

![IMG_3469](https://github.com/user-attachments/assets/e37c13f5-0557-4b8c-a469-caba5523b1c5)
