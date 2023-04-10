#include <ESP32Time.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HardwareSerial.h>

  char estado_anterior_quectel=0;
  char estado_actual_quectel=0;
#define TIME 5000
#define MSG_BUFFER_SIZE  (300)
String alerta_mensaje[] = {"Temperatura del domo por fuera del rango\r\n","Temperatura del domo por dentro del rango\r\n","Temperatura del agua fuera de rango\r\n","Temperatura del agua dentro del rango\r\n","Encendido de lamparas\r\n","Apagado de lamparas\r\n","\x1A"};
// Configuración del puerto serie
HardwareSerial quectelSerial(2); // Utilizar el puerto serie 2 del ESP32
#define QUECTEL_BAUDRATE 115200 // Baudrate del módulo Quectel
  enum {
  FSM_ESTADO_INICIO=0,
  FSM_ESTADO_ENVIANDO_AT,
  FSM_ESTADO_ENVIANDO_ATI,
  FSM_ESTADO_ENVIANDO_CPIN,
  FSM_ESTADO_ENVIANDO_CREG,
  FSM_ESTADO_ENVIANDO_CMGF,
  FSM_ESTADO_ENVIANDO_CMGS,
  FSM_ESTADO_MENSAJE_TXT,
  FSM_ESTADO_ERROR,
  FSM_ESTADO_MENSAJE_ENVIADO,
};

String texto;
//Constantes de servidor
  const char* IP_SERVER = "34.203.228.246";
  const char* MQTT_USER = "guest";
  const char* MQTT_PASSWORD = "guest";
  const char* DATA_TOPIC = "data";
  String CLIENT_ID = "ESP32Client-" + String(random(0xffff), HEX);
   String Fecha ;
   String HORA;
   long Hora;

//Mensaje a rabbit
  unsigned long lastMsg = 0;
  char msg[MSG_BUFFER_SIZE]; //Message to send to RabbitMQ

//Initialization WiFi Client and MQTT Client
  WiFiClient espClient;
  PubSubClient client(espClient);

//RTC
  ESP32Time rtc;

  //Configuracion de servidor NTP
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = -5*3600;
    const int daylightOffset_sec = 0;


// Final


TaskHandle_t task1;
TaskHandle_t task2;
TaskHandle_t task3;
TaskHandle_t task4;
TaskHandle_t task5;
TaskHandle_t task6;


//Tanque
    byte A[8] = {
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10111
  };
  byte B[8] = {
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b11101
  };
  byte C[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
  };
  byte D[8] = {
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10111,
    0b10000,
    0b11111
  };
  byte E[8] = {
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b11101,
    0b00001,
    0b11111
  };
  byte F[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b11111
  };
    byte H[8] = {
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000
  };
    byte I[8] = {
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001
  };



// La LCD
  LiquidCrystal_I2C lcd(0x27, 20, 4);
//Sensor de humedad  
  const int humedad = 36 ;
  int valor_humedad = 0;
  const int humedad1 = 39 ;
  int valor_humedad1 = 0 ;

//Fotoresistencia
  const int fotoresistencia = 34 ;
  int valor_fotoresistencia = 0 ;
  int tiempodeencendidodeluz = 10000;
  
//Medición nivel del tanque de reserva 
  const int PinTrig = 4;
  const int PinEcho = 15;
  int distancia = 0;
//Constante de el Gpio 12 para la lectura de los sensores de temperatura en modo parasito 
  #define ONE_WIRE_BUS 13
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire); 
  DeviceAddress sensor1 = { 0x28, 0x4D, 0xE3, 0x79, 0x97, 0x14, 0x3, 0xE9 };
  DeviceAddress sensor2 = { 0x28, 0x26, 0xE3, 0x1,  0x0,  0x0,  0x0, 0x1 };
  DeviceAddress sensor3= { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0x14, 0x77, 0xB8 };
  DeviceAddress sensor4 = { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0xE9, 0x43, 0x77 };
  DeviceAddress sensor5 = { 0x28, 0xFF, 0x64, 0x1E, 0xF, 0x77, 0x9F, 0xBC };
  float Sensor_medio_ambiente ;
  float Sensor_domo ;
  float Sensor_tanque_princial ;
  float Sensor_tanque_reserva ;
  float Sensor_plantas ;
//Constantes de encoder
  #define encoderCLK 19 
  #define encoderDT  18  
  #define encoderSW  5 
  int Estado = 1; 
  int Sig_Estado = 1;

