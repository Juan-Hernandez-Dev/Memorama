#include "../include/EstadoJuego.h"
#include <fstream>

EstadoJuego::EstadoJuego() {
    reiniciar();
}

void EstadoJuego::reiniciar() {
    puntuacionActual = 0;
    tiempoRestante = 0;
    parejasEmparejadas = 0;
    intentosTotales = 0;
    intentosCorrectos = 0;
    comboActual = 1;
    comboMaximo = 1;
    reveladoUsado = false;
    nivelActual = "";
    categoriaActual = "";
}

void EstadoJuego::guardar(const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) return;

    archivo.write(reinterpret_cast<const char*>(&puntuacionActual), sizeof(puntuacionActual));
    archivo.write(reinterpret_cast<const char*>(&tiempoRestante), sizeof(tiempoRestante));
    archivo.write(reinterpret_cast<const char*>(&parejasEmparejadas), sizeof(parejasEmparejadas));
    archivo.write(reinterpret_cast<const char*>(&intentosTotales), sizeof(intentosTotales));
    archivo.write(reinterpret_cast<const char*>(&intentosCorrectos), sizeof(intentosCorrectos));
    archivo.write(reinterpret_cast<const char*>(&comboActual), sizeof(comboActual));
    archivo.write(reinterpret_cast<const char*>(&comboMaximo), sizeof(comboMaximo));
    archivo.write(reinterpret_cast<const char*>(&reveladoUsado), sizeof(reveladoUsado));

    size_t longitud = nivelActual.size();
    archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(longitud));
    archivo.write(nivelActual.c_str(), longitud);

    longitud = categoriaActual.size();
    archivo.write(reinterpret_cast<const char*>(&longitud), sizeof(longitud));
    archivo.write(categoriaActual.c_str(), longitud);
}

void EstadoJuego::cargar(const std::string& nombreArchivo) {
    std::ifstream archivo(nombreArchivo, std::ios::binary);
    if (!archivo) return;

    archivo.read(reinterpret_cast<char*>(&puntuacionActual), sizeof(puntuacionActual));
    archivo.read(reinterpret_cast<char*>(&tiempoRestante), sizeof(tiempoRestante));
    archivo.read(reinterpret_cast<char*>(&parejasEmparejadas), sizeof(parejasEmparejadas));
    archivo.read(reinterpret_cast<char*>(&intentosTotales), sizeof(intentosTotales));
    archivo.read(reinterpret_cast<char*>(&intentosCorrectos), sizeof(intentosCorrectos));
    archivo.read(reinterpret_cast<char*>(&comboActual), sizeof(comboActual));
    archivo.read(reinterpret_cast<char*>(&comboMaximo), sizeof(comboMaximo));
    archivo.read(reinterpret_cast<char*>(&reveladoUsado), sizeof(reveladoUsado));

    size_t longitud;
    archivo.read(reinterpret_cast<char*>(&longitud), sizeof(longitud));
    nivelActual.resize(longitud);
    archivo.read(&nivelActual[0], longitud);

    archivo.read(reinterpret_cast<char*>(&longitud), sizeof(longitud));
    categoriaActual.resize(longitud);
    archivo.read(&categoriaActual[0], longitud);
}
