//@author Artur Pakuła

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

#define SLAVE_ADDRESS 0x30
#define NR_OF_READERS   5
#define RST_PIN         9
#define SS_PIN_1        10
#define SS_PIN_2        8
#define SS_PIN_3        7
#define SS_PIN_4        6
#define SS_PIN_5        5


MFRC522 mfrc522[NR_OF_READERS];
byte ssPins[] = {SS_PIN_1, SS_PIN_2,  SS_PIN_3, SS_PIN_4, SS_PIN_5};
unsigned long cards[] = {404,404,404,404,404};
unsigned long sent[] = {404,404,404,404,404};
String toSend = "404";
byte keyToSend = 0;
byte lastSentPos = 0;
byte taskId = 70;
byte cR = 5;

unsigned long getID(byte currentReader) {
  unsigned long hex_num;
  hex_num =  mfrc522[currentReader].uid.uidByte[0] << 24;
  hex_num += mfrc522[currentReader].uid.uidByte[1] << 16;
  hex_num += mfrc522[currentReader].uid.uidByte[2] <<  8;
  hex_num += mfrc522[currentReader].uid.uidByte[3];
  return hex_num;
}

unsigned long readLong(byte currentReader, byte count = 100) {
  unsigned long card = 404;
  for (int i = 0; i < count; i++) {
    if ( ! mfrc522[currentReader].PICC_IsNewCardPresent()) {
      //Serial.print("brak karty");
      continue;
    }
    if ( ! mfrc522[currentReader].PICC_ReadCardSerial()) {
      //Serial.print("błąd odczytu karty");
      continue;
    }
    card = getID(currentReader);
    if (card != 404) {
      break;
    }
  }
  return card;
}

void setup() {
  SPI.begin();
  for (int reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
  
  //Serial.begin(9600); 
  //while (!Serial);
  
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveEvent);      
  Wire.onRequest(requestEvent);
  
}

void loop() {
    for(byte key = 0; key < NR_OF_READERS; key++) {
      delay(50);
      unsigned long x = readLong(key,5);
      if(x != cards[key]){
        //Serial.println(x);
      }
      cards[key] = x;
    }
}


void trigger(byte task, unsigned long arr[]){
  byte response = 99;
  
  if(task == 70){
      response = cR;
      taskId = 72;
  }else if(task == 72){
      response = 22;
    
      for(byte key = 0; key < cR; key++) {  
        if(sent[key] != arr[key]){
          response = 20;
          toSend = String(arr[key]);
          keyToSend = key;
          taskId = 74;
          lastSentPos = 0;
          break;
        }
      }
  }else if(task == 74){
        response = keyToSend;
        taskId = 76;
  }else if(task == 76){
        response = toSend.length();
        taskId = 78;
  }else if (task == 78){
      response = toSend[lastSentPos] - '0';
      lastSentPos++;
      if(lastSentPos == toSend.length()){
        taskId = 72;
        sent[keyToSend] = cards[keyToSend];
      }
  }
//  Serial.print("task:");
//  Serial.print(task);
//  Serial.print(", send:");
//  Serial.println(response);
  Wire.write(response);
  
}

void requestEvent() {
  trigger(taskId,cards);
}

void receiveEvent(byte byteCount) {
  while (Wire.available() && byteCount-- >0) {
    taskId = Wire.read();
  }
  if(taskId == 68){
      for(byte key = 0; key < cR; key++) {  
        sent[key] = 404;
      }
  }
}
