/*
 * Parte 1 - Arduino
 * 
 * Automação da partida do motor
 * Autimação dos farois
 * 
 * by Felipe Alessander Caetano Luiz
*/

#include "Servo.h"
#include <OneWire.h>
#include <DallasTemperature.h>

//definindo pinos de entradas
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;

#define Motor_on_input 3


//definindo pinos de saídas
#define  chave_on_output 4
#define chave_ACC_output 5
#define motor_arranque_output 6
#define injetor_output 7
#define farol_output 8
#define farolmilha_output 9
#define AQUECEDOR_injetor_output 10
#define AQUECEDOR_combustivel_output 11
#define ServoMotor 12

#define lanterna_output 13
#define farol_output 14
#define farol_de_milha_output 15


#define CHAVE_input 16

#define Sensor_LDR A5

Servo meuservo; // Cria o objeto servo para programação 


//definindo variveis
int Chave = 0;
int vfarol = 0;
int vfarolmilha = 0;
int temp = 0;
boolean Status_car = false;

int angulo = 0;
int MOTOR_on = 0;
int LUZ = 0;

unsigned long time0;
unsigned long time1;
unsigned long time2;
unsigned long time3;

void setup() {
  sensors.begin();
  
//definindo entradas

pinMode(Motor_on_input, INPUT);


pinMode(CHAVE_input, INPUT);


//definindo saídas
pinMode(chave_on_output, OUTPUT);
pinMode(chave_ACC_output, OUTPUT);
pinMode(motor_arranque_output, OUTPUT);
pinMode(injetor_output, OUTPUT);
pinMode(AQUECEDOR_injetor_output, OUTPUT);
pinMode(AQUECEDOR_combustivel_output, OUTPUT);

pinMode(lanterna_output, OUTPUT);
pinMode(farol_output, OUTPUT);
pinMode(farol_de_milha_output, OUTPUT);


meuservo.attach(ServoMotor);

//Iniciando rele com desligado
digitalWrite(AQUECEDOR_injetor_output, LOW);
digitalWrite(AQUECEDOR_combustivel_output, LOW);
digitalWrite(chave_on_output, LOW);
digitalWrite(chave_ACC_output, LOW);
digitalWrite(motor_arranque_output, LOW);
digitalWrite(injetor_output, LOW);
digitalWrite(lanterna_output, LOW);
digitalWrite(farol_output, LOW);
digitalWrite(farol_de_milha_output, LOW);
meuservo.write(angulo);
}

void loop() {

Chave = digitalRead(CHAVE_input);
Car_On();

if (Chave == 1) {

Status_car = Car_ONLINE();
farois();
  if (Status_car) {  // VERIFICAÇÀO PARA VER SE O MOTOR JA ESTA LIGADO
    digitalWrite(chave_ACC_output, LOW);
    Car_partida();
   }else{ if ((temp == 1)and (!Status_car)) { // Carro esta ligado so que a temperatura do motor ainda esta baixa, ele vai ligar o aquecedor do motor e dai deixar o carro acelerado um pouco
      digitalWrite(AQUECEDOR_combustivel_output, temp);
      meuservo.write(angulo + 180);
      temp = temperatura_motor();
      digitalWrite(chave_ACC_output, HIGH);
      farois();
    }else{
      digitalWrite(AQUECEDOR_combustivel_output, LOW); // Carro esta ligado com a temperatura certa entao ele deligará o aquecedor e volta o acelerador ao normal
      meuservo.write(angulo);
      digitalWrite(chave_ACC_output, HIGH);
      farois();
  }
  }
  
}else{
    digitalWrite(AQUECEDOR_injetor_output, LOW);
    digitalWrite(AQUECEDOR_combustivel_output, LOW);
    digitalWrite(chave_on_output, LOW);
    digitalWrite(chave_ACC_output, LOW);
    digitalWrite(motor_arranque_output, LOW);
    digitalWrite(injetor_output, LOW);
    digitalWrite(lanterna_output, LOW);
    digitalWrite(farol_output, LOW);
    digitalWrite(farol_de_milha_output, LOW);
    meuservo.write(angulo);
}
  
}




void Car_On(){
  digitalWrite(chave_on_output, HIGH);  
}


void Car_partida(){
    time0 = millis();
    time1 = 0;
    temp = temperatura_motor();
    if (time0 <= time1 + 5000){  //NUM PERIODO DE 5 SEGUNDOS O CARRO VAI ESQUENTAR CASO O COMBUSTIVEL ESTEJE ABAIXO DE 50°C E INJETAR-LO (GASOLINA)
      digitalWrite(AQUECEDOR_injetor_output, temp);
      digitalWrite(injetor_output, HIGH);
      
    }else{  // APOS O TEMPO EXECIDO ELE DESLIGARÁ O AQUECEDOR E O INJETOR DE GASOLINA
      time0 = millis();
      time2 = millis();
      digitalWrite(AQUECEDOR_injetor_output, LOW);
      digitalWrite(injetor_output, LOW);
      temp = temperatura_motor();
      digitalWrite(AQUECEDOR_combustivel_output, temp); // LIGAR O AQUECEDOR DO COMBUSTIVEL CASO O CARRO ESTERJE FRIO
      
      if ((time0 <= time2 + 3000) and (Status_car)) { //O ARDUINO VAI ACELERAR O CARRO E POR 3 SEGUNDOS VAI LIGAR O MOTOR DE ARRANQUE PARA QUE LIGUE O CARRO 
         meuservo.write(angulo + 180);
         digitalWrite(motor_arranque_output, HIGH);
         
      }else {
        digitalWrite(motor_arranque_output, LOW); //Desligar O MOTOR DE ARRANQUE
        temp = temperatura_motor();
      }   
    temp = temperatura_motor();
    }     
}



boolean Car_ONLINE(){ // Funcao que verifica e retorna se o motor esta ligado ou nao

MOTOR_on = digitalRead(Motor_on_input);

if (MOTOR_on == 0) {
  return true;
}else{
  return false;
}

}

int temperatura_motor(){ // Funcao que verifica e retorna se a temperatura do motor esta baixa ou nao
sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
  if (tempC < 50) {
    return 1;
  }else{
    return 0;
  }
  
}

void farois(){
  digitalWrite(lanterna_output, HIGH);
  LUZ = analogRead(Sensor_LDR);

    if ((LUZ <= 500) and (!Status_car))    
  {  
    digitalWrite(farol_output, HIGH);
    digitalWrite(farol_de_milha_output, HIGH);   
  }
  else  //Caso contrário, apaga o led  
  {  
    digitalWrite(farol_output, LOW);
    digitalWrite(farol_de_milha_output, LOW);
  } 

  
}
