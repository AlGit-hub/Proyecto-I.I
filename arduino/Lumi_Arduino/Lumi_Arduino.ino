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

void loop() {
  while (Serial.available())    atender(Serial.read());
  while (bluetooth.available()) atender(bluetooth.read());
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
