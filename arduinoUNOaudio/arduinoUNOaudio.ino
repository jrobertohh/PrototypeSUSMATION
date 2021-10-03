#include <DHT.h>
#include "VoiceRecognitionV3.h"
#include <Adafruit_Sensor.h> 

const byte arduino_8 = 8;
#include <Wire.h>

//AUDIO LIBRERIAS
#include <SdFatConfig.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SPI.h>   //Biblioteca para la comunicación vía SPI.
#include <SdFat.h>        // Biblioteca para acceder a la tarjeta SD (necesaria para la biblioteca vs1053_SdFat)
#include <vs1053_SdFat.h> // Biblioteca para reproducir archivos MP3

//AUDIO VARIABLES
SdFat sd;         //Generar el objeto para la tarjeta SD
vs1053 MP3player; //Crear el objeto reproductor multimedia

//Sensor de temperatura y humedad
int SENSORTH = 5;   //Cambió de 2 a 5
//int temp, humedad;
DHT dht (SENSORTH, DHT11);

//FLAMA
//int sensorFlama = 10;

//Reconocimiento de VOZ
VR myVR(3,4);    // 3/13:RX 2/12:TX, you can choose your favourite pins.
uint8_t records[7]; // save record
uint8_t buf[64];
//int led = 13;
#define Tiempo    (0)
//#define Si   (1) 
//#define No   (2)
//#define Musica    (3)
//#define Encender   (4)
//#define Apagar    (5)
//#define Fin     (6)
void printSignature(uint8_t *buf, int len){
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}
void printVR(uint8_t *buf){
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");
  Serial.print(buf[2], DEC);
  Serial.print("\t\t");
  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");
  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup() {
 //Wire.begin(arduino_8);                /* join i2c bus with address 8 */
 
 Serial.begin(115200);           /* start serial for debug */

 //AUDIO_SETUP
  // Iniciar la comunicación con la tarjeta SD. Si ocurre un error, detente aquí
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED))
    sd.initErrorHalt();
  // Cambiar al directorio raíz de la tarjeta SD. Si ocurre un error, se muestra el mensaje de error "sd.chdir".
  if (!sd.chdir("/"))
      sd.errorHalt("sd.chdir");
  //MP3 Shield starten
  MP3player.begin();
  // Establece el volumen. El primer valor es para el canal de audio izquierdo. El segundo para el canal de audio correcto
  //MP3player.setVolume(15,15); //Ajustar el volumen
  //MP3player.playTrack(1);

  //Sensor de Temperatura y humedad
  dht.begin();

  //Reconocimiento de VOZ
  myVR.begin(9600);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  //pinMode(led, OUTPUT);
  //digitalWrite(led, LOW);
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  if(myVR.load((uint8_t)Tiempo) >= 0){
    Serial.println("Comando Tiempo leído");
  }
  /*if(myVR.load((uint8_t)Si) >= 0){
    Serial.println("Comando Si  leído");
  }
  //Agregado
  if(myVR.load((uint8_t)No) >= 0){
    Serial.println("Comando No leído");
  }
  if(myVR.load((uint8_t)Musica) >= 0){
    Serial.println("Comando Musica leído");
  }
  if(myVR.load((uint8_t)Encender) >= 0){
    Serial.println("Comando Encender leído");
  }
  if(myVR.load((uint8_t)Apagar) >= 0){
    Serial.println("Comando Apagar leído");
  }
  if(myVR.load((uint8_t)Fin) >= 0){
    Serial.println("Comando Fin leído");
  }*/

  //FLAMA
  //pinMode(sensorFlama, INPUT);
}

