/*
 * ==============================NODEMCU MQTT NODE-RED=================================
 * 
 * ===================================REMOTE REMOTE=======================================
 * 
 * ==================================AANG RAPE'I=======================================
 * 
 * ==================================27-01-2021========================================
 * 
 * ============================= KONEKSI PIN DAN SERVO ==============
`* PIN D5 --> SERVO WARNA ORANGE
`* 3V     --> SERVO WARNA MERAH
 * G      --> SERVO WARNA COKELAT
 */
#include <ESP8266WiFi.h> //Library  ESP8266
#include <PubSubClient.h> //Library MQTT
#include <Servo.h>

Servo myservo;
int servo_pos = 0;
//Setup WIFI
const char*   ssid        = "MODEM"; //Nama SSID Wifi yang akan diakses!
const char*   pass        = "tarungjaya"; //Password Wifi

//Setup MQTT broker
const char*   mqtt_server = "broker.hivemq.com"; //Server MQTT/Broker
const int     mqtt_port   = 1883; //MQTT Port
const char*   mqttuser    = ""; //MQTT Username 
const char*   mqttpass    = ""; //MQTT Password
String        clientId    = "ESP8266Client-"; //MQTT ClientID



WiFiClient espclient;
PubSubClient client(espclient);

//Fungsi Menyambungkan ke Wifi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//Fungsi Menerima Pesan dari MQTT Broker
void callback(String topic, byte* payload, unsigned int length) {
  Serial.print("message arrived[");
  Serial.print(topic);
  Serial.println("]");
  String msgg;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msgg += (char)payload[i];
  }

  if(servo_pos != msgg.toInt()){
    myservo.write(msgg.toInt());
    Serial.print("\n Servo Write ");
    Serial.print(msgg);
    servo_pos = msgg.toInt();
  } 
}



//Fungsi Menyambungkan Ulang ke MQTT Broker
void reconnect() {
  while (!client.connected()) {
    clientId += String(random(0xffff), HEX);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(mqtt_server);
    if (client.connect(clientId.c_str(), mqttuser, mqttpass )) {
      Serial.println("connected");
      client.subscribe("ke_alat");
    } else {
      Serial.print("failed with state, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

//Fungsi ini dijalankan sekali ketika NodeMCU mulai start
void setup() {
  Serial.begin(115200);
  setup_wifi();
  myservo.attach(D5);//servo pin
  myservo.write(0);
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback); 
}

//Fungsi ini dijalakan berulang-ulang selama NodeMCU menyala
void loop(){
 
 if (!client.connected()) { //menyambungkan kembali jika terputus dengan MQTT Broker
    reconnect();
 }
 
 if(!client.loop()){ //menyambungkan kembali menerima pesan jika terputus dengan MQTT Broker
    client.connect("AangESP8266Client",  mqttuser, mqttpass );
 }


delay(200);

}
