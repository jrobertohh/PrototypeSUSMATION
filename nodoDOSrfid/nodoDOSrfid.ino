#include <Servo.h>
Servo servoMotor;

//Relé
int RELE = 3;

//Sensor
const int sensor = 2;

//Datos para MFRC522
#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 5   //
#define SS_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);

//Datos para ESP
//#include <SPI.h>
#include <ESP8266WiFi.h>
byte ledPin = 2;
char ssid[] = "iPhone (2)";           // SSID of your home WiFi
char pass[] = "0qgiwz43";            // password of your home WiFi
const char* host = "jrobertohhresearch.tech";  //Dirección IPv4 */
//unsigned long askTimer = 0;
IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

void setup() {
  Serial.begin(115200);   

  pinMode(ledPin, OUTPUT);
  
  //Datos para MFRC522
  while(!Serial);       //No hacer nada si el puerto serial no está disponible 
  SPI.begin();          // Iniciar el bus SPI
  mfrc522.PCD_Init();   // Iniciar MFRC522
  //mfrc522.PCD_DumpVersionToSerial();  // Mostrar los detalles de la tarjeta
  //Serial.println(F("Buscar PICC para ver la UID, SAK, typo, y otros datos de bloque..."));

  //Datos para ESP
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  servoMotor.attach(16);
  servoMotor.write(65);
  pinMode(RELE, OUTPUT);
  pinMode (sensor , INPUT);
}

byte ActualUID[4]; //Almacenar TAG leído
byte Usuario[4]={0x63, 0xC4, 0xA0, 0xD3};
//Card UID: 71, 09, 59, 08
int cont=0;
void loop() {

  
  //Serial.print("Sensor:");
  //Serial.println(digitalRead( sensor));
  if (digitalRead( sensor)) {cont++; delay(1000);//Serial.print("Cont:");Serial.println(cont);
  }
  if(cont == 5) {digitalWrite(RELE, HIGH); delay(3000); cont = 0;} else digitalWrite(RELE, LOW);
  
       
  
  //Serial.println("Prueba1");
  Serial.println(mfrc522.PICC_IsNewCardPresent());
  delay(1000);
  if ( mfrc522.PICC_IsNewCardPresent()) {  
      //Seleccionamos una tarjeta
            //Serial.println("Prueba2");
            if ( mfrc522.PICC_ReadCardSerial()) {
                  // Enviamos serialemente su UID
                  Serial.print(F("Card UID:"));
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
                  //Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  //Serial.println("Prueba3");
                  client.connect(server, 80);   // Connection to the server

                  if(compareArray(ActualUID,Usuario)){
                    Serial.println("Acceso concedido...");
                      /*else if(compareArray(ActualUID,Usuario2))
                        Serial.println("Acceso concedido...");*/
                        // We now create a URL for the request
                        //int gradoLuz = String(answer).toInt();
                        //Envío de datos
                        int lecturaNODO = 2;
                        digitalWrite(ledPin, LOW);
                        Serial.println("********************************");
                        Serial.print("Byte sent to the AP: ");
                        Serial.println(client.println(lecturaNODO));
                        String answer = client.readStringUntil('\r');
                        Serial.println("From the AP: " + answer);
                        client.flush();
                        digitalWrite(ledPin, HIGH);

                        //const client = client.available();
                        WiFiClient client;
                        const int httpPort = 80;
                        if (!client.connect(host, httpPort)) {
                              Serial.println("connection failed");
                              return;
                        }
                        String uidUsuario = String(Usuario[1]) + String(Usuario[2]) + String(Usuario[3]); 
                        String url = "/drole/guardar3.php";
                        String dato1 = "?uidUsuario=";
                        String dato2 = "&nodo=";
                        String nodo="002";
                        Serial.print("Requesting URL: ");
                        Serial.println(url);
                        //Insertar registro en BD
                        client.print(String("GET ") + url + dato1 + uidUsuario + dato2 + nodo + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
                        unsigned long timeout = millis();
                        while (client.available() == 0) { //Devuelve la cantidad de datos que se han escrito por el cliente para el servidor al que está conectado
                            if (millis() - timeout > 5000) {
                                Serial.println(">>> Client Timeout !");
                                client.stop();
                                return;
                            }
                        }
                        // Read all the lines of the reply from server and print them to Serial
                        while (client.available()) {
                                String line = client.readStringUntil('\r'); //El carácter de retorno de carro
                                Serial.print(line);
                        }
                        Serial.println();
                        Serial.println("closing connection");
                        //delay(2000);
                  
                  for(int i=65; i<120; i+=5){
      servoMotor.write(i);
      delay(100);
  }
  //Serial.print("repetición");
  delay(5000);
  for(int i=120; i>65; i-=5){
      servoMotor.write(i);
      delay(100);
  }
    delay(5000);
                  }
                  else
                    Serial.println("Acceso denegado...");
                  
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();

                  
          
            }
      
  }
  
  
}
//Función para comparar dos vectores
 boolean compareArray(byte array1[],byte array2[])
{
  for(byte i = 0; i < mfrc522.uid.size; i++){
      if(array1[i] != array2[i])return(false);
    }
  
  /*if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);*/
}