//parte operativa
  const int motobomba =  12;
  const int recirculacion = 14;
  const int riego = 27;
  const int Luz_ultravioleta = 27;
  unsigned long currentTime=0;
  unsigned long previousTime=0;
  unsigned long currentTime1=0;
  unsigned long previousTime1=0;
  unsigned long recirculacion_on = 900000;
  unsigned long recirculacion_off = 900000;
  unsigned long interval ;
  unsigned long interval1 ;
  bool estado_motobomba = LOW;
  bool estado_recirculacion = LOW;
  bool estado_luz_ultravioleta = LOW;

void setup() {

  // Iniciar LCD
  lcd.begin(20, 4);
  lcd.init();
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cultivo");
  lcd.setCursor(0, 1); // Segunda fila
  lcd.print("Hidroponico");
  delay(200);
  pinMode (encoderCLK,INPUT_PULLUP);
  pinMode (encoderDT,INPUT_PULLUP);
  pinMode (encoderSW,INPUT_PULLUP);

//Monitor serial 
  Serial.begin(115200);
  quectelSerial.begin(QUECTEL_BAUDRATE);

//Pines de nivel de agua
  pinMode(PinTrig, OUTPUT);
  pinMode(PinEcho, INPUT);
  digitalWrite(PinTrig, LOW);//Inicializamos el pin con 0
  lcd.createChar (0,A);
  lcd.createChar (1,B);
  lcd.createChar (2,C);
  lcd.createChar (3,D);
  lcd.createChar (4,E);
  lcd.createChar (5,F);
  lcd.createChar (6,H);
  lcd.createChar (7,I);

      //Iniciando los sensores de temperatura
       sensors.begin();
       //inicializacion de sensores
      sensors.requestTemperatures();

xTaskCreatePinnedToCore(
  actuador, // Function to call
  "task1", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  1, // Priority 
  &task1, // Task handler to use
  1 // Core where to run
);

xTaskCreatePinnedToCore(
  LUZ_ULTRAVIOLETA, // Function to call
  "task2", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  2, // Priority 
  &task2, // Task handler to use
  1 // Core where to run
);

xTaskCreatePinnedToCore(
  SERVIDOR, // Function to call
  "task3", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  1, // Priority 
  &task3, // Task handler to use
  1 // Core where to run
);

xTaskCreatePinnedToCore(
  Circulacion_intermitente, // Function to call
  "task4", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  2, // Priority 
  &task4, // Task handler to use
  1 // Core where to run
);

xTaskCreatePinnedToCore(
  Control_temperaruta_Tanques, // Function to call
  "task5", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  2, // Priority 
  &task5, // Task handler to use
  1 // Core where to run
);

xTaskCreatePinnedToCore(
  Control_temperaruta_Domo, // Function to call
  "task6", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  2, // Priority 
  &task6, // Task handler to use
  1 // Core where to run
);




}

