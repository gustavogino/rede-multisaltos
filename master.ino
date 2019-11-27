//esse codigo foi adaptado desse site aqui:
//https://howtomechatronics.com/tutorials/arduino/how-to-build-an-arduino-wireless-network-with-multiple-nrf24l01-modules/


#include <RF24Network.h>
#include <RF24.h>

RF24 radio(7, 8);               //Cria um objeto RF24 chamado radio, 7 e 8 são os pinos CE e CSN
RF24Network network(radio);      //Cria uma network e adiciona o objeto radio na mesma

const uint16_t self = 00; //Endereço deste nodo em octal

//  Esses endereço de filho talvez nao sejam necessarios para este trabalho
//  pelo menos para o mestre
//const uint16_t children[] = {01}; //Endereço dos nodos filhos
//const uint8_t nChildren = 1; //Número de filhos

void setup() {
  Serial.begin(9600);
  radio.begin();
  network.begin(90, self);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
}

void loop() {
  network.update();
  
  //===== Recebendo =====//
  while ( network.available() ) { //Se exister algum dado para ser lido
    RF24NetworkHeader headerIn;
    unsigned long incomingData;
    network.read(headerIn, &incomingData, sizeof(incomingData)); // Le o dado
    //nao precisa manda ACK porque a biblioteca ja faz isso. Depois eu que so loco

    //imprime mensagem no monitor serial
    Serial.print("Mensagem: " + incomingData);
    Serial.println("\tRecebida do nodo: " + headerIn.from_node);
  }
}
