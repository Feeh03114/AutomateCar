#include "Servo.h"

//definindo pinos de entradas
#define sensorTemp1_input A0
#define sensorLums_input A1

#define Motor_on_input 2


//definindo pinos de saídas
#define  chave_on_output 3
#define chave_ACC_output 4
#define motor_arranque_output 5
#define injetor_output 6
#define farol_output 7
#define farolmilha_output 8
#define AQUECEDOR_injetor_output 9
#define AQUECEDOR_combustivel_output 10
#define ServoMotor 11


#define CHAVE_input 12

Servo meuservo; // Cria o objeto servo para programação 
int angulo = 0;

//definindo variveis
int Chave = 0;
int vfarol = 0;
int vfarolmilha = 0;


unsigned long time0;
unsigned long time1;
unsigned long time2;
unsigned long time3;

void setup() {
  
//definindo entradas
pinMode(sensorLums_input, INPUT);
pinMode(sensorTemp1_input, INPUT);
pinMode(sensorTemp2_input, INPUT);

pinMode(Motor_on_input, INPUT);


pinMode(CHAVE_input, INPUT);


//definindo saídas
pinMode(chave_on_output, OUTPUT);
pinMode(chave_ACC_output, OUTPUT);
pinMode(motor_arranque_output, OUTPUT);
pinMode(injetor_output, OUTPUT);
pinMode(AQUECEDOR_injetor_output, OUTPUT);
pinMode(AQUECEDOR_combustivel_output, OUTPUT);


meuservo.attach(ServoMotor);

//Iniciando rele com desligado
digitalWrite(AQUECEDOR_injetor_output, LOW);
digitalWrite(AQUECEDOR_combustivel_output, LOW);
digitalWrite(chave_on_output, LOW);
digitalWrite(chave_ACC_output, LOW);
digitalWrite(motor_arranque_output, LOW);
digitalWrite(injetor_output, LOW);
meuservo.write(angulo);
}

void loop() {




Chave = digitalRead(CHAVE_input);
Car_On()
digitalWrite(chave_ACC_output, HIGH);
if (Chave == 1) {

boolean Status_car = Car_ONLINE();
  if (Status_car) {  // VERIFICAÇÀO PARA VER SE O MOTOR JA ESTA LIGADO
    digitalWrite(chave_ACC_output, LOW);
    Car_partida()
   }else{ if ((temp == 1)and (!Status_car)) { // Carro esta ligado so que a temperatura do motor ainda esta baixa, ele vai ligar o aquecedor do motor e dai deixar o carro acelerado um pouco
     digitalWrite(AQUECEDOR_combustivel_output, temp);
      meuservo.write(angulo + 180);
      temp = temperatura_motor();
      digitalWrite(chave_ACC_output, HIGH);
    }else{
      digitalWrite(AQUECEDOR_combustivel_output, LOW); // Carro esta ligado com a temperatura certa entao ele deligará o aquecedor e volta o acelerador ao normal
      meuservo.write(angulo);
      digitalWrite(chave_ACC_output, HIGH);
  }
  }
  
}else{
  
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



boolean Car_ONLINE(){

MOTOR_on = digitalRead(Motor_on_input);

if (MOTOR_on == 0) {
  return true;
}else{
  return false;
}

}

int temperatura_motor(){
int temperature = temp.getTemp();
  if (temperature < 50) {
    return 1;
  }else{
    return 0;
  }
  
}
