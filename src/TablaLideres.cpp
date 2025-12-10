#include "../include/TablaLideres.h"
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>

void TablaLideres::agregarRegistro(const Registro& registro) {
    registros.push_back(registro);

    std::sort(registros.begin(), registros.end(), [](const Registro& a, const Registro& b) {
        if (a.puntuacion != b.puntuacion) return a.puntuacion > b.puntuacion;
        return a.tiempoUsado < b.tiempoUsado;
    });

    if (registros.size() > 50) {
        registros.resize(50);
    }
}

void TablaLideres::cargarBinario(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) return;

    registros.clear();

    size_t cantidad;
    archivo.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));

    for (size_t i = 0; i < cantidad; i++) {
        Registro r;

        size_t longitud;
        archivo.read(reinterpret_cast<char*>(&longitud), sizeof(longitud));
        r.nombreJugador.resize(longitud);
        archivo.read(&r.nombreJugador[0], longitud);

        archivo.read(reinterpret_cast<char*>(&r.puntuacion), sizeof(r.puntuacion));
        archivo.read(reinterpret_cast<char*>(&r.tiempoUsado), sizeof(r.tiempoUsado));
        archivo.read(reinterpret_cast<char*>(&r.precision), sizeof(r.precision));
        archivo.read(reinterpret_cast<char*>(&r.comboMaximo), sizeof(r.comboMaximo));

        archivo.read(reinterpret_cast<char*>(&longitud), sizeof(longitud));
        r.nivel.resize(longitud);
        archivo.read(&r.nivel[0], longitud);

        archivo.read(reinterpret_cast<char*>(&longitud), sizeof(longitud));
        r.categoria.resize(longitud);
        archivo.read(&r.categoria[0], longitud);

        registros.push_back(r);
    }
}

void TablaLideres::guardarBinario(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) return;

    size_t cantidad = registros.size();
    archivo.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));

    for (const auto& r : registros) {
        size_t longitud = r.nombreJugador.size();
        archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(longitud));
        archivo.write(r.nombreJugador.c_str(), longitud);

        archivo.write(reinterpret_cast<const char*>(&r.puntuacion), sizeof(r.puntuacion));
        archivo.write(reinterpret_cast<const char*>(&r.tiempoUsado), sizeof(r.tiempoUsado));
        archivo.write(reinterpret_cast<const char*>(&r.precision), sizeof(r.precision));
        archivo.write(reinterpret_cast<const char*>(&r.comboMaximo), sizeof(r.comboMaximo));

        longitud = r.nivel.size();
        archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(longitud));
        archivo.write(r.nivel.c_str(), longitud);

        longitud = r.categoria.size();
        archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(longitud));
        archivo.write(r.categoria.c_str(), longitud);
    }
}

void TablaLideres::exportarTexto(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo) return;

    archivo << "=== TABLA DE RECORDS - JUEGO DE MEMORIA ===\n\n";

    auto mejoresRegistros = obtenerMejoresRegistros(10);

    archivo << std::left << std::setw(5) << "#"
            << std::setw(15) << "JUGADOR"
            << std::setw(10) << "PUNTOS"
            << std::setw(10) << "TIEMPO"
            << std::setw(12) << "PRECISION"
            << std::setw(8) << "COMBO"
            << std::setw(12) << "NIVEL"
            << "CATEGORIA\n";
    archivo << std::string(90, '-') << "\n";

    for (size_t i = 0; i < mejoresRegistros.size(); i++) {
        const auto& r = mejoresRegistros[i];

        std::stringstream tiempoStr;
        tiempoStr << std::setfill('0') << std::setw(2) << (r.tiempoUsado / 60)
                  << ":" << std::setw(2) << (r.tiempoUsado % 60);

        std::stringstream precStr;
        precStr << std::fixed << std::setprecision(1) << r.precision << "%";

        archivo << std::left << std::setw(5) << (i + 1)
                << std::setw(15) << r.nombreJugador
                << std::setw(10) << r.puntuacion
                << std::setw(10) << tiempoStr.str()
                << std::setw(12) << precStr.str()
                << std::setw(8) << ("x" + std::to_string(r.comboMaximo))
                << std::setw(12) << r.nivel
                << r.categoria << "\n";
    }
}

std::vector<Registro> TablaLideres::obtenerMejoresRegistros(int cantidad, const std::string& nivel,
                                                             const std::string& categoria) {
    std::vector<Registro> filtrados;

    for (const auto& r : registros) {
        bool coincide = true;
        if (!nivel.empty() && r.nivel != nivel) coincide = false;
        if (!categoria.empty() && r.categoria != categoria) coincide = false;
        if (coincide) filtrados.push_back(r);
    }

    if (filtrados.size() > (size_t)cantidad) {
        filtrados.resize(cantidad);
    }

    return filtrados;
}
