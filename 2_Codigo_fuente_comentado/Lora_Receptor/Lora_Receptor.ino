#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <PubSubClient.h> 
#include <WiFi.h>
#include <Firebase_ESP_Client.h> 
#include <HardwareSerial.h>

#define DEBUG(a, b) for (int index = 0; index < b; index++) Serial.print(a[index]); Serial.println();
#define rx 4 //16                          //Se define el pin 4 como RX en el Lora receptor.
#define tx 5//17                           //Se define el pin 5 como TX en el Lora receptor.       

HardwareSerial serial1(1);                 //Definir cómo va a funcionar el puerto serie.

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
#define BUFFER_SIZE                                 80 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

int16_t txNumber;

int16_t rssi,rxSize;

bool lora_idle = true;


                                             
void setup() {
    Serial.begin(9600);
    //mySerial.begin(9600); 
    serial1.begin(9600, SERIAL_8N1,rx,tx);
    Mcu.begin();
    
    txNumber=0;
    rssi=0;
  
    RadioEvents.RxDone = OnRxDone;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
}

void loop()
{
  if(lora_idle)
  {
    lora_idle = false;
    Serial.println("into RX mode");
    Radio.Rx(0);
  }
  //delay(1000);
  Radio.IrqProcess( );
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    Radio.Sleep( );
    Serial.print(rxpacket);
    writeString(rxpacket);
    delay(1000);  
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,rssi,rxSize);
    lora_idle = true;
}

void writeString(String stringData) {                                  //Se utiliza para enviar en serie una cadena con Serial.write()

    for (int i = 0; i < stringData.length(); i++)
    {
      serial1.write(stringData[i]);                                   //Se envía por puerto serial cada caracter hasta completar la longitud 
      Serial.print(stringData[i]);
      Serial.print("bandera");
    }                                                                  //de cada paquete a enviar al NodeMCU


}
