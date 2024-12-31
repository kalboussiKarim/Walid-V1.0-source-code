#include <WiFi.h>           // pour pouvoir connecter au routeur
#include <PubSubClient.h>   // pour pouvoir cpnnecter au broker MQTT
#include <ESP32Servo.h>     // pour utiliser les servo moteurs

/**********************Sleep Mode*************************/
#define TIME_TO_SLEEP 30 
#define uS_TO_S_FACTOR 1000000


#define WIFI_PIN 25
#define MQTT_PIN 26
#define ON_PIN 27

/************************Horn********************************/
#define HORN_PIN 13

/************************Pan*********************************/
#define PAN_PIN 14
Servo panServo;
int pan = 90;

/************************Tilt*********************************/
#define TILT_PIN 15
Servo tiltServo;
int tilt = 90;

/**********************Moteur à droite************************/
int enableRightMotor=22; 
int rightMotorPin1=21;
int rightMotorPin2=19;

/**********************Moteur à gauche************************/
int enableLeftMotor=23;
int leftMotorPin1=18;
int leftMotorPin2=5;


/*************************************************************/
bool state = false;
String direction= "S" ;
int horn = 0;

#define FLASH_PIN 4
#define CAMERA_PIN 17
int flash = 0;
int camera = 0;


/*********************Parmetres du routeur********************/
const char* ssid = ssid_S;                       // valeur vient de secrets.h
const char* password = password_S;               // valeur vient de secrets.h
/*************************************************************/


/******************Parmetres du serveur MQTT******************/
const char* mqtt_server = mqtt_server_S;        // valeur vient de secrets.h
const char* mqttUser = mqttUser_S;              // valeur vient de secrets.h
const char* mqttPassword = mqttPassword_S;      // valeur vient de secrets.h
/*************************************************************/

WiFiClient espClient;
PubSubClient client(espClient);



void setup_wifi() {
  delay(10);
  digitalWrite(WIFI_PIN,HIGH);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  
  WiFi.printDiag(Serial);
  Serial.println(WiFi.channel());
  unsigned long currentMillis = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(millis() - currentMillis > 10000){
      Serial.println("restart");
      digitalWrite(WIFI_PIN,LOW);
      ESP.restart();
    }
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(WIFI_PIN,LOW);
}

void reconnect() {
  
  while (!client.connected()) {
    digitalWrite(MQTT_PIN,HIGH);
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Rover", mqttUser, mqttPassword)) {
      Serial.println("connected");
      
      client.subscribe("robot/test");
      client.subscribe("robot/direction");
      client.subscribe("robot/flash");
      client.subscribe("robot/horn");
      client.subscribe("robot/pan");
      client.subscribe("robot/tilt");
      digitalWrite(MQTT_PIN,LOW);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // nestaneou 5 secondes bech namlou essaie okhra pour reconnecter
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String msgg;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    msgg += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "robot/test") {
    Serial.println(msgg);
    if(msgg == "on"){
      state=true;
      digitalWrite(CAMERA_PIN,LOW);
      client.publish("robot/moving_status", "on");
      delay(1000);
    }
    else if(msgg == "off"){
      state=false;
      client.publish("robot/moving_status", "off");
      digitalWrite(WIFI_PIN,LOW);
      digitalWrite(MQTT_PIN,LOW);
      digitalWrite(ON_PIN,LOW);
      digitalWrite(FLASH_PIN,HIGH);
      digitalWrite(CAMERA_PIN,HIGH);
      delay(1000);
      esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
      esp_deep_sleep_start();
    }
  }else if (String(topic) == "robot/direction") {
    direction = msgg;
    Serial.print("direction : ");
    Serial.println(direction);
  }else if (String(topic) == "robot/flash") {
    flash  = msgg.toInt();;
    Serial.print("flash : ");
    
    if(flash==1){
      digitalWrite(FLASH_PIN,LOW);
      Serial.println(flash);
    }else if(flash==0){
      digitalWrite(FLASH_PIN,HIGH);
      Serial.println(flash);
    }
  }else if (String(topic) == "robot/horn") {
    horn = msgg.toInt();;
    Serial.print("horn : ");
    Serial.println(horn);
  }else if (String(topic) == "robot/pan") {
    pan = msgg.toInt();;
    Serial.print("pan : ");
    Serial.println(pan);
  }else if (String(topic) == "robot/tilt") {
    tilt = msgg.toInt();;
    Serial.print("tilt : ");
    Serial.println(tilt);
  }
}

void moveCar(String direction)
{
  if (direction =="S")
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);
    digitalWrite(enableRightMotor,LOW);    
    digitalWrite(enableLeftMotor,LOW);
  }
  else if (direction =="F")
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);
    digitalWrite(enableRightMotor,HIGH);    
    digitalWrite(enableLeftMotor,HIGH);      
  }
  else if (direction =="B")
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);
    digitalWrite(enableRightMotor,HIGH);    
    digitalWrite(enableLeftMotor,HIGH);    
  }
  else if (direction =="R")
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);
    digitalWrite(enableRightMotor,HIGH);    
    digitalWrite(enableLeftMotor,HIGH);
  }
  else if (direction =="L")
  {
    
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);
    digitalWrite(enableRightMotor,HIGH);    
    digitalWrite(enableLeftMotor,HIGH);        
  }
  
}


void setup() {

  Serial.begin(115200);

  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  pinMode(FLASH_PIN,OUTPUT);
  pinMode(CAMERA_PIN,OUTPUT);
  digitalWrite(FLASH_PIN,HIGH);
  digitalWrite(CAMERA_PIN,HIGH);


  pinMode(HORN_PIN,OUTPUT);
  panServo.attach(PAN_PIN);
  panServo.setPeriodHertz(50);
  tiltServo.attach(TILT_PIN);
  tiltServo.setPeriodHertz(50);
  pinMode(WIFI_PIN,OUTPUT);
  pinMode(MQTT_PIN,OUTPUT);
  pinMode(ON_PIN,OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  }

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(state){
    moveCar(direction);
    digitalWrite(ON_PIN,HIGH);
    panServo.write(pan);
    tiltServo.write(tilt);
    digitalWrite(HORN_PIN,horn);
  }else{
    digitalWrite(ON_PIN,LOW);
    moveCar("S");
  }

}




