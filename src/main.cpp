#include "Matrix2D.hpp" // Incluye la clase Matrix2D para representar la matriz de coeficientes del sistema lineal.
#include "SystemAnalyzer.hpp" // Incluye la clase SystemAnalyzer para clasificar la estabilidad y el tipo de punto critico.
#include "TrajectoryExporter.hpp" // Incluye la clase TrajectoryExporter para integrar numéricamente y exportar las trayectorias.
#include <iostream> // Incluye la librería de entrada y salida estándar para interactuar con el usuario a través de la consola.
#include <cstdlib> // Incluye cstdlib para habilitar la función std::system y ejecutar el script de Python.
#include <string> // Incluye la librería de cadenas de texto para manipular rutas de archivos e instrucciones del sistema.

int main() { // Función principal que coordina el flujo global de ejecución del programa.
    std::cout << "========================================================\n"; // Imprime una línea divisoria superior en la consola.
    std::cout << "  ANALISIS DE SISTEMAS DINAMICOS LINEALES 2D (0,0)      \n"; // Muestra el título principal del programa de análisis.
    std::cout << "========================================================\n\n"; // Imprime la línea divisoria inferior con un salto de línea.

    double a1, b1, a2, b2; // Declara las variables para almacenar los coeficientes a1, b1, a2, b2 del sistema lineal.

    std::cout << "Ingrese los coeficientes del sistema lineal 2D:\n"; // Solicita al usuario ingresar los valores del sistema x' = A*x.
    std::cout << "Ecuacion 1: x' = a1*x + b1*y\n"; // Muestra la forma general de la primera ecuación diferencial del sistema.
    std::cout << "  a1: "; // Solicita el ingreso del coeficiente a1.
    std::cin >> a1; // Captura desde la consola el valor numérico asignado a a1.
    std::cout << "  b1: "; // Solicita el ingreso del coeficiente b1.
    std::cin >> b1; // Captura desde la consola el valor numérico asignado a b1.

    std::cout << "Ecuacion 2: y' = a2*x + b2*y\n"; // Muestra la forma general de la segunda ecuación diferencial del sistema.
    std::cout << "  a2: "; // Solicita el ingreso del coeficiente a2.
    std::cin >> a2; // Captura desde la consola el valor numérico asignado a a2.
    std::cout << "  b2: "; // Solicita el ingreso del coeficiente b2.
    std::cin >> b2; // Captura desde la consola el valor numérico asignado a b2.
    std::cout << "\n"; // Añade un espacio en blanco para separar la entrada de datos de los resultados.

    Matrix2D systemMatrix(a1, b1, a2, b2); // Instancia el objeto Matrix2D con los coeficientes a1, b1, a2, b2 ingresados.

    SystemAnalyzer analyzer; // Instancia el objeto SystemAnalyzer para procesar las propiedades cualitativas del sistema.
    AnalysisResult result = analyzer.analyze(systemMatrix); // Realiza el análisis analítico de unicidad, estabilidad y tipo de punto crítico.

    analyzer.printReport(result); // Imprime en pantalla el informe completo formateado con el resultado del análisis.

    std::string csvFilename = "trajectories.csv"; // Define el nombre del archivo CSV donde se guardarán los datos de la simulación.
    TrajectoryExporter exporter; // Instancia el objeto TrajectoryExporter para integrar las trayectorias en el plano de fase.

    std::cout << "\nGenerando datos de simulación numérica (RK4)...\n"; // Notifica al usuario el inicio del proceso de simulación numérica.
    bool exportSuccess = exporter.exportSimulationData(systemMatrix, csvFilename); // Ejecuta el método que simula y escribe las trayectorias en el archivo CSV.

    if (exportSuccess) { // Evalúa si la exportación de los datos de trayectorias a CSV se realizó con éxito.
        std::cout << "\nLlamando al script de Python para visualización gráfica...\n"; // Notifica el inicio de la herramienta gráfica en Python.
        
        std::string command = "python scripts/plot_phase_portrait.py " + csvFilename; // Construye el comando de consola para invocar el script de Python.
        int sysResult = std::system(command.c_str()); // Ejecuta el comando de sistema para generar el gráfico del retrato de fase.

        if (sysResult != 0) { // Evalúa si el primer comando con 'python' falló en entornos que requieren 'python3'.
            std::string commandPython3 = "python3 scripts/plot_phase_portrait.py " + csvFilename; // Construye la alternativa del comando usando 'python3'.
            sysResult = std::system(commandPython3.c_str()); // Ejecuta la llamada alternativa de sistema.
        } // Fin de la comprobación del comando python/python3.

        if (sysResult != 0) { // Verifica si el script gráfico no pudo ejecutarse en ninguno de los intérpretes.
            std::cout << "Nota: No se pudo ejecutar el script de Python automáticamente.\n"; // Informa al usuario sobre la limitación de ejecución.
            std::cout << "Puede generar la gráfica manualmente ejecutando:\n"; // Proporciona la instrucción de ejecución manual.
            std::cout << "  python scripts/plot_phase_portrait.py " << csvFilename << "\n"; // Muestra la sintaxis exacta de la orden en consola.
        } // Fin del bloque de advertencia sobre la ejecución de Python.
    } else { // Caso en que ocurrió un error al escribir los datos en el archivo CSV.
        std::cerr << "Error crítico: Ocurrió un fallo al exportar los datos de simulación.\n"; // Imprime un mensaje de fallo en la consola de errores.
    } // Fin del bloque de control de la exportación.

    std::cout << "\nEjecución finalizada exitosamente.\n"; // Notifica la culminación del programa completo.
    return 0; // Retorna 0 para indicar que el programa finalizó de manera limpia sin errores.
} // Fin de la función principal main.