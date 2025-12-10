#pragma once
#include <string>

class Nivel {
protected:
    int numParejas;
    int limiteTiempo;
    int tiempoRevelado;
    int columnasTablero;
    int filasTablero;

public:
    Nivel(int parejas, int tiempo, int revelado, int cols, int filas)
        : numParejas(parejas), limiteTiempo(tiempo), tiempoRevelado(revelado),
          columnasTablero(cols), filasTablero(filas) {}

    virtual ~Nivel() = default;

    int obtenerNumParejas() const { return numParejas; }
    int obtenerLimiteTiempo() const { return limiteTiempo; }
    int obtenerTiempoRevelado() const { return tiempoRevelado; }
    int obtenerColumnasTablero() const { return columnasTablero; }
    int obtenerFilasTablero() const { return filasTablero; }
    virtual std::string obtenerNombre() const = 0;
};

class NivelFacil : public Nivel {
public:
    NivelFacil() : Nivel(8, 90, 5, 4, 4) {}
    std::string obtenerNombre() const override { return "FACIL"; }
};

class NivelMedio : public Nivel {
public:
    NivelMedio() : Nivel(12, 120, 4, 4, 6) {}
    std::string obtenerNombre() const override { return "MEDIO"; }
};

class NivelDificil : public Nivel {
public:
    NivelDificil() : Nivel(18, 180, 3, 6, 6) {}
    std::string obtenerNombre() const override { return "DIFICIL"; }
};
