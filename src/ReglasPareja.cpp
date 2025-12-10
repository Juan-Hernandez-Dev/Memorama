#include "../include/ReglasPareja.h"
#include <fstream>
#include <sstream>

bool ReglasMatematicas::esPareja(const std::string& id1, const std::string& id2) const {
    return id1 == id2;
}

std::vector<Pareja> ReglasMatematicas::cargarParejas(const std::string& nombreArchivo) {
    std::vector<Pareja> parejas;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string categoria, pregunta, respuesta;

        std::getline(ss, categoria, '|');
        std::getline(ss, pregunta, '|');
        std::getline(ss, respuesta, '|');

        if (categoria == "MATEMATICAS") {
            Pareja p;
            p.pregunta = pregunta;
            p.respuesta = respuesta;
            p.categoria = categoria;
            parejas.push_back(p);
        }
    }

    return parejas;
}

bool ReglasGeografia::esPareja(const std::string& id1, const std::string& id2) const {
    return id1 == id2;
}

std::vector<Pareja> ReglasGeografia::cargarParejas(const std::string& nombreArchivo) {
    std::vector<Pareja> parejas;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string categoria, pregunta, respuesta;

        std::getline(ss, categoria, '|');
        std::getline(ss, pregunta, '|');
        std::getline(ss, respuesta, '|');

        if (categoria == "GEOGRAFIA") {
            Pareja p;
            p.pregunta = pregunta;
            p.respuesta = respuesta;
            p.categoria = categoria;
            parejas.push_back(p);
        }
    }

    return parejas;
}

bool ReglasVocabulario::esPareja(const std::string& id1, const std::string& id2) const {
    return id1 == id2;
}

std::vector<Pareja> ReglasVocabulario::cargarParejas(const std::string& nombreArchivo) {
    std::vector<Pareja> parejas;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string categoria, pregunta, respuesta;

        std::getline(ss, categoria, '|');
        std::getline(ss, pregunta, '|');
        std::getline(ss, respuesta, '|');

        if (categoria == "VOCABULARIO") {
            Pareja p;
            p.pregunta = pregunta;
            p.respuesta = respuesta;
            p.categoria = categoria;
            parejas.push_back(p);
        }
    }

    return parejas;
}

bool ReglasCiencias::esPareja(const std::string& id1, const std::string& id2) const {
    return id1 == id2;
}

std::vector<Pareja> ReglasCiencias::cargarParejas(const std::string& nombreArchivo) {
    std::vector<Pareja> parejas;
    std::ifstream archivo(nombreArchivo);
    std::string linea;

    while (std::getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string categoria, pregunta, respuesta;

        std::getline(ss, categoria, '|');
        std::getline(ss, pregunta, '|');
        std::getline(ss, respuesta, '|');

        if (categoria == "CIENCIAS") {
            Pareja p;
            p.pregunta = pregunta;
            p.respuesta = respuesta;
            p.categoria = categoria;
            parejas.push_back(p);
        }
    }

    return parejas;
}
