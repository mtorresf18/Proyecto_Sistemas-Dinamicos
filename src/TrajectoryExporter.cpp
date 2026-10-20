#include "TrajectoryExporter.hpp" // Incluye la declaración de la clase TrajectoryExporter.
#include <fstream> // Incluye fstream para la creación y escritura de archivos de texto CSV.
#include <iostream> // Incluye iostream para emitir mensajes de estado o error en la consola.
#include <cmath> // Incluye cmath para funciones matemáticas auxiliares como std::abs.

Point2D TrajectoryExporter::computeDerivatives(const Matrix2D& matrix, const Point2D& p) const { // Implementa el cálculo del sistema lineal x' = A*x.
    double dX = matrix.getA1() * p.x + matrix.getB1() * p.y; // Calcula x' = a1*x + b1*y para el punto p.
    double dY = matrix.getA2() * p.x + matrix.getB2() * p.y; // Calcula y' = a2*x + b2*y para el punto p.
    return {dX, dY}; // Retorna el vector tangente (x', y') evaluado en el punto p.
} // Fin del método computeDerivatives.

Point2D TrajectoryExporter::rk4Step(const Matrix2D& matrix, const Point2D& p, double dt) const { // Implementa un paso de integración numérica Runge-Kutta de 4º orden (RK4).
    Point2D k1 = computeDerivatives(matrix, p); // Calcula la primera pendiente k1 en el punto inicial p.
    
    Point2D p2 = {p.x + 0.5 * dt * k1.x, p.y + 0.5 * dt * k1.y}; // Estima el punto medio p2 usando la pendiente k1.
    Point2D k2 = computeDerivatives(matrix, p2); // Calcula la segunda pendiente k2 en el punto medio p2.
    
    Point2D p3 = {p.x + 0.5 * dt * k2.x, p.y + 0.5 * dt * k2.y}; // Estima el punto medio p3 usando la pendiente k2.
    Point2D k3 = computeDerivatives(matrix, p3); // Calcula la tercera pendiente k3 en el punto medio p3.
    
    Point2D p4 = {p.x + dt * k3.x, p.y + dt * k3.y}; // Estima el punto final p4 del intervalo usando la pendiente k3.
    Point2D k4 = computeDerivatives(matrix, p4); // Calcula la cuarta pendiente k4 en el punto final p4.

    double newX = p.x + (dt / 6.0) * (k1.x + 2.0 * k2.x + 2.0 * k3.x + k4.x); // Aplica la combinación ponderada de Runge-Kutta 4 para la variable x.
    double newY = p.y + (dt / 6.0) * (k1.y + 2.0 * k2.y + 2.0 * k3.y + k4.y); // Aplica la combinación ponderada de Runge-Kutta 4 para la variable y.

    return {newX, newY}; // Devuelve las coordenadas integradas tras el paso de tiempo dt.
} // Fin del método rk4Step.

bool TrajectoryExporter::exportSimulationData(const Matrix2D& matrix, const std::string& filename, double range, double dt, double tMax, int gridDensity) const { // Implementa la exportación masiva de trayectorias hacia el archivo CSV.
    std::ofstream outFile(filename); // Abre el archivo de salida con el nombre especificado.
    
    if (!outFile.is_open()) { // Verifica si el archivo no se pudo crear o abrir correctamente.
        std::cerr << "Error: No se pudo crear el archivo " << filename << std::endl; // Muestra un mensaje de error en la consola estándar de errores.
        return false; // Retorna false indicando el fallo en la apertura del archivo.
    } // Fin de la comprobación de apertura de archivo.

    outFile << "trajectory_id,t,x,y\n"; // Escribe la cabecera del archivo CSV para el procesamiento posterior.

    int trajectoryId = 0; // Inicializa el identificador de cada trayectoria individual.
    double stepSize = (2.0 * range) / gridDensity; // Calcula el espaciado entre puntos iniciales del plano de fase.

    for (int i = 0; i <= gridDensity; ++i) { // Bucle exterior para recorrer la malla en la dirección x.
        double x0 = -range + i * stepSize; // Calcula la coordenada inicial x0 de la condición inicial.
        for (int j = 0; j <= gridDensity; ++j) { // Bucle interior para recorrer la malla en la dirección y.
            double y0 = -range + j * stepSize; // Calcula la coordenada inicial y0 de la condición inicial.

            if (std::abs(x0) < 1e-5 && std::abs(y0) < 1e-5) { // Omite la condición inicial en el origen exacto (0,0) por ser el punto crítico.
                continue; // Salta al siguiente punto del bucle para no integrar una trayectoria constante en (0,0).
            } // Fin de la comprobación del origen.

            Point2D current = {x0, y0}; // Asigna el punto de inicio de la trayectoria actual.
            double t = 0.0; // Inicializa el contador de tiempo de simulación en cero.
            int numSteps = static_cast<int>(tMax / dt); // Calcula la cantidad total de pasos de integración temporal.

            for (int step = 0; step <= numSteps; ++step) { // Bucle temporal para integrar la trayectoria actual.
                outFile << trajectoryId << "," << t << "," << current.x << "," << current.y << "\n"; // Registra una fila en el archivo CSV con el estado actual.
                
                if (std::abs(current.x) > range * 3.0 || std::abs(current.y) > range * 3.0) { // Comprueba si la trayectoria diverge fuera del área visible.
                    break; // Interrumpe la integración de esta trayectoria si se aleja demasiado.
                } // Fin de la verificación de divergencia.

                current = rk4Step(matrix, current, dt); // Avanza las coordenadas del sistema un paso temporal mediante RK4.
                t += dt; // Incrementa el valor del tiempo acumulado en dt.
            } // Fin del bucle de integración temporal de la trayectoria.

            trajectoryId++; // Incrementa el identificador para la siguiente trayectoria.
        } // Fin del bucle interno de posiciones y.
    } // Fin del bucle externo de posiciones x.

    outFile.close(); // Cierra formalmente el archivo CSV guardando todos los datos en disco.
    std::cout << "Datos de trayectorias exportados exitosamente a " << filename << std::endl; // Notifica en consola la correcta exportación.
    return true; // Retorna true confirmando la finalización exitosa.
} // Fin del método exportSimulationData.