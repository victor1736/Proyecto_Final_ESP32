
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <PubSubClient.h>

//Inicializar wifi
WiFiClient esp32Client;
PubSubClient mqttClient(esp32Client);

const char* ssid     = "DESKTOP-G34CLR6 7356";     // red wifi a la que se quiere conectar
const char* password = "cronos12345678";// contraseña
char *server = "3.87.226.41";    //ip del servidor
int port = 1883;                   // puerto del servidor
char datos1[100];
String resultS = "";
int var = 0;

// Final


TaskHandle_t task1;
TaskHandle_t task2;
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

  int lleno =  2;
  int vacio = 15;

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

//Medición nivel del tanque de reserva 
  const int PinTrig = 16;
  const int PinEcho = 4;
  int distancia = 0;


//Constante de el Gpio 12 para la lectura de los sensores de temperatura en modo parasito 
  #define ONE_WIRE_BUS 17
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire); 
  DeviceAddress sensor1 = { 0x28, 0x4D, 0xE3, 0x79, 0x97, 0x14, 0x3, 0xE9 };
  DeviceAddress sensor2 = { 0x28, 0x47, 0xCB, 0x79, 0x97, 0x11, 0x3, 0x1F };
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

  int estadoCLK;
  int estadoDT;
  int estadoSW;
  int estado_pasado_clk;
  int estado_pasado_DT;
  int estado_pasado_SW;

//parte operativa
  const int motobomba =  13;
  const int recirculacion = 12;
  const int riego = 14;
  const int Luz_ultravioleta = 27;
  unsigned long currentTime=0;
  unsigned long previousTime=0;
  unsigned long currentTime1=0;
  unsigned long previousTime1=0;
  unsigned long recirculacion_on = 5000;
  unsigned long recirculacion_off = 10000;
  unsigned long interval ;
  unsigned long interval1 ;
  bool estado_motobomba = LOW;
  bool estado_recirculacion = LOW;
  bool estado_luz_ultravioleta = LOW;

  //control ambiental del domo 
      int periodo_control_ambiental = 60000 ;
      unsigned long tiempo_ahora_control_ambiental= 0;
  
  //control de temperatura de los tanques
      int periodo_control_tanques = 1000 ;
      unsigned long tiempo_ahora_control_tanques= 0;

  //Control de luz ultravioleta
       unsigned long periodo_control_luz_ultravioleta = 5000 ;
      unsigned long tiempo_ahora_control_luz_ultravioleta= 60000;


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

//wifi
  wifiInit();
  xTaskCreatePinnedToCore(
  cliente, // Function to call
  "task2", // Name for this task, mainly for debug
  10000, // Stack size
  NULL, // pvParameters to pass to the function
  1, // Priority 
  &task2, // Task handler to use
  1 // Core where to run
  );
}

