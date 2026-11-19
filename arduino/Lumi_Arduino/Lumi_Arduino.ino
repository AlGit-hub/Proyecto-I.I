/*
  Lumi · Iluminación inteligente
  Recibe letras desde la página (USB o Bluetooth HM-10) y enciende/apaga los LED.

  Letras:   S/s = sala     C/c = cocina    Q/q = cuarto    B/b = baño
            1   = encender todas           0   = apagar todas

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

const int BT_RX = 2;    // recibe desde el TX del HM-10
const int BT_TX = 3;    // envía al RX del HM-10
SoftwareSerial bluetooth(BT_RX, BT_TX);

void setup() {
  pinMode(PIN_SALA, OUTPUT);
  pinMode(PIN_COCINA, OUTPUT);
  pinMode(PIN_CUARTO, OUTPUT);
  pinMode(PIN_BANO, OUTPUT);
  todas(LOW);

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

void atender(char c) {
  switch (c) {
    case 'S': digitalWrite(PIN_SALA, HIGH);   break;
    case 's': digitalWrite(PIN_SALA, LOW);    break;
    case 'C': digitalWrite(PIN_COCINA, HIGH); break;
    case 'c': digitalWrite(PIN_COCINA, LOW);  break;
    case 'Q': digitalWrite(PIN_CUARTO, HIGH); break;
    case 'q': digitalWrite(PIN_CUARTO, LOW);  break;
    case 'B': digitalWrite(PIN_BANO, HIGH);   break;
    case 'b': digitalWrite(PIN_BANO, LOW);    break;
    case '1': todas(HIGH);                    break;
    case '0': todas(LOW);                     break;
  }
}

void todas(int estado) {
  digitalWrite(PIN_SALA, estado);
  digitalWrite(PIN_COCINA, estado);
  digitalWrite(PIN_CUARTO, estado);
  digitalWrite(PIN_BANO, estado);
}
