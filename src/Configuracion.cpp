#include "../include/Configuracion.h"
#include <fstream>
#include <sstream>

Configuracion::Configuracion() {
    cargarPorDefecto();
}

void Configuracion::cargarPorDefecto() {
    ajustes["tiempo_facil"] = 90;
    ajustes["tiempo_medio"] = 120;
    ajustes["tiempo_dificil"] = 180;
    ajustes["parejas_facil"] = 8;
    ajustes["parejas_medio"] = 12;
    ajustes["parejas_dificil"] = 18;
    ajustes["revelado_facil"] = 5;
    ajustes["revelado_medio"] = 4;
    ajustes["revelado_dificil"] = 3;
    ajustes["puntos_acierto"] = 100;
    ajustes["bonus_combo"] = 50;
    ajustes["penalidad_fallo"] = 10;
    ajustes["bonus_tiempo"] = 10;
    ajustes["penalidad_revelado"] = 50;
}

bool Configuracion::cargar(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo) return false;

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string clave;
        int valor;

        if (std::getline(ss, clave, '=') && ss >> valor) {
            ajustes[clave] = valor;
        }
    }

    return true;
}

void Configuracion::guardar(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo) return;

    archivo << "# Configuracion del Juego de Memoria\n\n";
    archivo << "# Tiempos por nivel (segundos)\n";
    archivo << "tiempo_facil=" << ajustes["tiempo_facil"] << "\n";
    archivo << "tiempo_medio=" << ajustes["tiempo_medio"] << "\n";
    archivo << "tiempo_dificil=" << ajustes["tiempo_dificil"] << "\n\n";

    archivo << "# Numero de parejas por nivel\n";
    archivo << "parejas_facil=" << ajustes["parejas_facil"] << "\n";
    archivo << "parejas_medio=" << ajustes["parejas_medio"] << "\n";
    archivo << "parejas_dificil=" << ajustes["parejas_dificil"] << "\n\n";

    archivo << "# Tiempo de revelado inicial (segundos)\n";
    archivo << "revelado_facil=" << ajustes["revelado_facil"] << "\n";
    archivo << "revelado_medio=" << ajustes["revelado_medio"] << "\n";
    archivo << "revelado_dificil=" << ajustes["revelado_dificil"] << "\n\n";

    archivo << "# Sistema de puntuacion\n";
    archivo << "puntos_acierto=" << ajustes["puntos_acierto"] << "\n";
    archivo << "bonus_combo=" << ajustes["bonus_combo"] << "\n";
    archivo << "penalidad_fallo=" << ajustes["penalidad_fallo"] << "\n";
    archivo << "bonus_tiempo=" << ajustes["bonus_tiempo"] << "\n";
    archivo << "penalidad_revelado=" << ajustes["penalidad_revelado"] << "\n";
}

int Configuracion::obtener(const std::string& clave, int valorPorDefecto) {
    auto it = ajustes.find(clave);
    return (it != ajustes.end()) ? it->second : valorPorDefecto;
}

void Configuracion::establecer(const std::string& clave, int valor) {
    ajustes[clave] = valor;
}
