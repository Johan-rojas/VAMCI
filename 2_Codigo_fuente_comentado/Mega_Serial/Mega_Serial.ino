#include <Ezo_i2c.h>                    //Incluir la libería EZO I2C desde  https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Wire.h>                        //Incluir la libería Wire para comunicar dispositivos por el bus I2C
#include <SoftwareSerial.h>              //Incluir la libería SoftwareSerial para comunicar dispositivos por el bus Serial.
 
int StateSensors = HIGH;  // Variable para ver el estado del pulsador
String Datos = "";        //Variable para concatenar las mediciones de los sensores

SoftwareSerial mySerial(19, 18); // RX, TX Pines para la comunicación serial del Arduino Mega

Ezo_board orp = Ezo_board(98, "ORP");   //crear un objeto de circuito Potencial Redox, cuya dirección es 98 y el nombre es "ORP"
Ezo_board od = Ezo_board(97, "DO");    //crear un objeto de circuito Oxígeno Disuelto, cuya dirección es 97 y el nombre es "DO"
Ezo_board ph = Ezo_board(99, "PH");   //crear un objeto de circuito PH, cuya dirección es 99 y el nombre es "PH"
Ezo_board ec = Ezo_board(100, "EC"); //crear un objeto de circuito Conductividad Eléctrica, cuya dirección 100 es y el nombre es "EC"
Ezo_board rtd = Ezo_board(102, "TEMP"); //crear un objeto de circuito Temperatura cuya dirección es 102 y el nombre es "TEMP"


void setup() {
  Wire.begin();                           //iniciar la comunicación I2C
  mySerial.begin(9600);                   // Comienzo de la comunicación serie
  Serial.begin(9600);                   //iniciar la comunicación serial a la computadora
}

void loop() {
  
  ph.send_read_cmd();                  //lee el valor del circuito PH
  rtd.send_read_cmd();                 //lee el valor del circuito temperatura o RTD
  ec.send_read_cmd();                  //lee el valor del circuito conductividad eléctrica o EC
  orp.send_read_cmd();                 //lee el valor del circuito potencial redox u ORP
  od.send_read_cmd();                  //lee el valor del circuito oxígeno disuelto u OD
  delay(2000);                          //el comando de lectura necesita 2 segundos para procesar y evitar solapamientos

  // Si el pulsador está presionado
    if (StateSensors == HIGH)
    {       
      Datos = receive_reading(rtd);                  //obtiene la lectura del circuito RTD
      Datos= "RTD/"+Datos+"\n";                      //concatena el valor de la lectura RTD a la varibale Datos
      Serial.print("Mensaje a enviar al LORA TX: "); //muestra en la consola de Arduino el valor de RTD a enviar al Lora transmisor
      Serial.println(Datos);                         //Imprime el valor que hay en Datos
      writeString(Datos);                            //Resuelve, agrega y quita los espacios de nombres en la variable Datos
      Datos="";                                      //Limpia la variable Datos
      
      Datos = receive_reading(orp);                  //obtiene la lectura del circuito ORP
      Datos= "ORP/"+Datos+"\n";                      //concatena el valor de la lectura ORP a la varibale Datos
      Serial.print("Mensaje a enviar al LORA TX: "); //muestra en la consola de Arduino el valor de ORP a enviar al Lora transmisor
      Serial.println(Datos);                         //Imprime el valor que hay en Datos
      writeString(Datos);                            //Resuelve, agrega y quita los espacios de nombres en la variable Datos
      Datos="";                                      //Limpia la variable Datos

      Datos = receive_reading(ec);                   //obtiene la lectura del circuito EC
      Datos= "EC/"+Datos+"\n";                       //concatena el valor de la lectura EC a la varibale Datos
      Serial.print("Mensaje a enviar al LORA TX: "); //muestra en la consola de Arduino el valor de EC a enviar al Lora transmisor
      Serial.println(Datos);                         //Imprime el valor que hay en Datos
      writeString(Datos);                            //Resuelve, agrega y quita los espacios de nombres en la variable Datos
      Datos="";                                      //Limpia la variable Datos
      
      Datos = receive_reading(od);                   //obtiene la lectura del circuito OD
      Datos= "OD/"+Datos+"\n";                       //concatena el valor de la lectura OD a la varibale Datos
      Serial.print("Mensaje a enviar al LORA TX: "); //muestra en la consola de Arduino el valor de OD a enviar al Lora transmisor
      Serial.println(Datos);                         //Imprime el valor que hay en Datos
      writeString(Datos);                            //Resuelve, agrega y quita los espacios de nombres en la variable Datos
      Datos="";                                      //Limpia la variable Datos

      Datos = receive_reading(ph);                    //obtiene la lectura del circuito PH
      Datos= "PH/"+Datos+"\n";                        //concatena el valor de la lectura PH a la varibale Datos
      Serial.print("Mensaje a enviar al LORA TX: ");  //muestra en la consola de Arduino el valor de PH a enviar al Lora transmisor
      Serial.println(Datos);                          //Imprime el valor que hay en Datos
      writeString(Datos);                             //Resuelve, agrega y quita los espacios de nombres en la variable Datos
      Datos="";                                       //Limpia la variable Datos                           
    }    
}

void writeString(String stringData) {                //Se utiliza para enviar en serie una cadena con Serial.write()
  for (int i = 0; i < stringData.length(); i++)
  {
    mySerial.write(stringData[i]);                  //Empuje cada carácter 1 por 1 en cada pase de bucle
    
    if(stringData[i]=='\n'){
      delay(2000);                                  //se necesitan 2 segundos para procesar y evitar solapamientos                          
    }    
  }

  delay(1000);                                      //se espera un segundo para enviar otra cadena de caracteres en serie
}

//Función para decodificar la lectura después de emitir el comando de lectura
String receive_reading(Ezo_board &Sensor) {

  String Data="";

  Serial.print(Sensor.get_name()); Serial.print(": ");  //imprimir el nombre del circuito obteniendo la lectura
  Sensor.receive_read_cmd();                            //obtener los datos de respuesta
  switch (Sensor.get_error()) {                         //cambie el caso según el código de respuesta.
    case Ezo_board::SUCCESS:
      Serial.println(Sensor.get_last_received_reading());  //el comando fue exitoso, imprima la lectura 
      Data=Sensor.get_last_received_reading();
      return Data;
      break;

    case Ezo_board::FAIL:
      Serial.print("Failed ");                          //significa que el comando ha fallado.
      break;

    case Ezo_board::NOT_READY:
      Serial.print("Pending ");                         //el comando aún no se ha terminado de medir.
      break;

    case Ezo_board::NO_DATA:
      Serial.print("No Data ");                         //El sensor no tiene datos para enviar
      break;
  }
}