void loop() {
  String menusaliendo[] = {"LCD 20x4","Saliendo...","Version 1.0.0"};
  Estado = Sig_Estado;
  if(Estado == 1)
  {
  
      int menu;
      String arrayMenu[] = {"Informacion", "Temperatura","Humedad","Ciclo de agua", "Luz Ultravioleta", "Nivel de Agua"};
      int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
      
      menu = menuANTIFALLOSLENTO(arrayMenu,size); 
    
      if(menu == -1)Sig_Estado = 1;
      else if(menu == 1)Sig_Estado = 2;
      else if(menu == 2)Sig_Estado = 3;
      else if(menu == 3)Sig_Estado = 4;
      else if(menu == 4)Sig_Estado = 5;
      else if(menu == 5)Sig_Estado = 6;
      else if(menu == 6)Sig_Estado = 7;

      }
        else if( Estado == 2){
        int menu;
        String arrayMenu[] = { "Materiales", "Version", "Cerrar"};
        int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
        menu = menuANTIFALLOSLENTO(arrayMenu,size);
        
        if(menu == -1)Sig_Estado = 1;
        else if(menu == 1)Sig_Estado = 31;
        else if(menu == 2)Sig_Estado = 32;
        else if(menu == 3)Sig_Estado = 33;
        Estado = Sig_Estado;
            if( Estado == 31)
            {
                int menu;
                String arrayMenu[] = { "Pantalla LCD", "Modulo", "Motobomba", "Sensor Temperatura", "Sensor Humedad","Sensor Fotovoltaico","Modulo Relay","Botones","Reflector", "cerrar"};
                int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
                menu = menuANTIFALLOSLENTO(arrayMenu,size);
        
                if(menu == -1)Sig_Estado = 1;
                else if(menu == 1)Sig_Estado = 91;
                else if(menu == 2)Sig_Estado = 92;
                else if(menu == 3)Sig_Estado = 93;
                else if(menu == 4)Sig_Estado = 94;
                else if(menu == 5)Sig_Estado = 95;
                else if(menu == 6)Sig_Estado = 96;
                else if(menu == 7)Sig_Estado = 97;
                else if(menu == 8)Sig_Estado = 98;
                else if(menu == 9)Sig_Estado = 99;
                else if(menu == 10)Sig_Estado = 100;
                Estado = Sig_Estado;  
                 String materiales[] = {"Modulo:","ESP32","Motobomba:", "Wolf","Sensor Temperatura:","DS18b20","Sensor Humedad:","Higrometro","Fotoresistencia:","Sensor de luz","Modulo Relay:","4 Canales","Botones:","Encoder","Reflector:","Leds Ultravioleta"};
                 
                    if( Estado == 91)
                    {
                      menu_print_opciones_uno(menusaliendo,0);
                      Sig_Estado = 1;
                    }
                    if( Estado == 92)
                    {
                      menu_print_opciones(materiales,0, 1);
                      Sig_Estado = 1;
                    }
                    if( Estado == 93)
                    {
                      menu_print_opciones(materiales,2, 3);
                      Sig_Estado = 1;
                    }
                    if( Estado == 94)
                    {
                      menu_print_opciones(materiales,4, 5);
                      Sig_Estado = 1;
                    }
                    if( Estado == 95)
                    {
                      menu_print_opciones(materiales,6, 7);
                      Sig_Estado = 1;
                    }
                    if( Estado == 96)
                    {
                      menu_print_opciones(materiales,8, 9);
                      Sig_Estado = 1;
                    }
                    if( Estado == 97)
                    {
                      menu_print_opciones(materiales,10, 11);
                      Sig_Estado = 1;
                    }
                    if( Estado == 98)
                    {
                      menu_print_opciones(materiales,12, 13);
                      Sig_Estado = 1;
                    }
                    if( Estado == 99)
                    {
                      menu_print_opciones(materiales,14, 15);
                      Sig_Estado = 1;
                    }
                    if( Estado == 100)
                    {
                      menu_print_opciones_uno(menusaliendo,1);
                      Sig_Estado = 1; 
                    }
                    Estado = Sig_Estado;
            }
           if( Estado == 32)
            {
              menu_print_opciones_uno(menusaliendo,2);
              Sig_Estado = 1;
            }
            if( Estado == 33)
            {
              menu_print_opciones_uno(menusaliendo,1);
              Sig_Estado = 1; 
            }
           Estado = Sig_Estado;
        }
        else if( Estado == 3){
        int menu;
        String arrayMenu[] = { "Medio ambiente", "Domo", "Tanque Principal", "Tanque Reserva", "Plantas", "cerrar"};
        int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
        menu = menuANTIFALLOSLENTO(arrayMenu,size);

        if(menu == -1)Sig_Estado = 1;
        else if(menu == 1)Sig_Estado = 41;
        else if(menu == 2)Sig_Estado = 42;
        else if(menu == 3)Sig_Estado = 43;
        else if(menu == 4)Sig_Estado = 44;
        else if(menu == 5)Sig_Estado = 45;
        else if(menu == 6)Sig_Estado = 46;
        Estado = Sig_Estado; 
        String cambio_variable1 = String (Sensor_medio_ambiente);
        String cambio_variable2 = String (Sensor_domo);
        String cambio_variable3 = String (Sensor_tanque_princial);
        String cambio_variable4 = String (Sensor_tanque_reserva);
        String cambio_variable5 = String (Sensor_plantas);
        String temperaturas_opciones []={"Medio Ambiente:",cambio_variable1,"Domo:",cambio_variable2,"Tanque Principal:",cambio_variable3,"Tanque de Reserva:",cambio_variable4,"Plantas:",cambio_variable5};
        if( Estado == 41)
                    {
                      menu_print_opciones(temperaturas_opciones,0, 1);
                      Sig_Estado = 1;
                    }
        if( Estado == 42)
                    {
                      menu_print_opciones(temperaturas_opciones,2, 3);
                      Sig_Estado = 1;
                    }
        if( Estado == 43)
                    {
                      menu_print_opciones(temperaturas_opciones,4, 5);
                      Sig_Estado = 1;
                    }
        if( Estado == 44)
                    {
                      menu_print_opciones(temperaturas_opciones,6, 7);
                      Sig_Estado = 1;
                    }
        if( Estado == 45)
                    {
                      menu_print_opciones(temperaturas_opciones,8, 9);
                      Sig_Estado = 1;
                    }  
        if( Estado == 46)
            {
              menu_print_opciones_uno(menusaliendo,1);
              Sig_Estado = 1; 
            }
        Estado = Sig_Estado;                                                          
      }
      else if( Estado == 4){
        int menu;
        String arrayMenu[] = {"Planta 1", "Planta 2","cerrar"};
        int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
        menu = menuANTIFALLOSLENTO(arrayMenu,size);

        if(menu == -1)Sig_Estado = 1;
        else if(menu == 1)Sig_Estado = 51;
        else if(menu == 2)Sig_Estado = 52;
        else if(menu == 3)Sig_Estado = 53;
        Estado = Sig_Estado;
        String cambio_variable_humedad = String (valor_humedad);
        String cambio_variable_humedad1 = String (valor_humedad1);
        String humedad_opciones[]={"Planta 1:",cambio_variable_humedad,"Planta 2:",cambio_variable_humedad1};
        if( Estado == 51)
                    {
                      menu_print_opciones(humedad_opciones,0, 1);
                      Sig_Estado = 1;
                    } 
        if( Estado == 52)
                    {
                      menu_print_opciones(humedad_opciones,2, 3);
                      Sig_Estado = 1;
                    } 
        if( Estado ==53)
            {
              menu_print_opciones_uno(menusaliendo,1);
              Sig_Estado = 1; 
            }
        Estado = Sig_Estado;                     
        
      }
      else if( Estado == 5){
        int menu;
        String arrayMenu[] = {"Tiempo de Encendido", "Tiempo de apagado", "Cerrar"};
        int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
        menu = menuANTIFALLOSLENTO(arrayMenu,size);

        if(menu == -1)Sig_Estado = 1;
        else if(menu == 1)Sig_Estado = 61;
        else if(menu == 2)Sig_Estado = 62;
        else if(menu == 3)Sig_Estado = 63;
        Estado = Sig_Estado;
        if( Estado == 61)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tiempo de Encendido:");
                      lcd.setCursor(0, 1);
                      lcd.print(recirculacion_on/60000);
                      lcd.setCursor(0, 2);
                      lcd.print("Minutos");
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado == 62)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tiempo de apagado:");
                      lcd.setCursor(0, 1);
                      lcd.print(recirculacion_off/60000);
                      lcd.setCursor(0, 2);
                      lcd.print("Minutos");
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado ==63)
            {
              menu_print_opciones_uno(menusaliendo,1);
              Sig_Estado = 1; 
            }
        Estado = Sig_Estado;
      }

      else if( Estado == 6){
        int menu;
        String arrayMenu[] = {"Lectura","Tiempo de Encendido", "Tiempo de apagado", "Cerrar"};
        int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
        menu = menuANTIFALLOSLENTO(arrayMenu,size);

        if(menu == -1)Sig_Estado = 1;
        else if(menu == 1)Sig_Estado = 71;
        else if(menu == 2)Sig_Estado = 72;
        else if(menu == 3)Sig_Estado = 73;
        else if(menu == 4)Sig_Estado = 74;
        Estado = Sig_Estado;
        String cambio_variable_ldr = String (valor_fotoresistencia);
        String ldr_opciones[]={"Lectura:",cambio_variable_ldr,"Tiempo de Encendido:","De 5 a 23 Horas","Tiempo de apagado:", "De 23 a 5 Horas"};
        if( Estado == 71)
                    {
                      menu_print_opciones(ldr_opciones,0, 1);
                      Sig_Estado = 1;
                    }
        if( Estado == 72)
                    {
                      menu_print_opciones(ldr_opciones,2, 3);
                      Sig_Estado = 1;
                    } 
        if( Estado == 73)
                    {
                      menu_print_opciones(ldr_opciones,4, 5);
                      Sig_Estado = 1;
                    } 
        if( Estado ==74)
            {
              menu_print_opciones_uno(menusaliendo,1);
              Sig_Estado = 1; 
            }
        Estado = Sig_Estado;
      }
      else if( Estado == 7){
            //medidas del tanque
            int lleno =  2;
            int vacio = 15;
            distancia = 0.01723 * ultrasonido(PinTrig, PinEcho);
            Serial.println(distancia);
            distancia = map(distancia, vacio , lleno, 0, 100);
            if (distancia < 100 && distancia>90){
            lcd.clear(); 
            lcd.setCursor (0,1);
            lcd.write(byte(0));
            lcd.write(byte(2));
            lcd.write(byte(2));
            lcd.write(byte(2)); 
            lcd.write(byte(1));
            lcd.setCursor (0,2);
            lcd.write(byte(0));
            lcd.write(byte(2));
            lcd.write(byte(2));
            lcd.write(byte(2)); 
            lcd.write(byte(1));
            lcd.setCursor (0,3);
            lcd.write(byte(3));
            lcd.write(byte(5));
            lcd.write(byte(5));
            lcd.write(byte(5)); 
            lcd.write(byte(4)); 
            lcd.setCursor(6,1);
            lcd.print("Nivel:");
            lcd.setCursor(6,2);
            lcd.print(distancia);
            }
            if (distancia < 90 && distancia>40){
            lcd.clear(); 
            lcd.setCursor (0,1);
            lcd.write(byte(6)); 
            lcd.setCursor (4,1);
            lcd.write(byte(7));
            lcd.setCursor (0,2);
            lcd.write(byte(0));
            lcd.write(byte(2));
            lcd.write(byte(2));
            lcd.write(byte(2)); 
            lcd.write(byte(1));
            lcd.setCursor (0,3);
            lcd.write(byte(3));
            lcd.write(byte(5));
            lcd.write(byte(5));
            lcd.write(byte(5)); 
            lcd.write(byte(4));
            lcd.setCursor(6,1);
            lcd.print("Nivel:");
            lcd.setCursor(6,2);
            lcd.print(distancia);
            
            }
            if (distancia < 40){
            lcd.clear(); 
            lcd.setCursor (0,1);
            lcd.write(byte(6)); 
            lcd.setCursor (4,1);
            lcd.write(byte(7));
            lcd.setCursor (0,2);
            lcd.write(byte(6)); 
            lcd.setCursor (4,2);
            lcd.write(byte(7));
            lcd.setCursor (0,3);
            lcd.write(byte(3));
            lcd.write(byte(5));
            lcd.write(byte(5));
            lcd.write(byte(5)); 
            lcd.write(byte(4)); 
            lcd.setCursor(6,1);
            lcd.print("Nivel:");
            lcd.setCursor(6,2);
            lcd.print(distancia);
            }
            delay(5000);
            Sig_Estado = 1;
      }
}

