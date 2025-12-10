#pragma once
#include "EstrategiaPuntuacion.h"
#include <string>
#include <vector>

struct Registro {
    std::string nombreJugador;
    int puntuacion;
    int tiempoUsado;
    float precision;
    int comboMaximo;
    std::string nivel;
    std::string categoria;
};

class TablaLideres {
private:
    std::vector<Registro> registros;

public:
    void agregarRegistro(const Registro& registro);
    void cargarBinario(const std::string& nombreArchivo);
    void guardarBinario(const std::string& nombreArchivo);
    void exportarTexto(const std::string& nombreArchivo);

    std::vector<Registro> obtenerMejoresRegistros(int cantidad, const std::string& nivel = "",
                                                   const std::string& categoria = "");
};
