"""
Monitor de Mediciones Electrónicas
----------------------------------
Proyecto integrador: interfaz gráfica en Tkinter (arquitectura orientada a eventos).

Metáfora hardware -> software:
    Window   = protoboard digital (la base del circuito)
    Label    = display LCD (salida de datos)
    Entry    = sensor de ingreso (entrada de datos, llega como texto)
    Button   = actuador (cierra el circuito al hacer clic)
    Callback = cable de cobre (conecta la interfaz con la lógica)
"""

import tkinter as tk

# ---------------------------------------------------------------
# Configuración
# ---------------------------------------------------------------
UMBRAL_POTENCIA_W = 50.0   # Por encima de este valor se considera sobrecarga

COLOR_FONDO = "#1e252b"
COLOR_TEXTO = "#e8eef2"
COLOR_SEGURO = "#2ecc71"
COLOR_PELIGRO = "#ff4d3d"
COLOR_NEUTRO = "#8a97a3"


# ---------------------------------------------------------------
# Lógica (el callback del botón)
# ---------------------------------------------------------------
def calcular_potencia():
    """Se ejecuta SOLO cuando el usuario presiona el botón (o la tecla Enter)."""
    try:
        # 1. Extracción: capturar la señal cruda (texto) de los Entry
        v_str = entrada_v.get().strip().replace(",", ".")
        i_str = entrada_i.get().strip().replace(",", ".")

        # 2. Type casting: texto -> número flotante
        v_float = float(v_str)
        i_float = float(i_str)

        # Validación extra: valores físicamente razonables para este monitor
        if v_float < 0 or i_float < 0:
            raise ValueError("Valores negativos")

        # 3. Operación: ley de Watt
        potencia = v_float * i_float

        # 4. Actualización: enviar la señal procesada a la pantalla
        etiqueta_resultado.config(text=f"{potencia:.2f} W", fg=COLOR_TEXTO)

        # 5. Lógica de seguridad (if/else + .config() = retroalimentación visual)
        if potencia <= UMBRAL_POTENCIA_W:
            etiqueta_estado.config(fg=COLOR_SEGURO, text="● ESTADO: SEGURO")
        else:
            etiqueta_estado.config(fg=COLOR_PELIGRO, text="● ¡PELIGRO: SOBRECARGA!")

    except ValueError:
        # Red de captura de errores: evita que el sistema colapse
        etiqueta_resultado.config(text="-- W", fg=COLOR_NEUTRO)
        etiqueta_estado.config(fg=COLOR_PELIGRO, text="Error: ingrese solo números positivos")


def limpiar():
    """Reinicia la estación a su estado inicial."""
    entrada_v.delete(0, tk.END)
    entrada_i.delete(0, tk.END)
    etiqueta_resultado.config(text="0.00 W", fg=COLOR_TEXTO)
    etiqueta_estado.config(fg=COLOR_NEUTRO, text="● ESTADO: EN ESPERA")
    entrada_v.focus()


# ---------------------------------------------------------------
# Interfaz: la protoboard digital
# ---------------------------------------------------------------
root = tk.Tk()
root.title("Monitor de Mediciones Electrónicas")
root.configure(bg=COLOR_FONDO, padx=24, pady=20)
root.resizable(False, False)

fuente_normal = ("Consolas", 12)
fuente_titulo = ("Consolas", 15, "bold")
fuente_display = ("Consolas", 30, "bold")

# Título (fila 0)
tk.Label(root, text="Monitor de Mediciones Electrónicas",
         font=fuente_titulo, bg=COLOR_FONDO, fg=COLOR_TEXTO
         ).grid(row=0, column=0, columnspan=2, pady=(0, 16))

# Sensores de ingreso (filas 1 y 2): Label + Entry lado a lado con grid()
tk.Label(root, text="Voltaje (V):", font=fuente_normal,
         bg=COLOR_FONDO, fg=COLOR_TEXTO).grid(row=1, column=0, sticky="e", padx=8, pady=6)
entrada_v = tk.Entry(root, font=fuente_normal, width=12, justify="center")
entrada_v.grid(row=1, column=1, padx=8, pady=6)

tk.Label(root, text="Corriente (A):", font=fuente_normal,
         bg=COLOR_FONDO, fg=COLOR_TEXTO).grid(row=2, column=0, sticky="e", padx=8, pady=6)
entrada_i = tk.Entry(root, font=fuente_normal, width=12, justify="center")
entrada_i.grid(row=2, column=1, padx=8, pady=6)

# Actuadores (fila 3): command= recibe la función SIN paréntesis
tk.Button(root, text="Calcular Potencia", font=fuente_normal,
          command=calcular_potencia, bg="#2f6f9f", fg="white",
          activebackground="#3d8bc4", activeforeground="white", width=18
          ).grid(row=3, column=0, columnspan=2, pady=(14, 4))

tk.Button(root, text="Limpiar", font=("Consolas", 10),
          command=limpiar, bg="#3a444d", fg=COLOR_TEXTO,
          activebackground="#4b5863", activeforeground="white", width=10
          ).grid(row=4, column=0, columnspan=2, pady=(0, 10))

# Display de potencia (fila 5)
tk.Label(root, text="Potencia (P = V × I):", font=fuente_normal,
         bg=COLOR_FONDO, fg=COLOR_NEUTRO).grid(row=5, column=0, columnspan=2)
etiqueta_resultado = tk.Label(root, text="0.00 W", font=fuente_display,
                              bg="#10151a", fg=COLOR_TEXTO, width=14, pady=8)
etiqueta_resultado.grid(row=6, column=0, columnspan=2, pady=(2, 10))

# Estado del sistema (fila 7)
etiqueta_estado = tk.Label(root, text="● ESTADO: EN ESPERA", font=fuente_normal,
                           bg=COLOR_FONDO, fg=COLOR_NEUTRO)
etiqueta_estado.grid(row=7, column=0, columnspan=2)

# Atajo: presionar Enter también "cierra el circuito"
root.bind("<Return>", lambda evento: calcular_potencia())

entrada_v.focus()

# Energizar el sistema: el programa espera pasivamente las interacciones
root.mainloop()