//Encoder
int menuANTIFALLOSLENTO(String *arrayMenu, int size) {
  //Variables locales
  int estadoCLK = LOW;
  int estado_pasado_clk = LOW;
  int estadoDT = LOW;
  int estado_pasado_DT = LOW;
  int estadoSW = LOW;
  int estado_pasado_SW = LOW;
  unsigned long tiempoCambioIncremento = 0;
  unsigned long tiempoCambioDecremento = 0;
  int extraOpcion = 0;
  float incremento = 1;

  //Menu
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("> ");
  float opcion = 1;  //del 1 al 1.75 Opcion 1  //Del 2  al 2.75 opcion 2

  //Pinta los 4 primeros del menu como mucho
  for(int x = 0; x < size && x <= 3 ; x++) { 
    lcd.setCursor(1,x);
    lcd.print(arrayMenu[x]);
  }
  delay(500);

  while(digitalRead(encoderSW) == HIGH) {
    //Lectura del estado actual del encoderCLK
    estadoCLK = digitalRead(encoderCLK);
    //Lectura del estado actual del DT
    estadoDT = digitalRead(encoderDT);
    //Lectura del estado actual SW
    estadoSW = digitalRead(encoderSW);
  
    if (estado_pasado_clk == LOW  && estadoCLK == HIGH){
      if (estadoDT == LOW){
        if(opcion < size) {
          opcion += incremento;
          tiempoCambioIncremento = millis();
        }
      } else {
        if(opcion > 1) {
          opcion -= incremento;
          tiempoCambioDecremento = millis();
        }
      }

      //Si sobrepasamos el limite por debajo
      if(opcion < 1 + extraOpcion) extraOpcion--;
      //Si sobrepasamos el limite por encima
      if(opcion > 4 + extraOpcion) extraOpcion++;

      //Pintamos de nuevo el menu
      lcd.clear();
      for(int x = extraOpcion; x < size && x <= (3+extraOpcion) ; x++) {  
        lcd.setCursor(1,x - extraOpcion);
        lcd.print(arrayMenu[x]);
      }
      //Pintamos el cursor
      lcd.setCursor(0,opcion-1-extraOpcion);
      lcd.print(">");

    }

    estado_pasado_clk = estadoCLK;
    estado_pasado_DT = estadoDT;
    estado_pasado_SW = estadoSW;
    yield();
  }

  //Aqui hemos salido del bucle ya que hemos pulsado el boton Enter o ATRAS
  if(millis() - tiempoCambioIncremento < 300) {
    opcion -= incremento;
  } else if(millis() - tiempoCambioDecremento < 300) {
    opcion += incremento;
  }
  
  return opcion;
}


