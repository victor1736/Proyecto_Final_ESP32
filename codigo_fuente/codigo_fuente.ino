#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <OneWire.h>
#include <DallasTemperature.h>

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

//Constante de el Gpio 12 para la lectura de los sensores de temperatura en modo parasito 
  #define ONE_WIRE_BUS 17
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire); 
  DeviceAddress sensor1 = { 0x28, 0x4D, 0xE3, 0x79, 0x97, 0x14, 0x3, 0xE9 };
  DeviceAddress sensor2 = { 0x28, 0x47, 0xCB, 0x79, 0x97, 0x11, 0x3, 0x1F };
  DeviceAddress sensor3= { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0x14, 0x77, 0xB8 };
  DeviceAddress sensor4 = { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0xE9, 0x43, 0x77 };
  DeviceAddress sensor5 = { 0x28, 0xFF, 0x64, 0x1E, 0xF, 0x77, 0x9F, 0xBC };


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

//Iniciando los sensores de temperatura
sensors.begin();
}


void loop() {
  
  
  
  Estado = Sig_Estado;
  if(Estado == 1)
  {
  
      int menu;
      String arrayMenu[] = {"Informacion", "Temperatura","Humedad","Ciclo de agua", "Luz Ultravioleta"};
      int size = sizeof(arrayMenu) / sizeof(arrayMenu[0]);
      
      menu = menuANTIFALLOSLENTO(arrayMenu,size); // no hace falta poner el numero 6, se calcula automaticamente. Solo debes de cambiar arrayMenu
    
      if(menu == -1)Sig_Estado = 1;
      else if(menu == 1)Sig_Estado = 2;
      else if(menu == 2)Sig_Estado = 3;
      else if(menu == 3)Sig_Estado = 4;
      else if(menu == 4)Sig_Estado = 5;
      else if(menu == 5)Sig_Estado = 6;

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
                      sensors.requestTemperatures();
                      Serial.print("Medio Ambiente(*C): ");
                      Serial.println(sensors.getTempC(sensor1));
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Medio Ambiente:");
                      lcd.setCursor(0, 1);
                      lcd.print(sensors.getTempC(sensor1));
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 42)
                    {
                      sensors.requestTemperatures();
                      Serial.print("Domo (*C): ");
                      Serial.println(sensors.getTempC(sensor2));
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Domo:");
                      lcd.setCursor(0, 1);
                      lcd.print(sensors.getTempC(sensor2));
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 43)
                    {
                      sensors.requestTemperatures();
                      Serial.print("Tanque Principal (*C): ");
                      Serial.println(sensors.getTempC(sensor3));
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tanque Principal:");
                      lcd.setCursor(0, 1);
                      lcd.print(sensors.getTempC(sensor3));
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 44)
                    {
                      sensors.requestTemperatures();
                      Serial.print("Tanque de Reserva(*C): ");
                      Serial.println(sensors.getTempC(sensor4));
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tanque de Reserva:");
                      lcd.setCursor(0, 1);
                      lcd.print(sensors.getTempC(sensor4));
                      delay(5000);
                      Sig_Estado = 1;
                    }
        if( Estado == 45)
                    {
                      sensors.requestTemperatures();
                      Serial.print("Plantas (*C): ");
                      Serial.println(sensors.getTempC(sensor5));
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Plantas:");
                      lcd.setCursor(0, 1);
                      lcd.print(sensors.getTempC(sensor5));
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
                      valor_humedad = analogRead(humedad);
                      Serial.print(valor_humedad);
                      valor_humedad =map(valor_humedad, 4095 , 1300, 0, 100);
                      Serial.print("planta 1: ");
                      Serial.println(valor_humedad);
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
                      valor_humedad1 = analogRead(humedad1);
                      Serial.print(valor_humedad1);
                      valor_humedad1 = map(valor_humedad1, 4095 , 1300, 0, 100);
                      Serial.print("planta 2: ");
                      Serial.println(valor_humedad1);
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
                      lcd.print("lectura");
                      delay(5000);
                      Sig_Estado = 1;
                    } 
        if( Estado == 62)
                    {
                      lcd.clear();
                      lcd.setCursor(0, 0);
                      lcd.print("Tiempo de apagado:");
                      lcd.setCursor(0, 1);
                      lcd.print("lectura");
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
                      valor_fotoresistencia = analogRead(fotoresistencia);
                      Serial.println(valor_fotoresistencia);
                      valor_fotoresistencia =map(valor_fotoresistencia, 4095 , 0, 0, 100);
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
                      lcd.print("lectura");
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