void loop() {

  Estado = Sig_Estado;
  if(Estado == 1)
  {
  
      int menu;
      String arrayMenu[] = {"Informacion", "Temperatura","Humedad","Ciclo de agua", "Luz Ultravioleta", "Nivel de Agua"};
      int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
      
      menu = menuANTIFALLOSLENTO(arrayMenu,size); // no hace falta poner el numero 6, se calcula automaticamente. Solo debes de cambiar arrayMenu
    
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
                    if( Estado == 91)
                    {
                      lcd.clear();
                      lcd.print("LCD 20x4");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 92)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Modulo:");
                      lcd.setCursor(0, 1);
                      lcd.print("ESP8266");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 93)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Motobomba:");
                      lcd.setCursor(0, 1);
                      lcd.print("wolf");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 94)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Sensor Temperatura:");
                      lcd.setCursor(0, 1);
                      lcd.print("DS18b20");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 95)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Sensor Humedad:");
                      lcd.setCursor(0, 1);
                      lcd.print("Higrometro");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 96)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Fotoresistencia:");
                      lcd.setCursor(0, 1);
                      lcd.print("Sensor de luz");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 97)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Modulo Relay:");
                      lcd.setCursor(0, 1);
                      lcd.print("4 Canales");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 98)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Botones:");
                      lcd.setCursor(0, 1);
                      lcd.print("Encoder");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 99)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Reflector:");
                      lcd.setCursor(0, 1);
                      lcd.print("Leds Ultravioleta");
                      delay(5000);
                      Sig_Estado = 1;
                    }
                    if( Estado == 100)
                    {
                      lcd.clear();
                      lcd.print("Saliendo...");
                      delay(1000);
                      Sig_Estado = 1; 
                    }
                    Estado = Sig_Estado;
            }
           if( Estado == 32)
            {
              lcd.clear();
              lcd.print("Version 1.0.0");
              delay(5000);
              Sig_Estado = 1;
            }
            if( Estado == 33)
            {
              lcd.clear();
              lcd.print("Saliendo...");
              delay(1000);
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
        if( Estado == 41)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Medio Ambiente:");
                      lcd.setCursor(0, 1);
                      lcd.print(Sensor_medio_ambiente);
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 42)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Domo:");
                      lcd.setCursor(0, 1);
                      lcd.print(Sensor_domo);
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 43)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tanque Principal:");
                      lcd.setCursor(0, 1);
                      lcd.print(Sensor_tanque_princial);
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 44)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tanque de Reserva:");
                      lcd.setCursor(0, 1);
                      lcd.print(Sensor_tanque_reserva);
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 45)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Plantas:");
                      lcd.setCursor(0, 1);
                      lcd.print(Sensor_plantas);
                      delay(5000);
                      Sig_Estado = 1;
                    }  
        if( Estado == 46)
            {
              lcd.clear();
              lcd.print("Saliendo...");
              delay(1000);
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
        if( Estado == 51)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Planta 1:");
                      lcd.setCursor(0, 1);
                      lcd.print(valor_humedad);
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado == 52)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Planta 2:");
                      lcd.setCursor(0, 1);
                      lcd.print(valor_humedad1);
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado ==53)
            {
              lcd.clear();
              lcd.print("Saliendo...");
              delay(1000);
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
                      lcd.print(recirculacion_on);
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado == 62)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tiempo de apagado:");
                      lcd.setCursor(0, 1);
                      lcd.print(recirculacion_off);
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado ==63)
            {
              lcd.clear();
              lcd.print("Saliendo...");
              delay(1000);
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
        if( Estado == 71)
                    {

                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Lectura:");
                      lcd.setCursor(0, 1);
                      lcd.print(valor_fotoresistencia);
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 72)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tiempo de Encendido:");
                      lcd.setCursor(0, 1);
                      lcd.print(periodo_control_luz_ultravioleta);
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado == 73)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tiempo de apagado:");
                      lcd.setCursor(0, 1);
                      lcd.print("lectura");
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado ==74)
            {
              lcd.clear();
              lcd.print("Saliendo...");
              delay(1000);
              Sig_Estado = 1; 
            }
        Estado = Sig_Estado;
      }
      else if( Estado == 7){
           
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
int menuANTIFALLOSLENTO(String *arrayMenu,int size)
{
//Menu
  //Vamos a marcar en que tiempo se hizo cualquier cambio y si se hizo un cambio hace muy poco tiempo y se pulso, ese cambio le damos por malo. ok?
  //Pintamos el cursor y marcamos la primera opcion
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("> ");
  float opcion = 1;  //del 1 al 1.75 Opcion 1  //Del 2  al 2.75 opcion 2
  int extraOpcion = 0;
  float incremento = 1;

  //Pinta los 4 primeros del menu como mucho
  for(int x = 0; x < size && x <= 3 ; x++) // 
  {
    lcd.setCursor(1,x);
    lcd.print(arrayMenu[x]);
  }
   delay(500);
  unsigned long tiempoCambioIncremento = 0;
  unsigned long tiempoCambioDecremento = 0;


    




  while(digitalRead(encoderSW) == 1)
  {
  //Lectura del estado actual del encoderCLK
  estadoCLK = digitalRead(encoderCLK);
  //Lectura del estado actual del DT
  estadoDT = digitalRead(encoderDT);
  //Lectura del estado actual SW
  estadoSW = digitalRead(encoderSW);
  //Convertir la lectura anterior en la actual
   //logica
      
    if (estado_pasado_clk ==LOW  && estadoCLK == HIGH){
        if (estadoDT == LOW){
        if(opcion <size)
        {
          opcion += incremento;
          tiempoCambioIncremento = millis();
          
        }

        } 
        else {
        if(opcion>1)
        {
          opcion -=incremento;
          tiempoCambioDecremento = millis();
          
        }
          }
      //Si sobrepasamos el limite por debajo
      if(opcion < 1 + extraOpcion)
        extraOpcion--;
      //Si sobrepasamos el limite por encima
      if(opcion > 4 + extraOpcion)
        extraOpcion++;

      //Pintamos de nuevo el menu
      lcd.clear();
      for(int x = extraOpcion; x < size && x <= (3+extraOpcion) ; x++)  
      {
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
//En cualquier caso no debería haberse movido ninguna tecla en 250ms?
  if(millis() - tiempoCambioIncremento < 300)
    opcion -= incremento;//Corregimos aunque no de tiempo a pintarla, ya que salimos del bucle y cambiamos de estado
  else if(millis() - tiempoCambioDecremento < 300)
    opcion += incremento;
    

  
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

//wifi
void wifiInit() { 
    Serial.print("Conectándose a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
        delay(500);  
    }
    Serial.println("");
    Serial.println("Conectado a WiFi");
    Serial.println("Dirección IP: ");
    Serial.println(WiFi.localIP());
  }

//Freedos

//cliente
void cliente(void *parameter) {
  mqttClient.setServer(server, port);
  for (;;) {
     if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  Serial.print("String: ");
  Serial.println(resultS);
  // topico de salida para el datos1 
//  sprintf(datos1,"{ "rt_temperature":%f}", Sensor_medio_ambiente   );
  mqttClient.publish("salida1", "rt_temperature ", Sensor_medio_ambiente);
  Serial.println(datos1);
  delay(5000);
    
  }
  vTaskDelay(10);
}


void actuador(void *parameter) {
//parte operativa
      pinMode(motobomba, OUTPUT);
      pinMode(recirculacion, OUTPUT);
      pinMode(riego, OUTPUT);
      digitalWrite(riego, LOW);
      pinMode(Luz_ultravioleta, OUTPUT);
      digitalWrite(Luz_ultravioleta, LOW);

      for (;;) { 
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

//       Serial.println(valor_fotoresistencia);     
      //parte operativa
      //recirculacion o alimentacion
      currentTime=millis();
      if(estado_recirculacion == LOW){
        interval=recirculacion_off;
      }else{
        interval=recirculacion_on;
      }
        if((currentTime-previousTime)>interval){
        previousTime=currentTime;
        estado_motobomba =! estado_motobomba;
        estado_recirculacion =! estado_recirculacion;
        digitalWrite(motobomba,!estado_motobomba);
        digitalWrite(recirculacion,!estado_recirculacion);
//        Serial.print(F("motobomba State : "));Serial.println(estado_motobomba);
//        Serial.print(F("recirculacion State : "));Serial.println(estado_recirculacion);
      }
//logica de bajar la temperatura en los tanques
        if (millis() > tiempo_ahora_control_tanques + periodo_control_tanques){
          tiempo_ahora_control_tanques = millis();
//          Serial.print (Sensor_tanque_princial);
        if (Sensor_tanque_princial > 27 ){
          digitalWrite(motobomba,HIGH);
          digitalWrite(recirculacion,HIGH);
//          Serial.println (" Se inicia el control de temperatura de los tanques");
          }else{
          digitalWrite(motobomba,LOW);
          digitalWrite(recirculacion,LOW);
//          Serial.println (" Se Finaliza el control de temperatura de los tanques");  
          }
      } 

//logica de control ambiental dentro del domo
      if (millis() > tiempo_ahora_control_ambiental + periodo_control_ambiental){
          tiempo_ahora_control_ambiental = millis();
//          Serial.print (Sensor_domo);
        if (Sensor_domo > 30 ){
          digitalWrite(motobomba,HIGH);
          digitalWrite(riego,HIGH);
//          Serial.println (" Se inicia el control de temperatura");
          }else{
          digitalWrite(motobomba,LOW);
          digitalWrite(riego,LOW);
//          Serial.println (" Se Finaliza el control de temperatura");  
          }
      }

   //logica de Luz ultravioleta   
        if (valor_fotoresistencia < 40){
              currentTime1=millis();
              if(estado_luz_ultravioleta == LOW){
                interval1=tiempo_ahora_control_luz_ultravioleta;
              }else{
                interval1=periodo_control_luz_ultravioleta  ;
              }
                if((currentTime1-previousTime1)>interval1){
                previousTime1=currentTime1;
                estado_luz_ultravioleta =! estado_luz_ultravioleta;
                digitalWrite(Luz_ultravioleta,!estado_luz_ultravioleta);
//                Serial.print(F("Luz ultravioleta : "));Serial.println(estado_luz_ultravioleta);

              }
        }
 
  }
  vTaskDelay(10);
}

//cliente
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");

  char payload_string[length + 1];
  
  int resultI;

  memcpy(payload_string, payload, length);
  payload_string[length] = '\0';
  resultI = atoi(payload_string);
  
  var = resultI;

  resultS = "";
  
  for (int i=0;i<length;i++) {
    resultS= resultS + (char)payload[i];
  }
  Serial.println();
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Intentando conectarse MQTT...");

    if (mqttClient.connect("arduinoClient")) {
      Serial.println("Conectado");

      mqttClient.subscribe("salida1");

           
    } else {
      Serial.print("Fallo, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" intentar de nuevo en 5 segundos");
      delay(5000); // espera 5 segundos para volver a conectar
    }
  }
}