long ultrasonido (int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

//Freedos

void actuador(void *parameter) {
//parte operativa
      pinMode(motobomba, OUTPUT);
      pinMode(recirculacion, OUTPUT);
      pinMode(riego, OUTPUT);
      digitalWrite(riego, LOW);
      pinMode(Luz_ultravioleta, OUTPUT);
      digitalWrite(Luz_ultravioleta, LOW);

      for (;;) { 
        //RTC 
        configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)){
          rtc.setTimeStruct(timeinfo);
        }
        
       //inicializacion de sensores
      sensors.requestTemperatures();
      valor_humedad = analogRead(humedad);
      valor_humedad1 = analogRead(humedad1);
      valor_humedad =map(valor_humedad, 4095 , 1300, 0, 100);
      valor_humedad1 = map(valor_humedad1, 4095 , 1300, 0, 100);
      Sensor_medio_ambiente = sensors.getTempC(sensor1);
      Sensor_domo = sensors.getTempC(sensor2);
      Sensor_tanque_princial = sensors.getTempC(sensor3);
      Sensor_tanque_reserva = sensors.getTempC(sensor4);
      Sensor_plantas = sensors.getTempC(sensor5);
      valor_fotoresistencia = analogRead(fotoresistencia);
      valor_fotoresistencia =map(valor_fotoresistencia, 4095 , 0, 0, 100);
      Fecha = rtc.getTime("%Y/%m/%d %H:%M:%S");
      HORA = rtc.getTime("%H");
      Hora = HORA.toInt();
          
yield();
  }
  vTaskDelay(10);

}

