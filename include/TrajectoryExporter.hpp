#ifndef TRAJECTORY_EXPORTER_HPP // Guard de inclusión para evitar redefiniciones al incluir la cabecera múltiples veces.
#define TRAJECTORY_EXPORTER_HPP // Define la constante única asociada a este archivo de encabezado.

#include "Matrix2D.hpp" // Incluye la clase Matrix2D para obtener los coeficientes a1, b1, a2, b2 del sistema dinámico.
#include <string> // Incluye la librería estándar para manejar nombres de archivos de texto (std::string).
#include <vector> // Incluye la librería de vectores dinámicos para almacenar colecciones de puntos y trayectorias.

// Estructura que representa un punto en el plano de fase 2D con coordenadas (x, y).
struct Point2D {
    double x; // Coordenada en el eje x del estado del sistema.
    double y; // Coordenada en el eje y del estado del sistema.
}; // Fin de la estructura Point2D.

// Clase encargada de integrar numéricamente las trayectorias del sistema lineal y exportar los datos a un archivo CSV.
class TrajectoryExporter {
private: // Métodos privados de soporte para la integración numérica.
    Point2D computeDerivatives(const Matrix2D& matrix, const Point2D& p) const; // Calcula el vector de velocidad (x', y') en el punto p.
    Point2D rk4Step(const Matrix2D& matrix, const Point2D& p, double dt) const; // Avanza un paso en el tiempo utilizando el método Runge-Kutta de orden 4.

public: // Métodos públicos accesibles para la generación de la simulación.
    TrajectoryExporter() = default; // Constructor por defecto de la clase TrajectoryExporter.

    bool exportSimulationData(const Matrix2D& matrix, const std::string& filename, double range = 5.0, double dt = 0.01, double tMax = 5.0, int gridDensity = 8) const; // Exporta el campo vectorial y las trayectorias numéricas a un archivo CSV.
}; // Fin de la declaración de la clase TrajectoryExporter.

#endif // TRAJECTORY_EXPORTER_HPP // Fin de la protección contra inclusión múltiple del guard TRAJECTORY_EXPORTER_HPP.