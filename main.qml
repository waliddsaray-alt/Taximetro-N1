import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 360
    height: 640
    title: "Taxímetro N1"
    
    // Fondo oscuro elegante
    background: Rectangle {
        color: "#121212"
    }

    Column {
        anchors.centerIn: parent
        spacing: 30
        alignment: Align.Center

        // Título de la App
        Text {
            text: "TAXÍMETRO N1"
            color: "#888888"
            font.pixelSize: 18
            font.bold: true
            letterSpacing: 2
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // CONTADOR PRINCIPAL (Dinero)
        Text {
            // Conectado directamente al totalPagar del motor C++
            text: "$" + taximetroEngine.totalPagar.toFixed(2)
            color: taximetroEngine.viajeActivo ? "#00FF66" : "#FFFFFF" 
            font.pixelSize: 64
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
            
            Behavior on color { ColorAnimation { duration: 200 } }
        }

        // Datos secundarios (Distancia recorrida)
        Text {
            text: "Distancia: " + taximetroEngine.distanciaKm.toFixed(2) + " km"
            color: "#AAAAAA"
            font.pixelSize: 18
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Item { width: 1; height: 20 } // Espaciador

        // BOTÓN: INICIAR VIAJE
        Button {
            width: 260
            height: 55
            text: "INICIAR VIAJE"
            enabled: !taximetroEngine.viajeActivo
            
            contentItem: Text {
                text: parent.text
                color: parent.enabled ? "#FFFFFF" : "#555555"
                font.bold: true
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: parent.enabled ? "#00AAFF" : "#222222"
                radius: 25
            }

            onClicked: {
                taximetroEngine.iniciarViaje();
            }
        }

        // BOTÓN: TERMINAR VIAJE
        Button {
            width: 260
            height: 55
            text: "TERMINAR VIAJE"
            enabled: taximetroEngine.viajeActivo

            contentItem: Text {
                text: parent.text
                color: parent.enabled ? "#FFFFFF" : "#555555"
                font.bold: true
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            background: Rectangle {
                color: parent.enabled ? "#FF3B30" : "#222222"
                radius: 25
            }

            onClicked: {
                taximetroEngine.terminarViaje();
            }
        }
    }
}
