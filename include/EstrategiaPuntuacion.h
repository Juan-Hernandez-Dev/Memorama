#pragma once

struct Puntuacion {
    int puntos;
    int tiempo;
    float precision;
    int comboMaximo;

    bool operator<(const Puntuacion& otra) const {
        if (puntos != otra.puntos) return puntos > otra.puntos;
        return tiempo < otra.tiempo;
    }
};

class EstrategiaPuntuacion {
public:
    virtual ~EstrategiaPuntuacion() = default;
    virtual int calcularAcierto(int combo) = 0;
    virtual int calcularFallo() = 0;
    virtual int calcularBonusTiempo(int segundosRestantes) = 0;
    virtual int calcularPenalidadRevelado() = 0;
};

class PuntuacionClasica : public EstrategiaPuntuacion {
public:
    int calcularAcierto(int combo) override { return 100; }
    int calcularFallo() override { return -10; }
    int calcularBonusTiempo(int segundosRestantes) override { return segundosRestantes * 10; }
    int calcularPenalidadRevelado() override { return -50; }
};

class PuntuacionCombo : public EstrategiaPuntuacion {
public:
    int calcularAcierto(int combo) override { return 100 + (combo - 1) * 50; }
    int calcularFallo() override { return -10; }
    int calcularBonusTiempo(int segundosRestantes) override { return segundosRestantes * 10; }
    int calcularPenalidadRevelado() override { return -50; }
};

class PuntuacionVelocidad : public EstrategiaPuntuacion {
public:
    int calcularAcierto(int combo) override { return 100; }
    int calcularFallo() override { return -10; }
    int calcularBonusTiempo(int segundosRestantes) override { return segundosRestantes * 20; }
    int calcularPenalidadRevelado() override { return -100; }
};
