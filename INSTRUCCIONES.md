# Lumi con Teachable Machine + Arduino Uno (3 habitaciones)

## Circuito (cada LED con su resistencia de 220–330 Ω hacia GND)
| Habitación | Pin Arduino | Letra encender / apagar |
|---|---|---|
| Sala | D4 | S / s |
| Cocina | D5 | C / c |
| Cuarto | D6 | Q / q |
| Todas | — | 1 / 0 |

Conexión de cada LED: pin → resistencia → pata larga (ánodo); pata corta (cátodo) → GND.
El LED integrado (D13) se enciende si hay alguna habitación encendida.

## Pasos
1. Abre `Lumi_Arduino/Lumi_Arduino.ino` en Arduino IDE, súbelo y cierra el Monitor Serial.
2. Abre la carpeta en VS Code → clic derecho en `index.html` → Open with Live Server (Chrome o Edge).
3. Conectar Arduino → prueba los botones de cada habitación.
4. Cargar modelo y escuchar → permite el micrófono → di «Oye Lumi, encender cocina», etc.

## Si el modelo no carga
Exporta el modelo en Teachable Machine (Exportar modelo → TensorFlow.js → Descargar), descomprime en una carpeta `modelo` dentro del proyecto y escribe `./modelo/` en el campo de URL.

## Relé con optoacoplador (opcional)
Sirve solo si quieres conmutar algo que consume más que un LED (una tira LED, un ventilador, una lámpara).
Para los LEDs NO es necesario.
- Módulo: VCC → 5V, GND → GND, IN → D7.
- Carga: por los terminales COM y NO (normalmente abierto).
- Muchos módulos se activan con nivel BAJO. En el .ino, `RELE_ACTIVO_EN_ALTO = false` es lo habitual; ponlo en `true` solo si tu módulo lo requiere.
- Para activarlo: `USAR_RELE = true`. El relé sigue el estado de la SALA.
- Empieza con carga de bajo voltaje (5V/12V). Nada de red eléctrica sin un adulto con experiencia.