///////////////////////////////////////////////////////////////////////////parte de conexion////////////////////////////////////////
//Funcion de wifi
  void setup_wifi() {
  //Constantes de wifi
  const char* WIFI_SSID = "DESKTOP-G34CLR6 7356";
  const char* WIFI_PASSWORD = "cronos12345678";
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }

  void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(CLIENT_ID.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(DATA_TOPIC, "hello world");
      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");

  }

void SERVIDOR(void *parameter) {

    setup_wifi();
    client.setServer(IP_SERVER, 1883);
    client.setCallback(callback);
 {
    /* data */
  };
  
  for (;;) { 
            if (!client.connected()) {
                reconnect();
              }
              client.loop();

              unsigned long now = millis();
                if (now - lastMsg > TIME) {
                  lastMsg = now;
                  snprintf (msg, MSG_BUFFER_SIZE, "measurements Temperatura_medio_ambiente=%.2f,Temperatura_Domo=%.2f,Temperatura_Tanque_Principal=%.2f,Temperatura_Tanque_de_reserva=%.2f,Temperatura_Plantas=%.2f,Humedad_Planta_1=%d,Humedad_Planta_2=%d,Fotoresistencia=%d", Sensor_medio_ambiente, Sensor_domo, Sensor_tanque_princial,Sensor_tanque_reserva,Sensor_plantas,valor_humedad,valor_humedad1,valor_fotoresistencia);
                  if (client.connect(CLIENT_ID.c_str(), MQTT_USER, MQTT_PASSWORD)) {
                    client.publish(DATA_TOPIC, msg);
                  }  
                }
                delay(10000);
                yield();
      }
  vTaskDelay(10);

}

