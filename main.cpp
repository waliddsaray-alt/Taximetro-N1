#include <iostream>
#include <chrono>

class TaximetroEngine {
private:
    const double PRECIO_POR_KM = 1.20;       
    const double PRECIO_POR_MINUTO = 0.40;   
    const double VELOCIDAD_UMBRAL = 15.0; 

    double distanciaRecorrida; 
    double totalPagar;         
    bool viajeActivo;

    std::chrono::steady_clock::time_point tiempoUltimaActualizacion;

public:
    TaximetroEngine() {
        distanciaRecorrida = 0.0;
        totalPagar = 0.0;
        viajeActivo = false;
    }

    void iniciarViaje() {
        distanciaRecorrida = 0.0;
        totalPagar = 0.0;
        viajeActivo = true;
        tiempoUltimaActualizacion = std::chrono::steady_clock::now();
    }

    void terminarViaje() {
        viajeActivo = false;
    }

    void actualizarConGPS(double velocidadActualKmH, double distanciaMetrosDesdeUltimoPunto) {
        if (!viajeActivo) return;

        auto ahora = std::chrono::steady_clock::now();
        std::chrono::duration<double> duracionSg = ahora - tiempoUltimaActualizacion;
        double minutosPasados = duracionSg.count() / 60.0;
        tiempoUltimaActualizacion = ahora; 

        double distanciaKmTramo = distanciaMetrosDesdeUltimoPunto / 1000.0;
        distanciaRecorrida += distanciaKmTramo;

        if (velocidadActualKmH < VELOCIDAD_UMBRAL) {
            totalPagar += minutosPasados * PRECIO_POR_MINUTO;
        } else {
            totalPagar += distanciaKmTramo * PRECIO_POR_KM;
        }
    }

    double obtenerTotalPagar() const { return totalPagar; }
    double obtenerDistanciaKm() const { return distanciaRecorrida; }
    bool estaActivo() const { return viajeActivo; }
};

int main() {
    // Esto se queda aquí solo para verificar que compile perfecto
    std::cout << "Motor del Taximetro N1 inicializado correctamente." << std::endl;
    return 0;
}
