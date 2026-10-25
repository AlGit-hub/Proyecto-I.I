/*
  Lumi - 3 habitaciones (sala, cocina, cuarto) controladas desde la página web
  Las órdenes llegan por USB (Web Serial) o por Bluetooth BLE (módulo HM-10).

  Protocolo (9600 baudios), un carácter por orden:
    'S' / 's' -> sala    encender / apagar
    'C' / 'c' -> cocina  encender / apagar
    'Q' / 'q' -> cuarto  encender / apagar
    '1' / '0' -> todas   encender / apagar

  Conexiones (cada LED con su resistencia de 220-330 ohm hacia GND):
    D4 -> resistencia -> LED de la SALA   -> GND
    D5 -> resistencia -> LED de la COCINA -> GND
    D6 -> resistencia -> LED del CUARTO   -> GND
    D13 -> LED integrado: se enciende si hay alguna habitación encendida

  Módulo HM-10 (Bluetooth BLE):
    VCC  -> 5V
    GND  -> GND
    TXD  -> D2 (directo)
    RXD  <- D3, pasando por un divisor de voltaje:
            D3 -> 1 kOhm -> RXD del HM-10 -> 2 kOhm -> GND
    Los pines 0 y 1 quedan libres para el USB.

  RELÉ (opcional, segunda etapa): sigue el estado de la SALA.
  Está DESHABILITADO. Antes de activarlo revisa el módulo:
    - módulo: VCC -> 5V, GND -> GND, IN -> D7
    - si el módulo se activa con nivel BAJO, deja RELE_ACTIVO_EN_ALTO = false
  Úsalo solo con carga de bajo voltaje (LED de 5V/12V, tira LED, etc.)
  No conectes lámparas de la red eléctrica sin supervisión de un adulto con experiencia.
*/

#include <SoftwareSerial.h>

const int PIN_BT_RX = 2;  // el Arduino recibe aquí (va al TXD del HM-10)
const int PIN_BT_TX = 3;  // el Arduino envía aquí (va al RXD del HM-10, con divisor)
SoftwareSerial bluetooth(PIN_BT_RX, PIN_BT_TX);

const int PIN_SALA = 4;
const int PIN_COCINA = 5;
const int PIN_CUARTO = 6;
const int PIN_INTEGRADO = LED_BUILTIN;  // D13

const bool USAR_RELE = false;
const int PIN_RELE = 7;
const bool RELE_ACTIVO_EN_ALTO = false;  // la mayoría de módulos con optoacoplador se activan con LOW

bool sala = false, cocina = false, cuarto = false;

void escribirRele(bool encendido) {
  if (!USAR_RELE) return;
  bool nivel = RELE_ACTIVO_EN_ALTO ? encendido : !encendido;
  digitalWrite(PIN_RELE, nivel ? HIGH : LOW);
}

void aplicar() {
  digitalWrite(PIN_SALA, sala ? HIGH : LOW);
  digitalWrite(PIN_COCINA, cocina ? HIGH : LOW);
  digitalWrite(PIN_CUARTO, cuarto ? HIGH : LOW);
  digitalWrite(PIN_INTEGRADO, (sala || cocina || cuarto) ? HIGH : LOW);
  escribirRele(sala);

  // Informe de estado (se ve en el Monitor Serial)
  Serial.print("sala=");   Serial.print(sala);
  Serial.print(" cocina="); Serial.print(cocina);
  Serial.print(" cuarto="); Serial.println(cuarto);
}

// Interpreta un carácter. Devuelve true si era una orden válida.
bool procesar(char c) {
  switch (c) {
    case 'S': sala = true;  break;
    case 's': sala = false; break;
    case 'C': cocina = true;  break;
    case 'c': cocina = false; break;
    case 'Q': cuarto = true;  break;
    case 'q': cuarto = false; break;
    case '1': sala = cocina = cuarto = true;  break;
    case '0': sala = cocina = cuarto = false; break;
    default: return false;  // cualquier otro carácter (saltos de línea, etc.) se ignora
  }
  return true;
}

void setup() {
  pinMode(PIN_SALA, OUTPUT);
  pinMode(PIN_COCINA, OUTPUT);
  pinMode(PIN_CUARTO, OUTPUT);
  pinMode(PIN_INTEGRADO, OUTPUT);

  if (USAR_RELE) {
    // Primero fijamos el nivel "apagado" y después activamos el pin, para evitar un pulso al arrancar
    escribirRele(false);
    pinMode(PIN_RELE, OUTPUT);
    escribirRele(false);
  }

  Serial.begin(9600);     // USB
  bluetooth.begin(9600);  // HM-10 (viene de fábrica a 9600)
  aplicar();  // todo apagado al arrancar
}

void loop() {
  bool cambio = false;

  while (Serial.available() > 0) {     // órdenes por USB
    if (procesar(Serial.read())) cambio = true;
  }
  while (bluetooth.available() > 0) {  // órdenes por Bluetooth
    if (procesar(bluetooth.read())) cambio = true;
  }

  if (cambio) aplicar();
}
