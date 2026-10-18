import tkinter as tk
from tkinter import messagebox

root = tk.Tk()
root.title("Instrumento de medición")
root.geometry("400x280")

etiqutav = tk.Label(root, text="Voltaje (V)")
etiqutav.grid(row=0, column=0, padx=10, pady=10) # grid() organiza los elementos en filas y columnas, como una tabla.

campo_v = tk.Entry(root)
campo_v.grid(row=0, column=1, padx=10, pady=10)

etiqutai = tk.Label(root, text="Correinte (I)")
etiqutai.grid(row=1, column=0, padx=10, pady=10) # grid() organiza los elementos en filas y columnas, como una tabla.

campo_i = tk.Entry(root)
campo_i.grid(row=1, column=1, padx=10, pady=10)

# 1. Función para calcular la potencia
def calcular():
    try:
        # Obtener los valores escritos
        V = float(campo_v.get())
        I = float(campo_i.get())

        # Calcular la potencia
        P = V * I

        # Mostrar el resultado
        #resultado.config(text=f"Potencia: {P:.3f} mW")
        resultado.config(text=f"Potencia: {P:.1f} W")

    except ValueError:
        messagebox.showerror(
            "Error",
            "Ingresa valores numéricos válidos."
        )

boton = tk.Button(root, text="Calcular Potencia", command=calcular)
boton.grid(row=2, column=0, columnspan=2, pady=10)

# Regla importante: evita mezclar pack() y grid() dentro de un mismo contenedor. Puedes utilizar ambos en una aplicación, pero en contenedores distintos.
# 6. Etiqueta para mostrar el resultado
resultado = tk.Label(root, text="Potencia: -- W", font=("Arial", 12, "bold"))
resultado.grid(row=3, column=0, columnspan=2, pady=10)

    
root.mainloop()