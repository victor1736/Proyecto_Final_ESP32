const int SENSOR_PIN = 12; // Medidor de pH salida analógica a entrada analógica ESP32 D12
const float OFFSET = 0.00; // Desviación de compensación

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  float phValue = 0;
  for (int i = 0; i < 50; i++) { // Obtener 50 valores de muestra del sensor
    phValue += analogRead(SENSOR_PIN);
    delay(20); // Esperar 20ms entre cada lectura
  }
  phValue /= 50.0; // Calcular el promedio de las 50 muestras
  delay(1950); // Esperar 2 segundos para promediar más tiempo
  for (int i = 0; i < 50; i++) { // Obtener 50 valores de muestra del sensor
    phValue += analogRead(SENSOR_PIN);
    delay(20); // Esperar 20ms entre cada lectura
  }
  phValue /= 100.0; // Calcular el promedio de las 100 muestras
  phValue = 3.5 * (phValue * 3.3 / 4095.0) + OFFSET; // Convertir la lectura a un valor de pH con referencia de 3.3V
  Serial.print("pH: ");
  Serial.println(phValue, 2); // Imprimir el valor de pH con 2 decimales
  delay(800);
}