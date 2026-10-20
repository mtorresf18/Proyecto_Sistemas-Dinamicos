#include "Matrix2D.hpp" // Incluye la clase Matrix2D para el análisis de la matriz de coeficientes.
#include "NonLinearSystem.hpp" // Incluye la clase NonLinearSystem para abstraer el sistema no lineal y su Jacobiano.
#include "SystemAnalyzer.hpp" // Incluye SystemAnalyzer para el diagnóstico cualitativo y aplicación del Teorema de Hartman-Grobman.
#include "TrajectoryExporter.hpp" // Incluye TrajectoryExporter para la simulación numérica RK4 y exportación a CSV.
#include <iostream> // Incluye la librería estándar para la interacción mediante la consola.
#include <cstdlib> // Incluye cstdlib para habilitar llamadas a comandos del sistema mediante std::system.
#include <string> // Incluye la librería de cadenas de texto para manipular rutas de archivos e instrucciones.

int main() { // Función principal que coordina el flujo global de ejecución del programa.
    std::cout << "========================================================\n"; // Imprime la línea divisoria superior.
    std::cout << "  ANALISIS DE SISTEMAS DINAMICOS (LINEALES Y NO LINEALES)\n"; // Imprime el título del programa.
    std::cout << "========================================================\n\n"; // Imprime la línea divisoria inferior.

    int option = 0; // Declara la variable para almacenar la selección del menú elegida por el usuario.
    std::cout << "Seleccione el tipo de sistema a evaluar:\n"; // Solicita la elección del tipo de sistema.
    std::cout << "  1. Sistema Lineal   (x' = a1*x + b1*y,  y' = a2*x + b2*y)\n"; // Muestra la opción de sistema lineal.
    std::cout << "  2. Sistema No Lineal (x' = f(x,y),       y' = g(x,y))\n"; // Muestra la opción de sistema no lineal.
    std::cout << "Opcion (1 o 2): "; // Solicita el ingreso de la opción por consola.
    std::cin >> option; // Captura la opción seleccionada.
    std::cout << "\n"; // Añade un salto de línea.

    SystemAnalyzer analyzer; // Instancia el objeto clasificador cualitativo de la dinámica.
    TrajectoryExporter exporter; // Instancia el exportador y simulador numérico mediante Runge-Kutta 4.
    std::string csvFilename = "trajectories.csv"; // Define el nombre estándar del archivo CSV de salida.
    bool exportSuccess = false; // Bandera de control para verificar si la exportación a CSV fue exitosa.

    if (option == 1) { // Caso 1: El usuario seleccionó analizar un sistema lineal.
        double a1, b1, a2, b2; // Declara las variables para los coeficientes de la matriz del sistema lineal.
        std::cout << "--- Ingrese coeficientes del Sistema Lineal ---\n"; // Título de la sección lineal.
        std::cout << "  a1: "; std::cin >> a1; // Solicita y captura a1.
        std::cout << "  b1: "; std::cin >> b1; // Solicita y captura b1.
        std::cout << "  a2: "; std::cin >> a2; // Solicita y captura a2.
        std::cout << "  b2: "; std::cin >> b2; // Solicita y captura b2.
        std::cout << "\n"; // Salto de línea decorativo.

        Matrix2D systemMatrix(a1, b1, a2, b2); // Instancia la matriz de 2x2 asociada al sistema lineal.
        AnalysisResult result = analyzer.analyze(systemMatrix); // Realiza el análisis analítico de estabilidad y punto crítico.
        analyzer.printReport(result); // Imprime el informe detallado en pantalla.

        exportSuccess = exporter.exportSimulationData(systemMatrix, csvFilename); // Ejecuta la simulación RK4 lineal.
    } else if (option == 2) { // Caso 2: El usuario seleccionó analizar un sistema no lineal.
        std::cout << "--- Sistema No Lineal Cuadratico General con Terminos Cruzados ---\n"; // Encabezado de la opción no lineal.
        std::cout << "Modelo: x' = a1*x + b1*y + c1*x^2 + d1*y^2 + e1*x*y\n"; // Muestra el modelo matemático expandido para x'.
        std::cout << "        y' = a2*x + b2*y + c2*x^2 + d2*y^2 + e2*x*y\n\n"; // Muestra el modelo matemático expandido para y'.

        double a1, b1, c1, d1, e1; // Declara las variables para los 5 coeficientes de la primera ecuación x' = f(x,y).
        double a2, b2, c2, d2, e2; // Declara las variables para los 5 coeficientes de la segunda ecuación y' = g(x,y).

        std::cout << "Ingrese coeficientes para x' (a1 b1 c1 d1 e1): "; // Solicita ingresar los 5 coeficientes de f(x,y).
        std::cin >> a1 >> b1 >> c1 >> d1 >> e1; // Captura a1, b1, c1, d1, e1 separados por espacios.

        std::cout << "Ingrese coeficientes para y' (a2 b2 c2 d2 e2): "; // Solicita ingresar los 5 coeficientes de g(x,y).
        std::cin >> a2 >> b2 >> c2 >> d2 >> e2; // Captura a2, b2, c2, d2, e2 separados por espacios.
        std::cout << "\n"; // Salto de línea decorativo.

        // Define f(x,y) mediante una expresión lambda que incluye términos lineales, cuadráticos y el término cruzado e1*x*y.
        auto f = [a1, b1, c1, d1, e1](double x, double y) {
            return a1 * x + b1 * y + c1 * x * x + d1 * y * y + e1 * x * y;
        }; // Fin de la expresión lambda para f(x,y).

        // Define g(x,y) mediante una expresión lambda que incluye términos lineales, cuadráticos y el término cruzado e2*x*y.
        auto g = [a2, b2, c2, d2, e2](double x, double y) {
            return a2 * x + b2 * y + c2 * x * x + d2 * y * y + e2 * x * y;
        }; // Fin de la expresión lambda para g(x,y).

        NonLinearSystem nonLinearSys(f, g); // Instancia el sistema no lineal pasando las funciones f y g.
        AnalysisResult result = analyzer.analyzeNonLinear(nonLinearSys, 0.0, 0.0); // Evalúa el Jacobiano en (0,0) y verifica la hiperbolicidad (Hartman-Grobman).
        analyzer.printReport(result); // Imprime el diagnóstico cualitativo del punto de equilibrio en pantalla.

        exportSuccess = exporter.exportSimulationDataNonLinear(nonLinearSys, csvFilename); // Ejecuta la integración numérico-temporal RK4 no lineal.
    } else { // Manejo de selección inválida.
        std::cout << "Opcion no valida. Saliendo del programa.\n"; // Notifica la entrada incorrecta.
        return 1; // Retorna código de error 1.
    } // Fin del bloque condicional de opciones.

    if (exportSuccess) { // Evalúa si el archivo CSV con las trayectorias se generó correctamente.
        std::cout << "\nEjecutando script de visualizacion grafica en Python...\n"; // Notifica el llamado al módulo de gráficos.
        std::string command = "python3 scripts/plot_phase_portrait.py " + csvFilename; // Construye la instrucción de consola para llamar al script Python.
        std::system(command.c_str()); // Ejecuta el comando del sistema operativo.
    } else { // En caso de fallo en la simulación.
        std::cerr << "Error: Ocurrio un fallo al generar o exportar la simulacion numerica.\n"; // Reporta el error en el flujo de salida de errores std::cerr.
    } // Fin de la verificación de exportación.

    return 0; // Finaliza la ejecución del programa sin errores.
} // Fin de la función principal main.