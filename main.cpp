#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>
#include <chrono>

class TaximetroEngine : public QObject {
    Q_OBJECT
    // Estas líneas le avisan a la pantalla que puede leer estos valores en tiempo real
    Q_PROPERTY(double totalPagar READ obtenerTotalPagar NOTIFY datosActualizados)
    Q_PROPERTY(double distanciaKm READ obtenerDistanciaKm NOTIFY datosActualizados)
    Q_PROPERTY(bool viajeActivo READ estaActivo NOTIFY datosActualizados)

private:
    const double PRECIO_POR_KM = 1.20;       
    const double PRECIO_POR_MINUTO = 0.40;   
    const double VELOCIDAD_UMBRAL = 15.0; 

    double distanciaRecorrida = 0.0; 
    double totalPagar = 0.0;         
    bool viajeActivo = false;

    std::chrono::steady_clock::time_point tiempoUltimaActualizacion;

public:
    explicit TaximetroEngine(QObject *parent = nullptr) : QObject(parent) {}

    // Funciones que activarán los botones de la pantalla
    Q_INVOKABLE void iniciarViaje() {
        distanciaRecorrida = 0.0;
        totalPagar = 0.0;
        viajeActivo = true;
        tiempoUltimaActualizacion = std::chrono::steady_clock::now();
        emit datosActualizados();
    }

    Q_INVOKABLE void terminarViaje() {
        viajeActivo = false;
        emit datosActualizados();
    }

    Q_INVOKABLE void actualizarConGPS(double velocidadActualKmH, double distanciaMetrosDesdeUltimoPunto) {
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
        emit datosActualizados();
    }

    double obtenerTotalPagar() const { return totalPagar; }
    double obtenerDistanciaKm() const { return distanciaRecorrida; }
    bool estaActivo() const { return viajeActivo; }

signals:
    void datosActualizados(); // Avisa a la pantalla que los números cambiaron para que los re-dibuje
};

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    // Aquí creamos el motor del taxímetro
    TaximetroEngine mTaximetro;

    QQmlApplicationEngine engine;
    
    // ESTE ES EL PUENTE: Registramos nuestro motor dentro de la pantalla con el nombre "taximetroEngine"
    engine.rootContext()->setContextProperty("taximetroEngine", &mTaximetro);
    
    // Carga la interfaz gráfica que hiciste
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    return app.exec();
}

#include "main.moc"
