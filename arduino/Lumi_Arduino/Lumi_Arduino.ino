/*
  Lumi · Iluminación inteligente
  Recibe letras desde la página (USB o Bluetooth HM-10), enciende/apaga los LED
  y RESPONDE con el estado real de cada luz para que la página lo confirme.

  Letras:   S/s = sala     C/c = cocina    Q/q = cuarto    B/b = baño
            1   = encender todas           0   = apagar todas
            ?   = informar el estado de todas las luces
  Respuestas (una línea por habitación): S1 S0 C1 C0 Q1 Q0 B1 B0

  Conexiones:
    LED sala   -> D4   (con resistencia de 220 ohm a GND)
    LED cocina -> D5
    LED cuarto -> D6
    LED baño   -> D7
    HM-10: TX -> D2,  RX -> D3 (con divisor de voltaje), VCC -> 5V, GND -> GND
*/

#include <SoftwareSerial.h>

const int PIN_SALA   = 4;
const int PIN_COCINA = 5;
const int PIN_CUARTO = 6;
const int PIN_BANO   = 7;

const int  PINES[4]  = {PIN_SALA, PIN_COCINA, PIN_CUARTO, PIN_BANO};
const char LETRAS[4] = {'S', 'C', 'Q', 'B'};

const int BT_RX = 2;    // recibe desde el TX del HM-10
const int BT_TX = 3;    // envía al RX del HM-10
SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(PINES[i], OUTPUT);
    digitalWrite(PINES[i], LOW);
  }
  Serial.begin(9600);      // USB (la página usa 9600)
  bluetooth.begin(9600);   // HM-10
}

// El HM-10 avisa por su cuenta «OK+CONN» al conectarse y «OK+LOST» al desconectarse.
// Esas letras (C, S...) se confundirían con órdenes, así que se descartan.
int saltar = 0;          // letras que faltan por descartar tras «OK+»
int pasoAviso = 0;       // 0 = nada, 1 = vimos «O», 2 = vimos «OK»

void loop() {
  while (Serial.available())    atender(Serial.read());
  while (bluetooth.available()) atenderBluetooth(bluetooth.read());
}

void atenderBluetooth(char c) {
  if (saltar > 0) { saltar--; return; }
  if (pasoAviso == 0 && c == 'O') { pasoAviso = 1; return; }
  if (pasoAviso == 1) { pasoAviso = (c == 'K') ? 2 : 0; if (pasoAviso == 2) return; }
  if (pasoAviso == 2) { pasoAviso = 0; if (c == '+') { saltar = 4; return; } }
  atender(c);
}

// Cuenta a la página (por USB y por Bluetooth) cómo quedó una luz: por ejemplo «C1»
void informar(int i) {
  char m[3] = { LETRAS[i], (digitalRead(PINES[i]) == HIGH) ? '1' : '0', '\n' };
  Serial.write(m, 3);
  bluetooth.write(m, 3);
}

void poner(int i, int estado) {
  digitalWrite(PINES[i], estado);
  informar(i);
}

void atender(char c) {
  switch (c) {
    case 'S': poner(0, HIGH); break;
    case 's': poner(0, LOW);  break;
    case 'C': poner(1, HIGH); break;
    case 'c': poner(1, LOW);  break;
    case 'Q': poner(2, HIGH); break;
    case 'q': poner(2, LOW);  break;
    case 'B': poner(3, HIGH); break;
    case 'b': poner(3, LOW);  break;
    case '1': todas(HIGH);    break;
    case '0': todas(LOW);     break;
    case '?': for (int i = 0; i < 4; i++) informar(i); break;
  }
}

void todas(int estado) {
  for (int i = 0; i < 4; i++) poner(i, estado);
}
