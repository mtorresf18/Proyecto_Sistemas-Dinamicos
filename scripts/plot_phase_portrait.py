import os  # Importa el módulo del sistema operativo para verificar la existencia de archivos en disco.
import sys  # Importa la librería sys para capturar argumentos pasados desde la terminal de comandos.
import matplotlib.pyplot as plt  # Importa el módulo pyplot de matplotlib para la generación y renderizado de gráficos 2D.
import numpy as np  # Importa numpy para operaciones vectoriales y soporte matemático de mallas.
import pandas as pd  # Importa pandas para la lectura y procesamiento estructurado del archivo CSV exportado por C++.


def main():  # Define la función principal que coordina la carga de datos y el renderizado del retrato de fase.
  csv_file = (
      'trajectories.csv'  # Asigna el nombre de archivo por defecto generado por el exportador de C++.
  )
  if (
      len(sys.argv) > 1
  ):  # Evalúa si el usuario proporcionó una ruta de archivo personalizada mediante la línea de comandos.
    csv_file = sys.argv[
        1
    ]  # Asigna el primer argumento recibido como la nueva ruta del archivo CSV a procesar.

  if not os.path.exists(
      csv_file
  ):  # Comprueba si el archivo CSV de datos numéricos realmente existe en la ruta especificada.
    print(
        f'Error: El archivo {csv_file} no existe. Generelo ejecutando el'
        ' programa C++ primero.'
    )  # Muestra un mensaje de alerta en consola si no encuentra los datos.
    sys.exit(
        1
    )  # Finaliza inmediatamente la ejecución del script Python con código de error.

  df = pd.read_csv(
      csv_file
  )  # Lee las columnas (trajectory_id, t, x, y) del CSV y las almacena en un DataFrame de pandas.

  fig, ax = plt.subplots(
      figsize=(8, 8)
  )  # Inicializa la figura y los ejes de graficación fijando un tamaño cuadrado de 8x8 pulgadas.

  trajectory_ids = df[
      'trajectory_id'
  ].unique()  # Extrae la lista de identificadores únicos para cada trayectoria integrada por RK4.

  for traj_id in (
      trajectory_ids
  ):  # Itera a través de cada trayectoria independiente generada a partir de las condiciones iniciales.
    traj_data = df[
        df['trajectory_id'] == traj_id
    ]  # Obten los puntos ordenados en el tiempo correspondientes a la trayectoria actual.
    x_vals = traj_data[
        'x'
    ].values  # Extrae el vector de estados x(t) a lo largo del tiempo de simulación.
    y_vals = traj_data[
        'y'
    ].values  # Extrae el vector de estados y(t) a lo largo del tiempo de simulación.

    ax.plot(
        x_vals, y_vals, color='blue', alpha=0.35, linewidth=1.1
    )  # Grafica la trayectoria continua en el plano de fase (x, y) con transparencia azul.

    if (
        len(x_vals) > 6
    ):  # Comprueba que la trayectoria posea suficientes pasos integrados para colocar una flecha de flujo.
      mid_idx = len(x_vals) // 2  # Obtiene el índice central de la secuencia de puntos de la trayectoria.
      ax.annotate(
          '',  # Define una anotación sin texto para dibujar únicamente el indicador de dirección.
          xy=(
              x_vals[mid_idx],
              y_vals[mid_idx],
          ),  # Define las coordenadas de destino para la cabeza de la flecha direccional.
          xytext=(
              x_vals[mid_idx - 1],
              y_vals[mid_idx - 1],
          ),  # Define las coordenadas de origen basadas en el punto temporal anterior.
          arrowprops=dict(
              arrowstyle='->', color='darkblue', lw=1.2
          ),  # Aplica el estilo visual de la flecha que indica el avance del tiempo t.
      )  # Finaliza la incorporación de la flecha de sentido de trayectoria.

  ax.axhline(
      0, color='black', linewidth=0.8, linestyle='--'
  )  # Dibuja la línea recta del eje horizontal x = 0.
  ax.axvline(
      0, color='black', linewidth=0.8, linestyle='--'
  )  # Dibuja la línea recta del eje vertical y = 0.

  ax.plot(
      0, 0, 'ro', markersize=8, label='Punto Critico (0,0)'
  )  # Resalta geométricamente el punto crítico de equilibrio (0,0) mediante un marcador rojo.

  ax.set_xlabel(
      'Estado x(t)', fontsize=12
  )  # Asigna el título informativo al eje horizontal representando la variable x.
  ax.set_ylabel(
      'Estado y(t)', fontsize=12
  )  # Asigna el título informativo al eje vertical representando la variable y.
  ax.set_title(
      'Retrato de Fase Alrededor del Punto Critico (0,0)', fontsize=14
  )  # Establece el encabezado del gráfico del sistema dinámico.

  ax.grid(
      True, linestyle=':', alpha=0.6
  )  # Habilita la cuadrícula de referencia espacial con líneas punteadas suaves.
  ax.legend(
      loc='upper right'
  )  # Posiciona la leyenda del gráfico en la esquina superior derecha.
  ax.set_aspect(
      'equal', adjustable='box'
  )  # Mantiene una proporción escalar idéntica 1:1 entre los ejes x e y para no distorsionar las órbitas.

  output_image = (
      'phase_portrait.png'  # Establece el nombre del archivo de imagen de salida.
  )
  plt.savefig(
      output_image, dpi=300, bbox_inches='tight'
  )  # Exporta la figura a un archivo de alta resolución (300 DPI) recortando márgenes.
  print(
      f'Grafico del retrato de fase guardado exitosamente como {output_image}'
  )  # Emite el mensaje de confirmación de guardado en la consola.

  plt.show()  # Despliega la interfaz gráfica interactiva con el gráfico del plano de fase.


if (
    __name__ == '__main__'
):  # Punto de entrada estándar para ejecutar la función main cuando el script es invocado directamente.
  main()  # Llama a la función principal del programa.