void loop() {
 delay(100);

/*   MP3player.setVolume(15,15); //Ajustar el volumen
  MP3player.playTrack(1);
MP3player.setVolume(15,15); //Ajustar el volumen
  MP3player.playTrack(2);
  MP3player.setVolume(15,15); //Ajustar el volumen
  MP3player.playTrack(3);
  MP3player.setVolume(15,15); //Ajustar el volumen
  MP3player.playTrack(4);
  MP3player.setVolume(15,15); //Ajustar el volumen
  MP3player.playTrack(5);
*/
 
 // Inicia el temporizador para reproducir las canciones.
  MP3player.available();
 //Wire.beginTransmission(OtherAddress); 
 Wire.begin(arduino_8); 
 Wire.onReceive(receiveEvent); /* register receive event */
 Wire.onRequest(requestEvent); /* register request event */
 Wire.endTransmission();

 //Sensor de TEMPERATURA
 float humedad = dht.readHumidity();
 float temp    = dht.readTemperature();
 //temp = temp - 3;
        //Serial.println(temp);
//Reconocimiento de VOZ
 int ret;
 ret = myVR.recognize(buf, 50);
 //Serial.println(buf[1]);
 if(ret>0){
    switch(buf[1]){
      case Tiempo: 
 if(temp < 0) { MP3player.setVolume(15,15);MP3player.playTrack(4);}
 else if(temp >= 0 && temp < 1 ){MP3player.setVolume(15,15);MP3player.playTrack(5);}
 else if(temp >= 1 && temp < 2 ){MP3player.setVolume(15,15);MP3player.playTrack(6);}
 else if(temp >= 2 && temp < 3 ){MP3player.setVolume(15,15);MP3player.playTrack(7);}
 else if(temp >= 3 && temp < 4 ){MP3player.setVolume(15,15);MP3player.playTrack(8);}
 else if(temp >= 4 && temp < 5 ){MP3player.setVolume(15,15);MP3player.playTrack(9);}
 else if(temp >= 5 && temp < 6 ){MP3player.setVolume(15,15);MP3player.playTrack(10);}
 else if(temp >= 6 && temp < 7 ){MP3player.setVolume(15,15);MP3player.playTrack(11);}
 else if(temp >= 7 && temp < 8 ){MP3player.setVolume(15,15);MP3player.playTrack(12);}
 else if(temp >= 8 && temp < 9 ){MP3player.setVolume(15,15);MP3player.playTrack(13);}
 else if(temp >= 9 && temp < 10 ){MP3player.setVolume(15,15);MP3player.playTrack(14);}
 else if(temp >= 10 && temp < 11 ){MP3player.setVolume(15,15);MP3player.playTrack(15);}
 else if(temp >= 11 && temp < 12 ){MP3player.setVolume(15,15);MP3player.playTrack(16);}
 else if(temp >= 12 && temp < 13 ){MP3player.setVolume(15,15);MP3player.playTrack(17);}
 else if(temp >= 13 && temp < 14 ){MP3player.setVolume(15,15);MP3player.playTrack(18);}
 else if(temp >= 14 && temp < 15 ){MP3player.setVolume(15,15);MP3player.playTrack(19);}
 else if(temp >= 15 && temp < 16 ){MP3player.setVolume(15,15);MP3player.playTrack(20);}
 else if(temp >= 16 && temp < 17 ){MP3player.setVolume(15,15);MP3player.playTrack(21);}
 else if(temp >= 17 && temp < 18 ){MP3player.setVolume(15,15);MP3player.playTrack(22);}
 else if(temp >= 18 && temp < 19 ){MP3player.setVolume(15,15);MP3player.playTrack(23);}
 else if(temp >= 19 && temp < 20){MP3player.setVolume(15,15);MP3player.playTrack(24);}
 else if(temp >= 20 && temp < 21 ){MP3player.setVolume(15,15);MP3player.playTrack(25);}
 else if(temp >= 21 && temp < 22 ){MP3player.setVolume(15,15);MP3player.playTrack(26);}
 else if(temp >= 22 && temp < 23 ){MP3player.setVolume(15,15);MP3player.playTrack(27);}
 else if(temp >= 23 && temp < 24 ){MP3player.setVolume(15,15);MP3player.playTrack(28);}
 else if(temp >= 24 && temp < 25 ){MP3player.setVolume(15,15);MP3player.playTrack(29);}
 else if(temp >= 25 && temp < 26 ){MP3player.setVolume(15,15);MP3player.playTrack(30);}
 else if(temp >= 26 && temp < 27 ){MP3player.setVolume(15,15);MP3player.playTrack(31);}
 else if(temp >= 27 && temp < 28 ){MP3player.setVolume(15,15);MP3player.playTrack(32);}
 else if(temp >= 28 && temp < 29 ){MP3player.setVolume(15,15);MP3player.playTrack(33);}
 else if(temp >= 29 && temp < 30 ){MP3player.setVolume(15,15);MP3player.playTrack(34);}
 else if(temp >= 30 && temp < 31 ){MP3player.setVolume(15,15);MP3player.playTrack(35);}
 else if(temp >= 31 && temp < 32 ){MP3player.setVolume(15,15);MP3player.playTrack(36);}
 else if(temp >= 32 && temp < 33 ){MP3player.setVolume(15,15);MP3player.playTrack(37);}
 else if(temp >= 33 && temp < 34 ){MP3player.setVolume(15,15);MP3player.playTrack(38);}
 else if(temp >= 34 && temp < 35 ){MP3player.setVolume(15,15);MP3player.playTrack(39);}
 else if(temp >= 35 && temp < 36 ){MP3player.setVolume(15,15);MP3player.playTrack(40);}
 else if(temp >= 36 && temp < 37 ){MP3player.setVolume(15,15);MP3player.playTrack(41);}
 else if(temp >= 37 && temp < 38 ){MP3player.setVolume(15,15);MP3player.playTrack(42);}
 else if(temp >= 38 && temp < 39 ){MP3player.setVolume(15,15);MP3player.playTrack(43);}
 else if(temp >= 39 && temp < 40 ){MP3player.setVolume(15,15);MP3player.playTrack(44);}
 else if(temp >= 40 && temp < 41 ){MP3player.setVolume(15,15);MP3player.playTrack(45);}
 else if(temp >= 41 && temp < 42 ){MP3player.setVolume(15,15);MP3player.playTrack(46);}
 else if(temp >= 42 && temp < 43 ){MP3player.setVolume(15,15);MP3player.playTrack(47);}
 else if(temp >= 43 && temp < 44 ){MP3player.setVolume(15,15);MP3player.playTrack(48);}
 else if(temp >= 44 && temp < 45 ){MP3player.setVolume(15,15);MP3player.playTrack(49);}
 else if(temp >= 45 && temp < 46 ){MP3player.setVolume(15,15);MP3player.playTrack(50);}
 else if(temp >= 46 && temp < 47 ){MP3player.setVolume(15,15);MP3player.playTrack(51);}
 else if(temp >= 47 && temp < 48 ){MP3player.setVolume(15,15);MP3player.playTrack(52);}
 else if(temp >= 48 && temp < 49 ){MP3player.setVolume(15,15);MP3player.playTrack(53);}
 else if(temp >= 49 && temp < 50 ){MP3player.setVolume(15,15);MP3player.playTrack(54);}
 else if(temp >= 50 && temp < 51 ){MP3player.setVolume(15,15);MP3player.playTrack(55);}
 else if(temp > 50){MP3player.setVolume(15,15);MP3player.playTrack(56);}
 break;
 }
    printVR(buf);
  }
 delay(3000);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
 Serial.println(Wire.available());
 while (0 <Wire.available()) {
    int c = Wire.read();      /* receive byte as a character */
    if(c == 2){
        MP3player.setVolume(15,15);
        MP3player.playTrack(1);
        delay(5000);
      }
      else if(c == 3){
        MP3player.setVolume(15,15);
        MP3player.playTrack(58);
        delay(5000);
      }
     else if(c == 4){
        
        MP3player.setVolume(15,15);
        MP3player.playTrack(2);
        delay(5000);
      }
    Serial.println(c);           /* print the character */
  }
 Serial.println();             /* to newline */

 //FLAMA
 
 
}

// function that executes whenever data is requested from master
void requestEvent() {
 Wire.write("Hello NodeMCU");  /*send string on request */
}
