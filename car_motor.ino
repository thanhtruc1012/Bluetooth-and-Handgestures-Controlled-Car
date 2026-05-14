#include<WiFi.h>
#include <ArduinoBLE.h>
#include<ArduinoOTA.h>
#include<Arduino.h>
//set up for bluetooth
BLEService uartService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); 
BLEStringCharacteristic rxChar("6E400002-B5A3-F393-E0A9-E50E24DCCA9E", BLEWrite | BLEWriteWithoutResponse, 20);
const char* ssid = "Your Wifi name";
const char* password = "Your Wifi Password";
//This code is from Judy Cutie
WiFiServer server(1210);

const int ENA = D5,
          IN1 = D7,
          IN2 = D4,
          IN3 = D2,
          IN4 = D6,
          ENB = D3;
const int pwmFreq = 1000;
const int pwmResolution = 8;
const int channelA = 0;
const int channelB = 1;

const int LED_red = 10,
          LED_green = 11,
          LED_yellow = 12;


int Motor_pins[6] = {ENA, IN1, IN2, IN3, IN4, ENB};
//connect wrong wires so LOW is HIGH
//HIGH is LOW
void stop(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  ledcWrite(channelA, 0);
  ledcWrite(channelB, 0);
  digitalWrite(LED_red, HIGH);
  digitalWrite(LED_yellow, LOW);
  digitalWrite(LED_green, LOW);
}
void turnLeft(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(channelA, 150);
  ledcWrite(channelB, 175);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_yellow, LOW);
  digitalWrite(LED_red, LOW);
  
}
void turnRight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);//banh trai tien len
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);//banh phai di lui
  ledcWrite(channelA, 175);
  ledcWrite(channelB, 150);//co nghia la chay tai no nguoc lai
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_yellow, LOW);
  digitalWrite(LED_red, LOW);
}

void reverse(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  ledcWrite(channelA, 175);
  ledcWrite(channelB, 175);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_yellow, LOW);
  digitalWrite(LED_red, LOW);
}

void goStraight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  ledcWrite(channelA, 175);
  ledcWrite(channelB, 175);
  digitalWrite(LED_green, LOW);
  digitalWrite(LED_yellow, LOW);
  digitalWrite(LED_red, LOW);
}

void SpeedUp(){
  ledcWrite(channelA, 255);
  ledcWrite(channelB, 255);
  digitalWrite(LED_green, HIGH);
  digitalWrite(LED_yellow, LOW);
  digitalWrite(LED_red, LOW);
}

void SlowDown(){
  ledcWrite(channelA, 100);
  ledcWrite(channelB, 100);
  digitalWrite(LED_yellow, HIGH);
  digitalWrite(LED_red, LOW);
  digitalWrite(LED_green, LOW);
}
String command = "S";  
void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("!");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
  server.begin();

  Serial.println("Server started");


  ArduinoOTA.begin();
  
  for (int i = 0; i < 6; i++){
    pinMode(Motor_pins[i], OUTPUT);
  }
  pinMode(LED_red, OUTPUT);
  pinMode(LED_green, OUTPUT);
  pinMode(LED_yellow, OUTPUT);
  ledcSetup(channelA, pwmFreq, pwmResolution);
  ledcAttachPin(ENA, channelA);

  ledcSetup(channelB, pwmFreq, pwmResolution);
  ledcAttachPin(ENB, channelB);

  
  if (!BLE.begin()) {
    Serial.println("Bluetooth Error!");
    while (1);
  }


  BLE.setLocalName("Test_Nano");
  BLE.setAdvertisedService(uartService);
  uartService.addCharacteristic(rxChar);
  BLE.addService(uartService);
  BLE.advertise();



  
}

void loop(){
  ArduinoOTA.handle();
  BLEDevice central = BLE.central();
  //Stop in the beginning
  
  //hand gesture with usb cable
  if (Serial.available()) {
    char c = Serial.read();
    Serial.println(c); 

      if (c == 'F') {
        goStraight();
        
      } 
      else if (c == 'L'){
        turnLeft();
        
      }
      else if (c == 'R'){
        turnRight();
        
      }
      else if (c == 'D'){
        reverse();
        

      }
      else if (c == 'S') {
        stop();
        
      }

      else
        stop();
  }
  // hand gesture without usb cable
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected");

    while (client.connected()) {

      if (client.available()) {
        char c = client.read();
        if (c == 'F') {
          goStraight();
          Serial.println("Go Straight");
      } 
        else if (c == 'L'){
          turnLeft();
          Serial.println("Go Left");
        }
        else if (c == 'R'){
          turnRight();
          Serial.println("Go Right");
        }
        else if (c == 'D'){
          reverse();
          Serial.println("Go back");

        }
        else if (c == 'S') {
          stop();
          Serial.println("Stop");
        }
        else
          stop();

        

        client.print("Success\n");
      }
    }

    client.stop();
    Serial.println("Client disconnected");
  }
  //blue tooth control
  if (central) {
    while (central.connected()) {
      if (rxChar.written()) {
          command = rxChar.value();
      }
      if (command == "F") {
        goStraight();
        //Serial.println("Go Straight");
      } 
      else if (command == "L"){
        turnLeft();
        //Serial.println("Go Left");
      }
      else if (command == "R"){
        turnRight();
        //Serial.println("Go Right");
      }
      else if (command == "D"){
        reverse();
        //Serial.println("Go back");

      }
      else if (command == "U"){
        SpeedUp();
      }
      else if (command == "E"){
        SlowDown();
      }
      else if (command == "S") {
        stop();
        //Serial.println("Stop");
      }
      else
        stop();
        
    }
  }
  



 

}

