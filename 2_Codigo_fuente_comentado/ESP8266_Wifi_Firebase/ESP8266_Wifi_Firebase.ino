#include <SoftwareSerial.h>                            //Incluir la libería SoftwareSerial para comunicar dispositivos por el bus Serial.
#include "stdlib.h"                                    //Contiene los prototipos de funciones de C para gestión de memoria dinámica, control de procesos y otras.
#include <Firebase_ESP_Client.h>                       //Esta librería permite utilizar interfaces de red de clientes Arduino externos, p. WiFiClient, EthernetClient y GSMClient.
#include <Arduino.h>                                   //Esta librería da acceso a los tipos y constantes estándar del lenguaje de Arduino.
#include <NTPClient.h>                                 //Esta librería sirve para conectarse a un servidor de tiempo NTP. Obtenga la hora de un servidor de hora NTP y manténgala sincronizada.
#include <WiFiUdp.h>                                   //Esta librería crea una instancia con nombre de la clase WiFi UDP que puede enviar y recibir mensajes UDP. 
#include "addons/TokenHelper.h"                        //Esta librería sirve para que proporcione la información del proceso de generación del token.
#include "addons/RTDBHelper.h"                         //Esta libería sirve para que proporcione la información de impresión de la carga útil de RTDB y otras funciones auxiliares.

#define WIFI_SSID "RESERVA SANGUARE"    //"Cristian"            //Inserte el nombre de la red a la que se quiere conectar el NodeMCU.
#define WIFI_PASSWORD "sanguare01" //"crisqui..."          //Inserte la contraseña de la red a la que se se quiere conectar el NodeMCU.
#define API_KEY "AIzaSyDM2jzBC5G-ViPBpYzHCNnj3UlYsFwRC9c"  //Insertar la clave API del proyecto de Firebase
#define DATABASE_URL "https://simevam-ii-default-rtdb.firebaseio.com/"   //Inserte la URL de la base de datos en tiempo real en Firebase, donde se publicarán los datos.

#define DEBUG(a, b) for (int index = 0; index < b; index++) Serial.print(a[index]); Serial.println();   //Se usa para detectar errores en la compilación, es una alerta preventiva.
#define rx 13                                          //Definir cuál pin RX va a ser, en este caso es el pin 13 del NodeMCU
#define tx 15                                          //Definir cuál pin TX va a ser, en este casi es el pin 15 del NodeMCU

SoftwareSerial mySerial(rx, tx);                      //Definir cómo va a funcionar el puerto serie.

FirebaseData fbdo;                                    //Definir objeto de datos de Firebase.
FirebaseAuth auth;                                    //Definir objeto de autenticación de Firebase.
FirebaseConfig config;                                //Definir objeto de configuración de Firebase.

//Declaracion de variables
char inChar;                                          //Se define la variable inChar para leer los caracteres que vienen por el puerto serial desde el Lora TX.
String string="";                                     //Se define la cadena de caracteres string donde se almacenan los datos que se reciben desde el Llora TX.
bool signupOK = false;                                //Se define la bandera signupOK para comprobar que llegó el dato y esta listo para publicarse en Firebase.

void setup() {
  Serial.begin(9600);                                 //Se configura la comunicación serial con frecuencia de trasnmisisón de 9600.
  mySerial.begin(9600);
  //string.reserve(200);                                //Se reservan 200 espacios de memoria en la variable string para transmitir los datos hasta Firebase.
  pinMode(LED_BUILTIN, OUTPUT);                       //Inicializar el pin LED_BUILTIN como salida

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);               //Comienza la conexión a la red WiFi, comprobando que las credenciales ingresadas sean correctas.
  Serial.print("Connecting to Wi-Fi");                //Se imprime en la consola el mensaje Conectando a WiFi.
  while (WiFi.status() != WL_CONNECTED){              //Mientras el estado de la conexión a WiFi sea diferente a conectado, se imprime un punto y se esperan 3 segundos
    Serial.print(".");                                //hasta que se conecte.
    delay(300);
  }
  Serial.println();                                   //Si la conexión es esxitosa se muestra en la consola el mensaje Conectado con la IP del Router al que se accesó.
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());                     
  Serial.println();

  config.api_key = API_KEY;                           //Asignar la clave api (obligatorio).
  config.database_url = DATABASE_URL;                 //Asignar la URL de la base de datos en tiempo real (RTDB) (obligatorio).
  
  if (Firebase.signUp(&config, &auth, "", "")){       //Inscribirse con la configuración previa para accesar a la base de datos.
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());  //Si las validaciones fallan, se imprime un mensaje con el error.
  }

  config.token_status_callback = tokenStatusCallback;  //Asignar la función de devolución de llamada para la tarea de generación de tokens de ejecución prolongada
                                                       //ver addons/TokenHelper.h    
  Firebase.begin(&config, &auth);                      //Si todo está correcto, se inicia la publicación en Firebase.                 
  Firebase.reconnectWiFi(true);                        //Solo si hay una conexión a internet estable se publican los datos.
}

