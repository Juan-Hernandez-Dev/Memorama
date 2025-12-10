#pragma once
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class Carta {
protected:
    std::string texto;
    std::string idPareja;
    SDL_Rect rectangulo;
    bool volteada;
    bool emparejada;

public:
    Carta(const std::string& texto, const std::string& idPareja);
    virtual ~Carta() = default;

    virtual void renderizar(SDL_Renderer* renderizador, TTF_Font* fuente) = 0;

    bool operator==(const Carta& otra) const;

    void voltear() { volteada = !volteada; }
    void establecerEmparejada(bool e) { emparejada = e; }
    void establecerRectangulo(SDL_Rect r) { rectangulo = r; }

    bool estaVolteada() const { return volteada; }
    bool estaEmparejada() const { return emparejada; }
    SDL_Rect obtenerRectangulo() const { return rectangulo; }
    std::string obtenerTexto() const { return texto; }
    std::string obtenerIdPareja() const { return idPareja; }
};

class CartaTexto : public Carta {
public:
    CartaTexto(const std::string& texto, const std::string& idPareja);
    void renderizar(SDL_Renderer* renderizador, TTF_Font* fuente) override;
};
