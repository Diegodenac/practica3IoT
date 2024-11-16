const int pirPin = 14;  // Pin donde está conectado el PIR
const int ledPin = 12;  // Pin donde está conectado el LED

unsigned long tiempoActivo = 0;      // Tiempo cuando el sensor detecta movimiento
unsigned long tiempoInactivo = 0;    // Tiempo cuando el sensor no detecta movimiento
unsigned long inicioTiempo = 0;      // Tiempo de inicio para medir cada estado
bool estadoAnterior = LOW;           // Estado anterior del PIR para detectar cambios

void setup() {
  pinMode(pirPin, INPUT);   // Configura el pin del PIR como entrada
  pinMode(ledPin, OUTPUT);  // Configura el pin del LED como salida
  Serial.begin(9600);       // Inicia la comunicación serie para monitoreo
  inicioTiempo = millis();  // Establece el tiempo inicial
}

void loop() {
  int pirState = digitalRead(pirPin); // Lee el estado del sensor PIR

  if (pirState == HIGH) {             // Si el sensor detecta movimiento
    digitalWrite(ledPin, HIGH);       // Enciende el LED

    if (estadoAnterior == LOW) {      // Detecta cambio de inactivo a activo
      // Calcula y muestra el tiempo en estado inactivo en segundos antes de cambiar a activo
      tiempoInactivo = millis() - inicioTiempo;
      //Serial.print("Tiempo inactivo: ");
      Serial.println(tiempoInactivo / 1000.0, 3);  // Convierte a segundos con 3 decimales
      //Serial.println(" s");

      // Reinicia el contador de tiempo activo e inicia la nueva medición
      inicioTiempo = millis();  // Reinicia el tiempo de inicio
      tiempoActivo = 0;         // Resetea el tiempo activo
    }

  } else {                            // Si no hay movimiento
    digitalWrite(ledPin, LOW);        // Apaga el LED

    if (estadoAnterior == HIGH) {     // Detecta cambio de activo a inactivo
      // Calcula y muestra el tiempo en estado activo en segundos antes de cambiar a inactivo
      tiempoActivo = millis() - inicioTiempo;
      /*Serial.print("Tiempo activo: ");
      Serial.print(tiempoActivo / 1000.0, 3);  // Convierte a segundos con 3 decimales
      Serial.println(" s");*/

      // Reinicia el contador de tiempo inactivo e inicia la nueva medición
      inicioTiempo = millis();  // Reinicia el tiempo de inicio
      tiempoInactivo = 0;       // Resetea el tiempo inactivo
    }
  }

  estadoAnterior = pirState;  // Actualiza el estado anterior
  delay(10);                  // Pequeño retardo para evitar lecturas erráticas
}
