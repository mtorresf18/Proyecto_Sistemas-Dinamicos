#ifndef SYSTEM_ANALYZER_HPP // Guard de inclusión para evitar redefiniciones.
#define SYSTEM_ANALYZER_HPP // Define la constante única del encabezado.

#include "Matrix2D.hpp" // Incluye Matrix2D para el análisis algebraico.
#include "NonLinearSystem.hpp" // Incluye NonLinearSystem para procesar sistemas no lineales.
#include <string> // Incluye std::string para almacenamiento de texto.

struct AnalysisResult { // Estructura expandida para contener el diagnóstico cualitativo.
    bool isUnique; // Registra si el punto crítico es aislado/único.
    bool isHyperbolic; // Indica si se cumple la condición de hiperbolicidad (Re(lambda) != 0).
    bool hartmanGrobmanApplies; // Indica si el Teorema de Hartman-Grobman es aplicable al sistema no lineal.
    std::string message; // Mensaje descriptivo principal.
    std::string criticalPointType; // Tipo de punto crítico (Nodo, Silla, Foco, Centro).
    std::string stability; // Nivel de estabilidad (Estable, Inestable, Asintóticamente Estable).
}; // Fin de la estructura AnalysisResult.

class SystemAnalyzer { // Clase encargada de la clasificación cualitativa.
private:
    static constexpr double EPSILON = 1e-9; // Tolerancia para comparaciones en punto flotante.

public:
    SystemAnalyzer() = default; // Constructor por defecto.

    AnalysisResult analyze(const Matrix2D& matrix) const; // Analiza la matriz del sistema lineal.
    AnalysisResult analyzeNonLinear(const NonLinearSystem& system, double x0 = 0.0, double y0 = 0.0) const; // Analiza el sistema no lineal aplicando Hartman-Grobman.
    
    void printReport(const AnalysisResult& result) const; // Imprime el diagnóstico en consola.
}; // Fin de la declaración de SystemAnalyzer.

#endif // SYSTEM_ANALYZER_HPP