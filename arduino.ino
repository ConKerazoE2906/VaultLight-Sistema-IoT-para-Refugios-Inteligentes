// Colocar sus credenciales
#define BLYNK_TEMPLATE_ID "TMPL2kz1p80XL"
#define BLYNK_TEMPLATE_NAME "Prueba"
#define BLYNK_AUTH_TOKEN "EoA2IzHxFl1A8NaWFmf__etlQStsweG9"
#define BLYNK_PRINT Serial

// Librería para conectar con el servidor de Blynk
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// Libreria para el dth 11
BlynkTimer timer;

// SSID y password de red WiFi
char ssid[] = "CAMPUS_EPN";
char pass[] = "politecnica**";
char token[] = BLYNK_AUTH_TOKEN;

// Librería y pines para conectar al módulo WiFi
// Se usa Serial1 en el Arduino Mega (TX1 y RX1)
//Librería y pines para conectar al modulo wifi
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(4, 5);  // (RX, TX)
ESP8266 wifi(&EspSerial);

//Metodo para encender y apagar el Led del Baño mediante Blynk 
int pinLED_B = 6;
BLYNK_WRITE(V7){
    int valorBoton=param.asInt();
    if(valorBoton==1){
      digitalWrite(pinLED_B,HIGH);
    }else{
      digitalWrite(pinLED_B,LOW);
    }
  }

//Metodo para encender y apagar el Led de la Sala mediante Blynk 
int pinLED_S=7;
BLYNK_WRITE(V0){
    int valorBoton1=param.asInt();
    if(valorBoton1==1){
      digitalWrite(pinLED_S,HIGH);
    }else{
      digitalWrite(pinLED_S,LOW);
    }
  }

//Metodo para encender y apagar el Led de la habitacion mediante Blynk 
int pinLED_H=8;
BLYNK_WRITE(V1){
    int valorBoton2=param.asInt();
    if(valorBoton2==1){
      digitalWrite(pinLED_H,HIGH);
    }else{
      digitalWrite(pinLED_H,LOW);
    }
  }

//Metodo para encender y apagar el Led de la Cocina mediante Blynk 
int pinLED_C=9;
BLYNK_WRITE(V2){
    int valorBoton3=param.asInt();
    if(valorBoton3==1){
      digitalWrite(pinLED_C,HIGH);
    }else{
      digitalWrite(pinLED_C,LOW);
    }
  }

int pinLED_P=10;



void setup() {
  
  // Debug console
  Serial.begin(9600);
  // set the data rate for the SoftwareSerial port
  EspSerial.begin(115200);
  EspSerial.write("AT+UART_DEF=9600,8,1,0,0\r\n");
  EspSerial.write("AT+RST\r\n");
  delay(100);
  EspSerial.begin(9600);
  delay(10);
  Blynk.begin(token, wifi, ssid, pass);


  //Pimode del led del baño
  pinMode(pinLED_B,OUTPUT);

  //Pimode del led de la sala
  pinMode(pinLED_S,OUTPUT);

  //Pimode del led de la habitacion
  pinMode(pinLED_H,OUTPUT);

  //Pimode del led de la Cocina
  pinMode(pinLED_C,OUTPUT);

  //Pimode del led de la Patio
  pinMode(pinLED_P,OUTPUT);





}
// Pin de la fotoresistencia
int LedFotoresistencia = A2;


void loop() {
  //Codigo para el sensor de agua
  int sensor=analogRead(A0);
  Serial.println("LLUVIAAAA");   
  Serial.println(sensor);   
  

  if(sensor>0 && sensor<100){
    Blynk.virtualWrite(V3,"Soleado");
    Blynk.virtualWrite(V4,sensor);
  }else if (sensor>100 && sensor<600){
    Blynk.virtualWrite(V3,"Esta chispeando ");
    Blynk.virtualWrite(V4,sensor);

  }else{
    Blynk.virtualWrite(V3,"Lluvia fuerte ☔ - recoge la ropa");
    Blynk.virtualWrite(V4,sensor);

  }

  //Codigo de la fotoresistencia
  int valorLuz = analogRead(LedFotoresistencia);
  Serial.println("Valor de la luz");
  Serial.println(valorLuz);
  Blynk.virtualWrite(V6,valorLuz);
  if(valorLuz<70){
    digitalWrite(pinLED_P,HIGH);
    Blynk.virtualWrite(V5,"NOCHE 🌃🌃");

  }else{
    digitalWrite(pinLED_P,LOW);
    Blynk.virtualWrite(V5,"DIA 🌄🌄");


  }



  
  
  delay(1000);


  Blynk.run();
  timer.run();
}