///////////////////////////////////////////////////////////////////////////parte de control////////////////////////////////////////
void LUZ_ULTRAVIOLETA(void *parameter) {
  const int HORA_ENCENDIDO = 5;
  const int HORA_APAGADO = 23;

  bool estado_anterior = false; // Inicialmente la luz está apagada

  for (;;) {
      bool estado_actual = (Hora >= HORA_ENCENDIDO && Hora <= HORA_APAGADO && valor_fotoresistencia < 30);
      if (estado_actual) {
          digitalWrite(Luz_ultravioleta, HIGH);
        } else {
          digitalWrite(Luz_ultravioleta, LOW);
        }

      if (estado_anterior != estado_actual) {
          estado_anterior = estado_actual;
          COMANDOS_AT(alerta_mensaje);
          Serial.print("Hora: ");
          Serial.println(Hora);
          Serial.print("Estado de la luz: ");
          Serial.println(estado_actual ? "Encendido" : "Apagado");
          
      }

    yield();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}



///////////////////////////////////////////////////////////////////
void Circulacion_intermitente(void *parameter) {

  for (;;) { 
          //parte operativa
      //recirculacion o alimentacion
            currentTime=millis();
      if(estado_recirculacion == LOW){
        interval=recirculacion_off;
//        Serial.println("apague");
      }else{
        interval=recirculacion_on;
//        Serial.println("Encendi");
      }

        if((currentTime-previousTime)>interval){
        previousTime=currentTime;
        estado_motobomba =! estado_motobomba;
        estado_recirculacion =! estado_recirculacion;
        digitalWrite(motobomba,!estado_motobomba);
        digitalWrite(recirculacion,!estado_recirculacion);
        Serial.print(F("motobomba State : "));Serial.println(estado_motobomba);
        Serial.print(F("recirculacion State : "));Serial.println(estado_recirculacion);
      }

      delay(1000);
      yield();
      }
  vTaskDelay(10);
}
/////////////////////////////////////////////////////////////////

void Control_temperaruta_Tanques(void *parameter) {
  bool estado_anterior = false;
  for (;;) { 
    bool estado_actual = (Sensor_tanque_princial >= 18 && Sensor_tanque_princial <= 27);
          if (estado_actual) {
            digitalWrite(motobomba, LOW);
            digitalWrite(recirculacion, LOW);
        } else {
            digitalWrite(motobomba, HIGH);
            digitalWrite(recirculacion, HIGH);
        }
          if (estado_anterior != estado_actual) {
          estado_anterior = estado_actual;
          COMANDOS_AT(alerta_mensaje);
          Serial.print("Estado de la contro de temperatura tanques: ");
          Serial.println(estado_actual ? "dentro de  rango" : "fuera de rango ");
          
      }
      delay(1000);
      yield();
      }
  vTaskDelay(10);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Control_temperaruta_Domo(void *parameter) {
  bool estado_anterior_dia = false;
  bool estado_anterior_noche = false;
  for (;;) { 
    bool estado_actual_dia =((Hora >= 5 && Hora <= 18) && (Sensor_domo < 25 && Sensor_domo > 20 ));
    bool estado_actual_noche =((Hora < 5 || Hora > 18) && (Sensor_domo <= 20 && Sensor_domo >= 15 ));
              if (estado_actual_dia) {
                    digitalWrite(motobomba, LOW);
                    digitalWrite(riego, LOW); 
                  }else{
                    digitalWrite(motobomba, HIGH);
                    digitalWrite(riego, HIGH); 
                  }
                   
               if (estado_actual_noche) {
                    digitalWrite(motobomba, LOW);
                    digitalWrite(riego, LOW);  
                  }else {
                    digitalWrite(motobomba, HIGH);
                    digitalWrite(riego, HIGH);
                  }

          if (estado_anterior_dia != estado_actual_dia) {
          estado_anterior_dia = estado_actual_dia;
          COMANDOS_AT(alerta_mensaje);
          Serial.print("Estado de la control de temperatura domo dia: ");
          Serial.println(estado_actual_dia ? "dentro de  rango" : "fuera de rango ");
          }
          if (estado_anterior_noche != estado_actual_noche) {
          estado_anterior_noche = estado_actual_noche;
          COMANDOS_AT(alerta_mensaje);
          Serial.print("Estado de la control de temperatura domo noche: ");
          Serial.println(estado_actual_noche ? "dentro de  rango" : "fuera de rango ");
          }

                  
                    delay(1000);
                    yield();
      }
  vTaskDelay(10);
}

void menu_print_opciones(String lista[], int opcion1, int opcion2) {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(lista[opcion1]);
      lcd.setCursor(0, 2);
      lcd.print(lista[opcion2]);
  unsigned long tiempoInicio = millis();
  while (millis() - tiempoInicio < 5000) {

    yield();
  }
}

void menu_print_opciones_uno(String lista[], int opcion1) {
      lcd.clear();
      lcd.print(lista[opcion1]);
  unsigned long tiempoInicio = millis();
  while (millis() - tiempoInicio < 5000) {

    yield();
  }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COMANDOS_AT(String lista[] ) {
  unsigned long tiempo_envio_comando_AT = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_AT = 500;  
  unsigned long tiempo_envio_comando_ATI = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_ATI = 500;
  unsigned long tiempo_envio_comando_CPIN = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_CPIN = 5000;
  unsigned long tiempo_envio_comando_CREG = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_CREG = 500;
  unsigned long tiempo_envio_comando_CMGF = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_CMGF = 500;
  unsigned long tiempo_envio_comando_CMGS = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_CMGS = 500;
  unsigned long tiempo_envio_comando_TXT = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_TXT = 500;
  unsigned long tiempo_envio_comando_ENVIADO = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_ENVIADO = 120000;
  unsigned long tiempo_envio_comando_ERROR = 0;
  const unsigned long RETARDO_ENVIO_COMANDO_ERROR = 50000;
  const int HORA_ENCENDIDO = 5;
  const int HORA_APAGADO = 23;

        if (millis() - tiempo_envio_comando_AT >= RETARDO_ENVIO_COMANDO_AT) {
          quectelSerial.print("AT\r\n");  //Envia comando AT
          texto=quectelSerial.readString();
          Serial.println(texto);
          tiempo_envio_comando_AT = millis();
        }
          if (texto.indexOf("OK")>0) {
            texto="";
               if (millis() - tiempo_envio_comando_ATI >= RETARDO_ENVIO_COMANDO_ATI) {
                quectelSerial.println("ATI\r\n"); //Envia comando AT
                texto = quectelSerial.readString();
                Serial.println(texto);
                tiempo_envio_comando_ATI = millis();
              }
                if (texto.indexOf("EC20") > 0) {
                  texto="";
                    if (millis() - tiempo_envio_comando_CPIN >= RETARDO_ENVIO_COMANDO_CPIN) {
                      quectelSerial.println("AT+CPIN?\r\n");  //Envia comando AT+CPIN?
                      quectelSerial.available();
                      texto=quectelSerial.readString();
                      Serial.println(texto);
                      tiempo_envio_comando_CPIN = millis();
                    }
                              if(texto.indexOf("OK")>0){
                                texto="";
                                  if (millis() - tiempo_envio_comando_CREG >= RETARDO_ENVIO_COMANDO_CREG) {
                                      quectelSerial.println("AT+CREG?\r\n");  //Envia comando AT+CREG?
                                      quectelSerial.available();
                                      texto=quectelSerial.readString();
                                      Serial.println(texto);
                                      tiempo_envio_comando_CREG = millis();
                                    }
                                              if(texto.indexOf("0,1")>0){
                                                texto="";
                                                  if (millis() - tiempo_envio_comando_CMGF >= RETARDO_ENVIO_COMANDO_CMGF) {    
                                                      quectelSerial.println("AT+CMGF=1\r\n"); //Envia comando AT+CMGF=1
                                                      quectelSerial.available();
                                                      texto=quectelSerial.readString();
                                                      Serial.println(texto);
                                                      tiempo_envio_comando_CMGF = millis();
                                                    }
                                                              if(texto.indexOf("OK")>0){
                                                                texto="";
                                                                    if (millis() - tiempo_envio_comando_CMGS >= RETARDO_ENVIO_COMANDO_CMGS) {     
                                                                      quectelSerial.println("AT+CMGS=\"3107483273\"");
                                                                      quectelSerial.available();
                                                                      texto = quectelSerial.readString();
                                                                      Serial.println(texto);
                                                                      tiempo_envio_comando_CMGS = millis();
                                                                    }
                                                                               if(texto.indexOf(">")>0){
                                                                                texto="";
                                                                                        if (millis() - tiempo_envio_comando_TXT >= RETARDO_ENVIO_COMANDO_TXT) {
                                                                                          quectelSerial.print((Sensor_tanque_princial >= 18 && Sensor_tanque_princial <= 27) ? lista[3] : lista[2]);
                                                                                          if (Hora >= 5 && Hora <= 18){
                                                                                            quectelSerial.print((Sensor_domo > 20 && Sensor_domo < 25)  ? lista[1] : lista[0]);
                                                                                          }
                                                                                          if (Hora < 5 || Hora > 18){
                                                                                          quectelSerial.print(( Sensor_domo > 15 && Sensor_domo < 20) ? lista[1] : lista[0]);  
                                                                                          }
                                                                                          if (Hora >= HORA_ENCENDIDO && Hora <= HORA_APAGADO){
                                                                                            quectelSerial.println(( valor_fotoresistencia < 30) ? lista[4] : lista[5]);
                                                                                          }else{
                                                                                            quectelSerial.println(lista[5]);
                                                                                          }
                                                                                          quectelSerial.available();
                                                                                          tiempo_envio_comando_TXT = millis();
                                                                                        }
                                                                              }else{
                                                                        Serial.println("Error comando AT+CMGS"); 
                                                                      }     
                                                              }else{
                                                              Serial.println("Error comando AT+CMGF"); 
                                                            }
                                              }else{
                              Serial.println("Error comando AT+GREG"); 
                            }
                                               
                              }else{
                              Serial.println("Error comando AT+CPIN"); 
                            }
                }else{
                Serial.println("Error comando ATI"); 
              }
          }else{
            Serial.println("Error comando AT"); 
          }
Serial.println("<<<<<<<<<<<<<<<<<<<<<<<<Se fue el mensaje>>>>>>>>>>>>>>>>>>>>>>>>");
}
