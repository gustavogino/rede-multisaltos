//esse codigo foi adaptado desse site aqui:
//https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/
//os cara mandaram bem :)

#include <RF24Network.h>
#include <RF24.h>

RF24 radio(7, 8);               //Cria um objeto RF24 chamado radio, 7 e 8 são os pinos CE e CSN
RF24Network network(radio);      //Cria uma network e adiciona o objeto radio na mesma

const uint16_t self = 011;     //Endereço deste nodo em octal
const uint16_t next_node = 01; //Endereço em octal do nodo que receberá a mensagem

const unsigned long interval = 2000;  //Tempo em milissegundos entre envio de mensagens
unsigned long last_sent;             //Tempo do ultimo envio

void setup() {
  Serial.begin(9600);
  radio.begin();
  network.begin(90, self);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  Serial.println("Comeco");
}

void loop() {
  network.update();
  
  //===== Lendo o dado e enviando =====//
  if (millis() - last_sent >= interval) {   //Se passou do intervalo para enviar o dado
    bool ok = false;
    RF24NetworkHeader headerOut(next_node);   // (Endereço de envio)
    
    //Enquanto nao conseguir enviar o dado vai ficar tentando
    unsigned long valor;
    while (!ok) {
      valor = analogRead(A0); //leitura analogica da porta 0
      ok = network.write(headerOut, &valor, sizeof(valor)); // Envia o dado da leitura
    }
    
    Serial.print("Enviou: " + String(valor));
    Serial.println("\tPara o nó: " + String(headerOut.to_node));
    last_sent = millis(); //atualiza tempo do ultimo envio
  }
}
