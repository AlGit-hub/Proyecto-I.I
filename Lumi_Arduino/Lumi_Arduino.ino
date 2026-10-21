/*
  Lumi - 3 habitaciones (sala, cocina, cuarto) controladas desde la página web

  Protocolo por USB (9600 baudios), un carácter por orden:
    'S' / 's' -> sala    encender / apagar
    'C' / 'c' -> cocina  encender / apagar
    'Q' / 'q' -> cuarto  encender / apagar
    '1' / '0' -> todas   encender / apagar

  Conexiones (cada LED con su resistencia de 220-330 ohm hacia GND):
    D4 -> resistencia -> LED de la SALA   -> GND
    D5 -> resistencia -> LED de la COCINA -> GND
    D6 -> resistencia -> LED del CUARTO   -> GND
    D13 -> LED integrado: se enciende si hay alguna habitación encendida

  RELÉ (opcional, segunda etapa): sigue el estado de la SALA.
  Está DESHABILITADO. Antes de activarlo revisa el módulo (ver instrucciones):
    - módulo: VCC -> 5V, GND -> GND, IN -> D7
    - si el módulo se activa con nivel BAJO, deja RELE_ACTIVO_EN_ALTO = false
  Úsalo solo con carga de bajo voltaje (LED de 5V/12V, tira LED, etc.)
  No conectes lámparas de la red eléctrica sin supervisión de un adulto con experiencia.
*/

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

  Serial.begin(9600);
  aplicar();  // todo apagado al arrancar
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    switch (c) {
      case 'S': sala = true;  break;
      case 's': sala = false; break;
      case 'C': cocina = true;  break;
      case 'c': cocina = false; break;
      case 'Q': cuarto = true;  break;
      case 'q': cuarto = false; break;
      case '1': sala = cocina = cuarto = true;  break;
      case '0': sala = cocina = cuarto = false; break;
      default: return;  // cualquier otro carácter se ignora
    }
    aplicar();
  }
}
