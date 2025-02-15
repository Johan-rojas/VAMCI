#include "LoRaWan_APP.h"
#include "Arduino.h"

#include <Ezo_i2c.h>                    //Incluir la libería EZO I2C desde  https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>                       //Incluir la libería Wire para comunicar dispositivos por el bus I2C
#include "stdlib.h"                     //Contiene los prototipos de funciones de C para gestión de memoria dinámica, control de procesos y otras.
//#include <SoftwareSerial.h>             //Incluir la libería SoftwareSerial para comunicar dispositivos por el bus Serial.
#include <HardwareSerial.h>

#define DEBUG(a, b) for (int index = 0; index < b; index++) Serial.print(a[index]); Serial.println(); //Se usa para detectar errores en la compilación, es una alerta preventiva.
#define rx 6 //16                          //Se define el pin 16 como RX en el Lora transmisor.
#define tx 7//17                           //Se define el pin 17 como TX en el Lora transmisor.
//SoftwareSerial seri(rx, tx);        //Definir cómo va a funcionar el puerto serie.

HardwareSerial serial1(1);

char inChar;                            //Se define la variable inChar para leer los caracteres que vienen por el puerto serial desde el Mega.                       
String string="";                       //Se define la cadena de caracteres string donde se almacenan los datos que se reciben desde el Mega.

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             5        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 80//Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

double txNumber;

bool lora_idle=true;

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );

void setup() {  
    //seri.begin(9600);
    serial1.begin(9600, SWSERIAL_8N1,rx,tx);
    Serial.begin(9600);
    Mcu.begin();
    
    //string.reserve(200);
    
    txNumber=0;

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 
   }



void loop()
{
 
  if(lora_idle == true)
  {
    if (serial1.available()){                        //Si el puerto serial esta disponible para enviar y/o recibir datos entonces:
      inChar = serial1.read();                       //Se procede con la lectura de caracteres   
  
      if(inChar!='\n'){                               //Si el caracter leído es diferente de \n:
         string+=inChar;                              //Suma de caracteres en variable string
         Serial.println(string); 
      }
      
      Serial.println(string); 
      if(inChar=='\n'){                               //Si el caracter leído es igual a \n:
        string+=inChar;                               //Suma de caracteres en variable string    
        Serial.println("Lectura sensores: ");         //Imprime la frase, Lectura de sensores:  
        Serial.println(string);  
        sprintf(txpacket,"%s",string);  //start a package
        Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));
        Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out 
        string="";
        lora_idle = false;
              
      }     
    }    
  }
  Radio.IrqProcess( );
}

void OnTxDone( void )
{
  Serial.println("TX done......");
  lora_idle = true;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    lora_idle = true;
}
