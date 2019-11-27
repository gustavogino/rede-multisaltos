//esse codigo foi adaptado desse site aqui:
#include <pt.h>
#include <RF24Network.h>
#include <RF24.h>

RF24 radio(7, 8);               //Cria um objeto RF24 chamado radio, 7 e 8 são os pinos CE e CSN
RF24Network network(radio);      //Cria uma network e adiciona o objeto radio na mesma

const uint16_t self = 02; //Endereço deste nodo em octal
const uint16_t next_node = 00; //Endereço em octal do nodo que receberá a mensagem

const unsigned long intervalo = 2000; 
unsigned long last_sent; 

int indicador = 0;
unsigned long incomingData;
unsigned long incomingData1;
bool ok;

 RF24NetworkHeader headerOut(next_node); // (Endereço de envio)
 RF24NetworkHeader headerIn;

void setup() {
  Serial.begin(9600);
  radio.begin();
  network.begin(90, self);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void ouvindo_thread1(int interval) {
  static long next_time = 0; 
  if(millis()<next_time) return;
  next_time += interval;

  while ( network.available() ) { //Se exister algum dado para ser lido
    network.read(headerIn, &incomingData, sizeof(incomingData)); // Le o dado
  }
}

void enviando_thread2(int interval) {
  static long next_time = 0;
  if(millis()<next_time) return;
  next_time += interval;
  
 //Repassa o dado
    
     ok = network.write(headerOut, &incomingData, sizeof(incomingData));
}

void loop() {

network.update();
  if (millis() - last_sent >= intervalo) {
    ouvindo_thread1(1);
    enviando_thread2(1);
    Serial.print("Recebeu: " + String(incomingData));
    Serial.println("\tDo nodo: " + String(headerIn.from_node));
    if (ok) Serial.println("Envio para o nodo: " + String(headerOut.to_node));
    else Serial.println("Falhou em repassar para o nodo: " + String(headerOut.to_node));
    last_sent = millis(); //atualiza tempo do ultimo envio
  }
}
