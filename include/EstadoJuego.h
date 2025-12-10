#pragma once
#include <string>

class EstadoJuego {
private:
    int puntuacionActual;
    int tiempoRestante;
    int parejasEmparejadas;
    int intentosTotales;
    int intentosCorrectos;
    int comboActual;
    int comboMaximo;
    bool reveladoUsado;
    std::string nivelActual;
    std::string categoriaActual;

public:
    EstadoJuego();

    void guardar(const std::string& nombreArchivo);
    void cargar(const std::string& nombreArchivo);
    void reiniciar();

    // Obtener valores
    int obtenerPuntuacion() const { return puntuacionActual; }
    int obtenerTiempoRestante() const { return tiempoRestante; }
    int obtenerParejasEmparejadas() const { return parejasEmparejadas; }
    int obtenerIntentosTotales() const { return intentosTotales; }
    int obtenerIntentosCorrectos() const { return intentosCorrectos; }
    int obtenerComboActual() const { return comboActual; }
    int obtenerComboMaximo() const { return comboMaximo; }
    bool estaReveladoUsado() const { return reveladoUsado; }

    // Establecer valores
    void establecerPuntuacion(int p) { puntuacionActual = p; }
    void establecerTiempoRestante(int t) { tiempoRestante = t; }
    void establecerParejasEmparejadas(int p) { parejasEmparejadas = p; }
    void incrementarIntentos() { intentosTotales++; }
    void incrementarIntentosCorrectos() { intentosCorrectos++; }
    void establecerComboActual(int c) { comboActual = c; if (c > comboMaximo) comboMaximo = c; }
    void usarRevelado() { reveladoUsado = true; }
    void establecerNivel(const std::string& n) { nivelActual = n; }
    void establecerCategoria(const std::string& c) { categoriaActual = c; }

    float obtenerPrecision() const {
        return intentosTotales > 0 ? (float)intentosCorrectos / intentosTotales * 100.0f : 100.0f;
    }
};
