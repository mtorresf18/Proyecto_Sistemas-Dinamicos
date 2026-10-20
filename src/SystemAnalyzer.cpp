#include "SystemAnalyzer.hpp" // Incluye la interfaz SystemAnalyzer.hpp para implementar sus métodos miembros.
#include <iostream> // Incluye la biblioteca de entrada y salida estándar para imprimir los reportes en consola.
#include <cmath> // Incluye la biblioteca matemática para la función std::abs en comparaciones de tolerancia.

AnalysisResult SystemAnalyzer::analyze(const Matrix2D& matrix) const { // Implementa el método de análisis cualitativo del sistema.
    AnalysisResult result; // Instancia la estructura de resultado que se completará durante el análisis.
    
    double det = matrix.computeDeterminant(); // Calcula el determinante det(A) de la matriz de coeficientes.
    
    if (std::abs(det) < EPSILON) { // Evalúa la condición de unicidad: si det(A) == 0 dentro de la tolerancia numérica.
        result.isUnique = false; // Registra que el punto crítico (0,0) no es único debido a det(A) = 0.
        result.message = "EL PUNTO CRITICO (0,0) NO ES UNICO"; // Asigna el mensaje requerido de forma textual cuando det(A) = 0.
        result.criticalPointType = "No determinado (recta o plano de equilibrios)"; // Clasifica el tipo como no aislado.
        result.stability = "No aplica / Depende de la variedad de equilibrios"; // Asigna el estado de estabilidad indeterminado.
        return result; // Retorna tempranamente el resultado informando la falta de unicidad.
    } // Fin del bloque condicional para el caso no único (det = 0).

    result.isUnique = true; // Confirma que el punto crítico (0,0) es único al cumplirse det(A) != 0.
    result.message = "El punto critico (0,0) es unico."; // Asigna el mensaje de confirmación de unicidad.

    auto eigenvalues = matrix.computeEigenvalues(); // Obtiene el par de valores propios (lambda1, lambda2) desde Matrix2D.
    std::complex<double> l1 = eigenvalues.first; // Extrae el primer valor propio lambda1.
    std::complex<double> l2 = eigenvalues.second; // Extrae el segundo valor propio lambda2.

    bool isComplex = (std::abs(l1.imag()) > EPSILON); // Verifica si los valores propios poseen parte imaginaria no nula.

    if (isComplex) { // Inicia el bloque de clasificación para valores propios complejos conjugados (alpha +/- i*beta).
        double alpha = l1.real(); // Extrae la parte real común alpha de los valores propios complejos.
        
        if (std::abs(alpha) < EPSILON) { // Evalúa si la parte real es cero (alpha = 0, valores imaginarios puros +/- i*omega).
            result.criticalPointType = "Centro"; // Clasifica el punto crítico como un Centro según la tabla de criterios.
            result.stability = "Estable (no asintotico)"; // Asigna la estabilidad no asintótica correspondiente a un Centro.
        } else if (alpha > EPSILON) { // Evalúa si la parte real es strictly positiva (alpha > 0).
            result.criticalPointType = "Foco (Espiral)"; // Clasifica el punto crítico como Foco o Espiral.
            result.stability = "Inestable"; // Asigna la clasificación de inestabilidad al foco espiral inestable.
        } else { // Caso en que la parte real es strictly negativa (alpha < 0).
            result.criticalPointType = "Foco (Espiral)"; // Clasifica el punto crítico como Foco o Espiral.
            result.stability = "Asintoticamente Estable"; // Asigna la clasificación de estabilidad asintótica al foco.
        } // Fin de las subdivisiones para valores propios complejos.
    } else { // Inicia el bloque de clasificación para valores propios reales (beta = 0).
        double r1 = l1.real(); // Extrae la primera raíz real r1.
        double r2 = l2.real(); // Extrae la segunda raíz real r2.

        if ((r1 > EPSILON && r2 < -EPSILON) || (r1 < -EPSILON && r2 > EPSILON)) { // Evalúa si r1 y r2 tienen signos opuestos.
            result.criticalPointType = "Punto Silla"; // Clasifica como Punto Silla cuando los signos de lambda son opuestos.
            result.stability = "Inestable"; // Asigna la clasificación de inestable propia de un punto silla.
        } else if (r1 > EPSILON && r2 > EPSILON) { // Evalúa si ambos valores propios reales son estrictamente positivos.
            result.criticalPointType = "Nodo"; // Clasifica como Nodo (o Nodo impropio si r1 == r2).
            result.stability = "Inestable"; // Asigna la clasificación de inestable para nodo con lambdas positivas.
        } else if (r1 < -EPSILON && r2 < -EPSILON) { // Evalúa si ambos valores propios reales son estrictamente negativos.
            result.criticalPointType = "Nodo"; // Clasifica como Nodo (o Nodo impropio si r1 == r2).
            result.stability = "Asintoticamente Estable"; // Asigna la clasificación de asintóticamente estable para lambdas negativas.
        } else { // Caso límite en que alguna raíz real sea cero.
            result.criticalPointType = "Degenerado"; // Clasifica como punto degenerado por tener lambda real igual a cero.
            result.stability = "Inestable / Indeterminado"; // Asigna estabilidad indeterminada para el caso degenerado.
        } // Fin de las subdivisiones para valores propios reales.
    } // Fin del bloque de análisis según la naturaleza de los valores propios.

    return result; // Retorna la estructura completada con el diagnóstico completo del punto crítico.
} // Fin del método analyze.

void SystemAnalyzer::printReport(const AnalysisResult& result) const { // Implementa la impresión formateada del reporte final.
    std::cout << "========================================\n"; // Imprime una línea divisoria de encabezado.
    std::cout << "     DIAGNOSTICO DEL SISTEMA LINEAL     \n"; // Imprime el título del reporte en consola.
    std::cout << "========================================\n"; // Imprime una línea divisoria de separación.
    
    if (!result.isUnique) { // Evalúa si se registró la falta de unicidad del punto crítico.
        std::cout << "MENSAJE: " << result.message << "\n"; // Muestra en pantalla el mensaje obligatorio "EL PUNTO CRITICO (0,0) NO ES UNICO".
    } else { // En caso de que el punto crítico sea único.
        std::cout << "Unicidad: " << result.message << "\n"; // Confirma que el punto es único.
        std::cout << "Tipo de Punto Critico: " << result.criticalPointType << "\n"; // Muestra el tipo de punto crítico determinado.
        std::cout << "Estabilidad: " << result.stability << "\n"; // Muestra la clasificación de estabilidad resultante.
    } // Fin de la comprobación de impresión.
    
    std::cout << "========================================\n"; // Imprime la línea divisoria final del reporte.
} // Fin del método printReport.