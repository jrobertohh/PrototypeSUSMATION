#include <Time.h>
#include <TimeLib.h>

//Duración

//Datos para ESP
#include <SPI.h>
#include <ESP8266WiFi.h>
byte ledPin = 2;
char ssid[] = "iPhone (2)";           // SSID of your home WiFi   "e84d76"
char pass[] = "0qgiwz43";            // password of your home WiFi   "272627885"
const char* host = "jrobertohhresearch.tech";  //Dirección IPv4 */
//unsigned long askTimer = 0;
IPAddress server(192,168,0,80);       // the fix IP address of the server
WiFiClient client;

//Datos sensor HUMEDAD
int dryValue = 0;
int wetValue = 1024;
int friendlyDryValue = 0;
int friendlyWetValue = 100;
int RELE = D2;

void setup(){

  Serial.begin(115200);
  pinMode(RELE, OUTPUT);

  //Datos para ESP
  WiFi.begin(ssid, pass);             // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  pinMode(ledPin, OUTPUT);

  //Time
  setTime(19,00,00,31,5,2019); // Las 19:00:00 del dia 31 de May de 2019

}

void loop(){

  int HUM = analogRead(A0);
  Serial.print("Valor del sensor CALIBRADO:");
  int friendlyValue = map (HUM, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
  Serial.println(friendlyValue);
  if(friendlyValue < 25){  //Máximo de humedad 860
      int minutos = 0;
      while(minutos < 19 ){
           HUM = analogRead(A0);
           Serial.print("Valor del sensor CALIBRADO:");
           friendlyValue = map (HUM, dryValue, wetValue, friendlyDryValue, friendlyWetValue);
           Serial.println(friendlyValue);
           time_t t = now();
           Serial.print("Valor del sensor de humedad con bomba:");
           Serial.println(HUM);
           digitalWrite(RELE, HIGH);  
           delay(2000); 
           Serial.print(day(t));
           Serial.print(+ "/") ;
           Serial.print(month(t));
           Serial.print(+ "/") ;
           Serial.print(year(t)); 
           Serial.print( " ") ;
           Serial.print(hour(t));  
           Serial.print(+ ":") ;
           Serial.print(minute(t));
           Serial.print(":") ;
           Serial.println(second(t));
           int minutos =  minute(t);
           Serial.print("Minutos :") ;
           Serial.println(minutos);    
      }   
      //while(analogRead(A0) < 300){
        //    int valorActual = analogRead(A0);
      //}
      Serial.println(analogRead(A0));
      client.connect(server, 80);   // Connection to the server
      //Envío de datos
      int lecturaNODO = 3;
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
       //String uidUsuario = String(Usuario[1]) + String(Usuario[2]) + String(Usuario[3]); 
       String url = "/drole/guardar4.php";
       //String dato1 = "?uidUsuario=";
       String dato2 = "?nodo=";
       String nodo="003";
       Serial.print("Requesting URL: ");
       Serial.println(url);
       //Insertar registro en BD
       client.print(String("GET ") + url + dato2 + nodo + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
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
         setTime(19,00,00,31,5,2019);     
  }
  else{ //if(friendlyValue >= 600){
       digitalWrite(RELE, LOW);
       HUM = analogRead(A0);
       Serial.print("Valor del sensor de humedad sin bomba:");
       Serial.println(HUM);
       delay(100);
  }
}
