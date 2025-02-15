# VAMCI - Variables Ambientales para Ciénagas

## 🌍 Descripción del Proyecto
**VAMCI (Variables Ambientales para Ciénagas)** es un sistema automatizado para la medición de variables acuáticas como **pH, oxígeno disuelto, temperatura y conductividad eléctrica** en la Ciénaga de Ayapel, Córdoba. Su propósito es facilitar la recopilación y visualización de estos datos en tiempo real mediante sensores, transmisión LoRa y almacenamiento en la nube.

Este sistema permite reducir errores en mediciones manuales y puede ser escalado para monitorear otras masas de agua, humedales y lagunas.

---

## ⚙️ Componentes del Sistema
### 🔌 Hardware:
- **Sensores** de pH, oxígeno disuelto, temperatura y conductividad eléctrica.
- **Placa Whitebox T2** compatible con **Arduino Mega**.
- **Módulos LoRa Heltec WiFi** para transmisión de datos hasta 2 km.
- **Placa NodeMCU 1.0** para conexión WiFi y subida de datos a la nube.
- **Fuentes de alimentación 5V** con conversión de voltaje **110VAC/5VDC**.
- **Cajas flotantes** para protección del sistema en la ciénaga.
- **Tubos de PVC** para guiar y proteger las puntas de los sensores.

### 💾 Software:
- **Arduino IDE** para la programación de los microcontroladores.
- **Firebase** para almacenamiento y gestión de datos en la nube.
- **Google Sheets** para el almacenamiento y visualización de datos históricos.
- **Aplicación móvil VAMCI** desarrollada en **Kodular** para consulta remota de datos.

---

## 📡 Funcionamiento del Sistema
1. **Sensores** capturan las variables ambientales en el agua.
2. **Arduino Mega + Whitebox T2** procesa los datos.
3. **Módulo LoRa** transmite la información al receptor en tierra.
4. **NodeMCU 1.0** recibe los datos y los sube a **Firebase**.
5. Los datos pueden visualizarse en **Google Sheets** o la **App VAMCI**.

---

## 📦 Instalación y Configuración
### 🔧 Requisitos:
- **Arduino IDE** instalado.
- **Librerías necesarias** para sensores y comunicación LoRa.
- Cuenta en **Firebase** y configuración de base de datos en tiempo real.
- Dispositivos de hardware correctamente ensamblados.

### 🚀 Pasos:
1. Clona este repositorio:
   ```bash
   git clone https://github.com/tuusuario/VAMCI.git
   ```
2. Carga el código en el **Arduino Mega** utilizando **Arduino IDE**.
3. Configura el **NodeMCU** con las credenciales de WiFi y Firebase.
4. Ensambla el sistema y coloca los sensores en la ciénaga.
5. Accede a la app VAMCI para visualizar los datos en tiempo real.

---

## 📱 Aplicación Móvil
- **Plataforma:** Kodular.
- **Funcionalidad:** Visualización de datos en gráficos y alertas.
- **Acceso:** Los datos se obtienen desde Firebase y se muestran en tiempo real.

---

## 📊 Resultados Esperados
- Monitoreo **automatizado y en tiempo real** de la calidad del agua.
- **Reducción de errores humanos** en la toma de mediciones.
- **Predicción de eventos climáticos** mediante análisis de datos históricos.

---

## 🤝 Contribuciones
¡Las contribuciones son bienvenidas! Si deseas mejorar este proyecto, por favor sigue estos pasos:
1. Realiza un **fork** del repositorio.
2. Crea una nueva rama:
   ```bash
   git checkout -b feature-nueva-funcion
   ```
3. Realiza cambios y sube tus commits:
   ```bash
   git commit -m "Añadida nueva función para X"
   ```
4. Envía un **pull request**.

---

## 📩 Contacto
Si tienes dudas o deseas colaborar, puedes contactarme en:
📧 **Correo:** [johan.rojasm@udea.edu.co]  
🔗 **LinkedIn:** [www.linkedin.com/in/johan-david-rojas-martinez-877a3a203]
