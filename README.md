<h1 align="center"> Sistema IoT para la monitorización y visualización de parámetros para un cultivo hidropónico con sistema de agricultura de precisión </h1>

   <p align="left">
   <img src="https://img.shields.io/badge/STATUS-EN%20DESAROLLO-green">
   </p>

<h2 align="center">
:construction: Proyecto en construcción :construction:
</h2>

Este proyecto busca crear un sistema de cultivo hidropónico que permita una gestión más eficiente del ambiente de cultivo, asegurando el crecimiento óptimo de las plantas y una mejor utilización de los recursos disponibles.

## Materiales utilizados:

-ESP32

![image](https://user-images.githubusercontent.com/82006043/224137550-7fbcaf5a-f3e6-49d3-8c83-b0e8df3526ee.png)


-Pantalla LCD de 20x4 con módulo I2C

![image](https://user-images.githubusercontent.com/82006043/224137905-e280cb1d-dde2-4f13-8823-4487c5ef809b.png)


-Encoder

![image](https://user-images.githubusercontent.com/82006043/224138072-f73256af-71d2-4f70-bb54-9baa1500afab.png)



-Sensores de temperatura DS18B20 

![image](https://user-images.githubusercontent.com/82006043/224138320-96548c25-a150-4988-88a8-d03218af654d.png)


-Sensores de humedad de suelo YL-69

![image](https://user-images.githubusercontent.com/82006043/224138562-e856dd73-6f99-4141-a6b2-eab34b3b28ba.png)


-Módulo fotoresistencia LDR

![image](https://user-images.githubusercontent.com/82006043/224138780-636fce41-d3fb-41d7-9d14-e70cb897b503.png)


-Sensor ultrasonido HC-SR04

![image](https://user-images.githubusercontent.com/82006043/224138928-04ed42ac-be92-4cd9-9d88-7f62be471723.png)


-Motobomba

![image](https://user-images.githubusercontent.com/82006043/224139037-5d327d1d-70e2-4631-9163-e753cbf29139.png)


-Electroválvula 

![image](https://user-images.githubusercontent.com/82006043/224139182-97905cda-888f-4249-a338-40773f57fc9d.png)


-Lámparas o reflectores construidos con LEDs ultravioleta

![image](https://user-images.githubusercontent.com/82006043/224139305-7c0ebfb3-284b-4256-94ef-edeabb3d2ba4.png)



## Integrando una interfaz interactiva para la visualización de datos en tiempo real en el sistema IoT de monitorización de parámetros para un cultivo hidropónico con técnicas de agricultura de precisión 

**Paso 1**: Se ha integrado una pantalla LCD de 20x4 con módulo I2C para la visualización de variables y el menú selectivo del proyecto de cultivo hidropónico, la cual está conectada a los pines D22 (SCL) y D21 (SDA) de la ESP32. Para su control se ha utilizado la librería LiquidCrystal_I2C.h, que permite la fácil comunicación entre la pantalla LCD y la ESP32. De esta manera, se pueden mostrar en tiempo real las variables obtenidas de los sensores y la información del menú selectivo, brindando una interfaz de usuario intuitiva y clara.

![image](https://user-images.githubusercontent.com/82006043/223729252-3f2b266f-6d0b-4777-ab42-2292b50e7985.png)

**Grafica 1**: Diagrama de conexión entre un ESP32 y una pantalla LCD 20x4 para la visualización de datos en un sistema IoT para el monitoreo de parámetros en un cultivo hidropónico con técnicas de agricultura de precisión. El ESP32 se conecta a la pantalla LCD mediante un bus I2C, permitiendo la transmisión de datos de forma eficiente. Esta conexión es esencial para la visualización de los datos del sistema IoT en tiempo real en el cultivo hidropónico.

**Paso 2**: Para la interacción del usuario con el sistema hidropónico se ha conectado y programado un encoder a la ESP32. Este encoder se ha conectado en los pines D5 (SW), D18 (DT) y D19 (CLK). La finalidad de este componente es permitir al usuario realizar la selección y navegación del menú del proyecto que se visualiza en la pantalla LCD.

![image](https://user-images.githubusercontent.com/82006043/223732043-cf89f7aa-6c5a-473a-9539-35674963c918.png)

**Grafica 2**: Diagrama de conexión entre un ESP32, una pantalla LCD 20x4 y un encoder para la navegación del menú en un sistema IoT para el monitoreo de parámetros en un cultivo hidropónico con técnicas de agricultura de precisión. El ESP32 se conecta a la pantalla LCD y al encoder mediante un bus I2C, permitiendo la transmisión de datos y la navegación del menú de forma eficiente. Esta conexión es esencial para la visualización y la interacción con los datos del sistema IoT en tiempo real en el cultivo hidropónico.

## Optimización del monitoreo en el cultivo hidropónico a través de la integración de sensores en el sistema IoT de agricultura de precisión para la recolección de datos en tiempo real

**Paso 1**: Para el sistema hidropónico se conectan y programan los sensores de temperatura DS18B20 en cascada, también conocido como modo parásito, en la ESP32, utilizando el pin D13. Se utilizan las librerías OneWire.h para la comunicación de los sensores en cascada y DallasTemperature.h para la lectura de los datos de los sensores. Los sensores se asignan de la siguiente forma:

-Medio ambiente = { 0x28, 0x4D, 0xE3, 0x79, 0x97, 0x14, 0x03, 0xE9 }

-Domo = { 0x28, 0x47, 0xCB, 0x79, 0x97, 0x11, 0x03, 0x1F }

-Tanque principal = { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0x14, 0x77, 0xB8 }

-Tanque de reserva = { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0xE9, 0x43, 0x77 }

-Plantas = { 0x28, 0xFF, 0x64, 0x1E, 0x0F, 0x77, 0x9F, 0xBC }

Estos sensores brindan información importante sobre la temperatura en diferentes partes del sistema hidropónico, lo que permite monitorear y ajustar el ambiente de cultivo para un mejor crecimiento de las plantas. Además, se utilizan en cascada para reducir la cantidad de pines utilizados en la ESP32.

![image](https://user-images.githubusercontent.com/82006043/223744352-b79702a2-007a-4e4c-bd98-e7483fd0bc9b.png)

**Grafica 3**: Diagrama de conexión entre un ESP32 y un sensor de temperatura DS18B20 para el monitoreo de parámetros en un sistema IoT para un cultivo hidropónico con técnicas de agricultura de precisión. El ESP32 se conecta al sensor de temperatura mediante un bus OneWire, permitiendo la transmisión de datos de forma eficiente. Esta conexión es esencial para la medición precisa y en tiempo real de la temperatura en el cultivo hidropónico, lo que permite tomar medidas oportunas para garantizar un ambiente adecuado para el crecimiento de las plantas.


**Paso 2**: Se realiza la conexión y programación de los sensores de humedad de suelo YL-69 en la ESP32, asignando los pines GPIO 36 para la planta 1 y GPIO 39 para la planta 2. Estos sensores se instalan en las plantas, donde llega la recirculación de agua del cultivo, siendo el de la planta 2 ubicado en el extremo final del sistema, donde se encuentra el desfogue del tanque principal hacia el tanque de reserva.

La función de estos sensores es medir el nivel de humedad en el suelo de cada planta, permitiendo determinar si la planta está recibiendo la cantidad adecuada de agua y nutrientes. Esto es especialmente útil en un sistema hidropónico, donde el agua se recircula y es necesario monitorear constantemente la calidad de la solución para mantener el crecimiento óptimo de las plantas.

![image](https://user-images.githubusercontent.com/82006043/223745931-f020231f-6dca-4e77-beeb-a9b8babf494f.png)

**Grafica 4**: Diagrama de conexión entre un ESP32 y un sensor de humedad YL-69 para el monitoreo de parámetros en un sistema IoT para un cultivo hidropónico con técnicas de agricultura de precisión. El ESP32 se conecta al sensor de humedad mediante un circuito divisor de voltaje, permitiendo la medición precisa de la humedad del suelo. Esta conexión es esencial para el monitoreo en tiempo real de la humedad del cultivo hidropónico, lo que permite ajustar la frecuencia de riego y evitar la sobre o sub irrigación que pueda afectar el crecimiento de las plantas.

**Paso 3**: Para detectar la cantidad de luz disponible en el ambiente y programar el ciclo de 18 horas de rayos ultravioleta en el cultivo hidropónico, se conecta y programa el módulo fotoresistencia LDR en la ESP32, en el pin D34. La utilización de este sensor permite una medición precisa de la cantidad de luz disponible en el ambiente, lo que permite programar el ciclo de rayos ultravioleta de forma adecuada.

Además, se habilita el RTC de la ESP32 para que la programación del ciclo de rayos ultravioleta sea aún más precisa y eficiente. De esta manera, se asegura que el ciclo de rayos ultravioleta se realice en el momento adecuado, garantizando una óptima calidad en el proceso de cultivo hidropónico.

![image](https://user-images.githubusercontent.com/82006043/223747102-ede16c01-c07d-4b6e-b144-c62dec27c446.png)

**Grafica 5**: Diagrama de conexión entre un ESP32 y un sensor de luz LDR para el monitoreo de parámetros en un sistema IoT para un cultivo hidropónico con técnicas de agricultura de precisión. El ESP32 se conecta al sensor LDR mediante un circuito divisor de voltaje, permitiendo la medición precisa de la intensidad de la luz. Esta conexión es esencial para completar el ciclo de luz requerido para la fotosíntesis del cultivo hidropónico, lo que permite mantener un ambiente adecuado para el crecimiento de las plantas y maximizar su productividad.


**Paso 4**:  Para una medición más eficiente del nivel de agua en el tanque de reserva, se implementa un Sensor Ultrasonido Hc Sr04 en el sistema. La utilización de este sensor permite una medición más precisa del nivel de agua, lo que se traduce en una mejor interpretación de los datos de medición.

En el código, se realizan transformaciones matemáticas para convertir la medición del sensor en valores de porcentaje de agua contenida en el tanque de reserva y en litros de agua disponibles en el sistema. De esta manera, se puede monitorear y visualizar con mayor precisión la cantidad de agua disponible en el sistema, lo que permite un mejor control del proceso de cultivo hidropónico.

![image](https://user-images.githubusercontent.com/82006043/223748270-0134334c-f853-4fa4-8710-7f170d03d11d.png)

**Grafica 6**: Diagrama de conexión entre un ESP32 y un sensor de ultrasonido para el monitoreo de parámetros en un sistema IoT para un cultivo hidropónico con técnicas de agricultura de precisión. El ESP32 se conecta al sensor de ultrasonido mediante pines digitales, permitiendo la medición del nivel de agua en los tanques de riego del cultivo hidropónico. Esta conexión es esencial para garantizar un suministro de agua adecuado para el cultivo y prevenir el bajo nivel de agua que puede afectar el crecimiento de las plantas.

## Conectividad y comunicación efectiva en el sistema IoT para la monitorización y visualización de parámetros en un cultivo hidropónico con agricultura de precisión

**Paso 1**: La implementación del módulo WiFi integrado en la ESP32, utilizando su respectiva librería, permite establecer una conexión con una red WiFi existente. Esto permite a la ESP32 recibir y transmitir mensajes de forma inalámbrica mediante la utilización de la señal WiFi. El uso de la librería correspondiente simplifica y agiliza el proceso de configuración y conexión a la red WiFi, permitiendo al programador enfocarse en el desarrollo de la lógica de la aplicación en lugar de dedicar tiempo y esfuerzo a la implementación de la funcionalidad de conectividad inalámbrica.

## Desarrollo del sistema de servidor y análisis de datos para el monitoreo virtual y visualización en tiempo real de gráficas en el cultivo hidropónico utilizando tecnologías de agricultura de precisión e IoT

**Paso 1**:  La comunicación entre la ESP32 y un servidor mediante el protocolo MQTT permite enviar variables desde la ESP32 al servidor, lo que permite una mejor interpretación y visualización de los datos. Esto se logra gracias a la librería PubSubClient.h, la cual facilita la implementación del protocolo MQTT en el código en lenguaje C. Al establecer una conexión al servidor MQTT, se pueden enviar mensajes con los valores de las variables que se desean monitorear y visualizar en tiempo real.

**Paso 2**: Se desarrolla el proyecto de comunicación entre una ESP32 y RabbitMQ mediante contenedores en Docker, los cuales también incluyen InfluxDB y Grafana. Estas últimas herramientas permitirán almacenar y visualizar los datos provenientes de las variables tomadas por la ESP32 en tiempo real. Todo esto se encuentra enmarcado en un proyecto de cultivo hidropónico, en el cual se busca optimizar el control y seguimiento de las condiciones del cultivo mediante el monitoreo constante de las variables ambientales, como la temperatura, la humedad y los niveles de nutrientes. El monitoreo constante de estas variables es fundamental para asegurar el crecimiento óptimo de las plantas y garantizar el éxito del cultivo. La combinación de la ESP32 con RabbitMQ permite una comunicación eficiente y confiable entre los diferentes elementos del sistema, mientras que InfluxDB y Grafana brindan una plataforma para almacenar y visualizar los datos en tiempo real. Además, al reducir la necesidad de supervisión y mantenimiento manual, se puede optimizar el tiempo y los recursos dedicados al cultivo, lo que se traduce en una mayor eficiencia y rentabilidad para los productores. En resumen, el proyecto de cultivo hidropónico que se está desarrollando utilizando la tecnología de contenedores, RabbitMQ, InfluxDB y Grafana es una excelente iniciativa que puede tener un impacto significativo en el sector agrícola. el proyecto ya se desarrollo

## Optimización del trabajo simultáneo en el sistema IoT para la monitorización de parámetros en un cultivo hidropónico mediante la división de tareas y el uso del sistema operativo FreeRTOS y técnicas de agricultura de precisión

**Paso 1**: La implementación de FreeDOS en la ESP32 para realizar múltiples tareas al mismo tiempo en el cultivo hidropónico es una actividad muy beneficiosa. Al aprovechar el poder de procesamiento de la ESP32, podemos ejecutar varias tareas críticas al mismo tiempo, como el monitoreo constante de las variables ambientales, el control del riego y la alimentación de nutrientes, y la comunicación con RabbitMQ.
La ventaja de usar FreeDOS es que nos permite ejecutar estas tareas sin tener que utilizar el segundo núcleo de la ESP32, lo que nos permite ahorrar energía y recursos y mejorar la capacidad de respuesta del sistema. Además, FreeDOS es una plataforma confiable y estable, lo que significa que podemos estar seguros de que nuestras tareas críticas se están ejecutando de manera efectiva.
Al mejorar la capacidad de respuesta del cultivo hidropónico, podemos garantizar un crecimiento óptimo de las plantas y mejorar la rentabilidad del proyecto. La capacidad de realizar múltiples tareas críticas al mismo tiempo nos permite tomar decisiones rápidas y precisas en función de los datos recopilados en tiempo real, lo que a su vez nos permite optimizar el uso de recursos y garantizar el éxito del cultivo.


## Actuadores

**Motobomba**: La motobomba es un dispositivo que está conectado al pin 13 de la placa ESP32. Su función es proporcionar el movimiento y la presión necesarios para recircular los nutrientes y controlar la temperatura dentro del domo de cultivo hidropónico. La técnica de cultivo hidropónico consiste en cultivar plantas sin suelo y utilizando una solución de nutrientes disuelta en agua para proporcionar los nutrientes necesarios. En este sistema, el agua se recircula constantemente para asegurarse de que las raíces de las plantas tengan acceso a los nutrientes y al agua en todo momento. La motobomba es esencial para mantener este proceso de recirculación y para garantizar que la temperatura dentro del domo de cultivo sea la adecuada para el crecimiento saludable de las plantas. Con el uso de la motobomba, se puede asegurar un flujo constante de nutrientes y agua para las plantas, lo que se traduce en un mayor rendimiento y una mejor calidad de los cultivos.

**Electroválvula de recirculación**: La electroválvula de recirculación es otro componente importante en los sistemas de cultivo hidropónico, ya que permite que el agua recircule desde el tanque de reserva al principal a través del sistema de tuberías y la motobomba, que genera el flujo de agua necesario. El control del flujo de agua en la electroválvula está a cargo del sistema de tiempo de la ESP32.

El flujo constante de agua en el sistema de cultivo hidropónico es importante para proporcionar nutrientes y oxígeno a las plantas, y para mantener una temperatura constante en el ambiente de cultivo. Además, la recirculación del agua es esencial para evitar el desperdicio y asegurar el uso eficiente de los nutrientes.

La electroválvula de recirculación se activa y desactiva mediante un temporizador o controlador de tiempo, que está conectado al pin 12 de la ESP32. Cuando se activa, permite que el agua fluya desde el tanque de reserva al tanque principal, donde se mezcla con los nutrientes y se oxigena mediante la acción de la motobomba. Luego, el agua se distribuye a través del sistema de tuberías y aspersores para llegar a las raíces de las plantas.

La recirculación constante del agua también ayuda a mantener la temperatura constante en el ambiente de cultivo hidropónico, ya que el agua actúa como un medio de transferencia de calor. El agua caliente se enfría a medida que fluye a través del sistema, y el agua fría se calienta a medida que se mezcla con el ambiente cálido del domo de cultivo. Esto ayuda a mantener una temperatura constante en el ambiente de cultivo, lo que es beneficioso para el crecimiento saludable de las plantas.

**La electroválvula de riego por nebulización**: es otro componente esencial en los sistemas de cultivo hidropónico que se utiliza para controlar la temperatura y la humedad en el ambiente de cultivo. Esta electroválvula se activa gracias al flujo generado por la motobomba, y su funcionamiento se basa en la ley de equilibrio de la termodinámica.

La electroválvula está conectada al pin 14 de la ESP32 y se activa mediante la lectura de los sensores de temperatura en el domo de cultivo hidropónico. Los sensores de temperatura monitorean constantemente la temperatura y la humedad en el ambiente de cultivo, y envían señales a la ESP32 para que controle el flujo de agua a través de la electroválvula de riego por nebulización.

Cuando los sensores detectan que la temperatura en el ambiente de cultivo es alta, la ESP32 activa la electroválvula de riego por nebulización, lo que hace que el agua se pulverice en forma de pequeñas gotas en el ambiente de cultivo. Este proceso ayuda a bajar la temperatura y aumentar la humedad en el ambiente de cultivo, lo que es beneficioso para el crecimiento saludable de las plantas.

La ley de equilibrio de la termodinámica establece que, cuando dos objetos con diferentes temperaturas se ponen en contacto, la energía se transferirá desde el objeto más caliente al más frío hasta que ambos objetos alcancen una temperatura de equilibrio. En el caso de la electroválvula de riego por nebulización, el agua pulverizada se evapora rápidamente en el ambiente de cultivo, lo que ayuda a bajar la temperatura.

Además, la humedad relativa en el ambiente de cultivo también aumenta debido a la evaporación del agua. Un ambiente con una humedad relativa adecuada es esencial para la absorción de nutrientes por parte de las plantas y para prevenir el crecimiento de bacterias y hongos en el cultivo hidropónico.

**Luz Ultravioleta**: Las lámparas o reflectores construidos con LEDs ultravioleta pueden ser utilizados en el cultivo hidropónico para mejorar el ciclo de rayos ultravioleta, lo que puede contribuir a la fotosíntesis de las plantas. Estas luces se pueden encender a través del pin 14 de la ESP32 y con la ayuda de un módulo LDR, lo que permite un control preciso de la iluminación en el cultivo hidropónico. Además, los LEDs ultravioleta pueden ayudar a mejorar la calidad y cantidad de los productos cultivados en sistemas hidropónicos al proporcionar una fuente de luz adicional para las plantas. Esto puede ser particularmente beneficioso en situaciones en las que la luz natural es limitada o insuficiente para las necesidades de las plantas. En general, las lámparas o reflectores LED ultravioleta pueden ser una herramienta útil para los cultivadores hidropónicos que buscan mejorar la salud y el rendimiento de sus cultivos.

