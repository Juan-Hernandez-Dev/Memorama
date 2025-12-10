#pragma once
#include <string>
#include <map>

class Configuracion {
private:
    std::map<std::string, int> ajustes;
    void cargarPorDefecto();

public:
    Configuracion();
    bool cargar(const std::string& nombreArchivo);
    void guardar(const std::string& nombreArchivo);

    int obtener(const std::string& clave, int valorPorDefecto = 0);
    void establecer(const std::string& clave, int valor);
};
