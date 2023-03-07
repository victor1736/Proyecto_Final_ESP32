# Proyecto_Final_ESP32
Cultivo Hidroponico con ESP32

Paso 1

Pantalla LCD 20x4 con módulo I2C para proyecto de cultivo hidropónico con ESP32

Este repositorio contiene el código necesario para controlar una pantalla LCD 20x4 con módulo I2C para visualizar variables y un menú selectivo en un proyecto de cultivo hidropónico utilizando un microcontrolador ESP32.

Materiales necesarios

-ESP32
-Pantalla LCD 20x4 con módulo I2C
-Cables de conexión
Conexión

La pantalla LCD debe conectarse a los pines D22 (SCL) y D21 (SDA) del ESP32.

Dependencias

El código requiere la librería LiquidCrystal_I2C para controlar la pantalla LCD, la cual se puede descargar desde el administrador de librerías de Arduino IDE.

Uso:

El código incluido en este repositorio muestra un ejemplo de cómo controlar la pantalla LCD y cómo visualizar variables y un menú selectivo en ella. El usuario puede adaptar el código para su propio proyecto modificando las variables y opciones de menú según sea necesario.

![image](https://user-images.githubusercontent.com/82006043/217104121-40c075ec-d1b5-40aa-b712-c70124172aa9.png)




Integracion del menu por medio del encoder 

![image](https://user-images.githubusercontent.com/82006043/218502063-f2fdf95d-f7b3-481d-9429-c34b3fca2477.png)

Se implementa un encoder para la selecion del menu que se imprime en la pantalla LCD 20x4 