String DatoString = "";                                //Se declara la variable Dato de tipo string.
String SensorPath = "";                                //Se declara la varianle SensorPath tipo string.

void loop() {                                          //La función de bucle se ejecuta una y otra vez para siempre.
  
  if (mySerial.available()){                           //Si el puerto serial esta disponible.
    inChar = mySerial.read();                          //Se procede con la lectura de caracteres   


    if(inChar!='\n' && inChar!='/'){
       string+=inChar;                                 //Suma de caracteres en variable string, es decir que se concatenan.
    }

    if(inChar=='/'){                                   //Si la variable inChar es igual al caracater /, entonces se comienza a publicar los datos el Firebase.


      if(string=="RTD"){                                                    //Si String es igual a RTD, se publica en Firebase el valor que registró el sensor de RTD.
        SensorPath="App_SIREMEVAM/Sux6U6NwAAWIy451JcwK6d0Bxhj2/RTD";       
      }else if(string=="ORP"){                                              //Si String es igual a ORP, se publica en Firebase el valor que registró el sensor de ORP.
        SensorPath="App_SIREMEVAM/Sux6U6NwAAWIy451JcwK6d0Bxhj2/ORP";
      }else if(string=="EC"){                                               //Si String es igual a EC, se publica en Firebase el valor que registró el sensor de EC.
        SensorPath="App_SIREMEVAM/Sux6U6NwAAWIy451JcwK6d0Bxhj2/EC";  
      }else if(string=="OD"){                                               //Si String es igual a OD, se publica en Firebase el valor que registró el sensor de OD.           
        SensorPath="App_SIREMEVAM/Sux6U6NwAAWIy451JcwK6d0Bxhj2/OD";
      }else if(string=="PH"){                                               //Si String es igual a PH, se publica en Firebase el valor que registró el sensor de PH.
        SensorPath="App_SIREMEVAM/Sux6U6NwAAWIy451JcwK6d0Bxhj2/PH";
      }     

      string="";                                                           //Se limpia la varibale String.
         
    }        
   
    if (inChar=='\n'){                                                     //Si inChar es igual al caracter \n, se muestra el letrero Dato recibido del LORA punto B en 
      Serial.print("Dato recibido del LORA punto B: ");                    //la consola y se imprime "->" junto el valor del dato recibido correspodiente.
      Serial.print("->");                                          
      Serial.println(string);           
      //DatoFloat=string.toFloat();


      if (Firebase.ready() && signupOK){                                   //Si las validaciones de Firebase son correctas y se encuentra disponible para publicar
            if(Firebase.RTDB.setString(&fbdo, SensorPath, string)){        //y si String es igual a RTD, se publica en Firebase el valor que registró el sensor de RTD.
              Serial.println("PASSED");                                    //y de igual manera con los demas sensores y se imprime "PASSED" cuando ya se publicó.
            }
            else {
              Serial.println("FAILED");                                    //Si la variable String no es igual a cualquiera de las etiquetas que se asignaron a los sensores
              Serial.println("REASON: " + fbdo.errorReason());             //no se publica el dato y se imprime en consola la palabra "FAILED".
            }
        } 
        
      string="";                                                          //Borra la variable string para almacenar nuevos datos    
    } 
   }       
 }                                                                       //Espere dos segundos (para demostrar el LED bajo activo)
