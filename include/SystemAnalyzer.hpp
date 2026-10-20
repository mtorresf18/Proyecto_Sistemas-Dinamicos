#ifndef SYSTEM_ANALYZER_HPP // Guard de inclusión para evitar redefiniciones al incluir la cabecera múltiples veces.
#define SYSTEM_ANALYZER_HPP // Define la constante única asociada a este archivo de encabezado.

#include "Matrix2D.hpp" // Incluye la definición de la clase Matrix2D para acceder a sus métodos matemáticos.
#include <string> // Incluye la librería estándar para el manejo de cadenas de texto (std::string).

// Estructura que almacena el resultado completo del análisis del sistema dinámico.
struct AnalysisResult {
    bool isUnique; // Indica si el punto crítico (0,0) es único (true) o no (false).
    std::string message; // Almacena mensajes informativos o de advertencia según la teoría.
    std::string criticalPointType; // Almacena la clasificación del punto crítico (Nodo, Silla, Foco, Centro).
    std::string stability; // Almacena la clasificación de estabilidad (Estable, Inestable, Asintóticamente Estable).
}; // Fin de la estructura AnalysisResult.

// Clase encargada de analizar la estabilidad y tipo de punto crítico de un sistema lineal.
class SystemAnalyzer {
private: // Miembros privados para control interno y precisión de cálculos.
    static constexpr double EPSILON = 1e-9; // Tolerancia numérica para comparaciones de punto flotante cercanas a cero.

public: // Métodos públicos accesibles desde el main u otros módulos.
    SystemAnalyzer() = default; // Constructor por defecto explícito de la clase.

    AnalysisResult analyze(const Matrix2D& matrix) const; // Método principal que analiza una matriz de coeficientes dada.
    
    void printReport(const AnalysisResult& result) const; // Método auxiliar para imprimir en pantalla el informe detallado del análisis.
}; // Fin de la declaración de la clase SystemAnalyzer.

#endif // SYSTEM_ANALYZER_HPP // Fin de la protección contra inclusión múltiple del guard SYSTEM_ANALYZER_HPP.