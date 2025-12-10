#include "../include/Carta.h"
#include <SDL_ttf.h>

Carta::Carta(const std::string& texto, const std::string& idPareja)
    : texto(texto), idPareja(idPareja), volteada(false), emparejada(false) {
    rectangulo = {0, 0, 0, 0};
}

bool Carta::operator==(const Carta& otra) const {
    return idPareja == otra.idPareja;
}

CartaTexto::CartaTexto(const std::string& texto, const std::string& idPareja)
    : Carta(texto, idPareja) {}

void CartaTexto::renderizar(SDL_Renderer* renderizador, TTF_Font* fuente) {
    SDL_Color colorFondo = {102, 126, 234, 255};
    SDL_Color colorBlanco = {255, 255, 255, 255};
    SDL_Color colorTexto = {102, 126, 234, 255};

    if (emparejada) {
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
        SDL_RenderFillRect(renderizador, &rectangulo);

        SDL_Rect borde = rectangulo;
        borde.x += 3; borde.y += 3;
        borde.w -= 6; borde.h -= 6;
        SDL_SetRenderDrawColor(renderizador, colorFondo.r, colorFondo.g, colorFondo.b, 255);
        SDL_RenderDrawRect(renderizador, &borde);

        SDL_Surface* superficie = TTF_RenderUTF8_Blended(fuente, texto.c_str(), colorTexto);
        if (superficie) {
            SDL_Texture* textura = SDL_CreateTextureFromSurface(renderizador, superficie);
            SDL_Rect rectTexto = {
                rectangulo.x + (rectangulo.w - superficie->w) / 2,
                rectangulo.y + (rectangulo.h - superficie->h) / 2,
                superficie->w, superficie->h
            };
            SDL_RenderCopy(renderizador, textura, NULL, &rectTexto);
            SDL_DestroyTexture(textura);
            SDL_FreeSurface(superficie);
        }
    } else if (volteada) {
        SDL_SetRenderDrawColor(renderizador, 255, 255, 255, 255);
        SDL_RenderFillRect(renderizador, &rectangulo);

        SDL_Surface* superficie = TTF_RenderUTF8_Blended(fuente, texto.c_str(), colorTexto);
        if (superficie) {
            SDL_Texture* textura = SDL_CreateTextureFromSurface(renderizador, superficie);
            SDL_Rect rectTexto = {
                rectangulo.x + (rectangulo.w - superficie->w) / 2,
                rectangulo.y + (rectangulo.h - superficie->h) / 2,
                superficie->w, superficie->h
            };
            SDL_RenderCopy(renderizador, textura, NULL, &rectTexto);
            SDL_DestroyTexture(textura);
            SDL_FreeSurface(superficie);
        }
    } else {
        SDL_SetRenderDrawColor(renderizador, colorFondo.r, colorFondo.g, colorFondo.b, 255);
        SDL_RenderFillRect(renderizador, &rectangulo);

        SDL_Surface* superficie = TTF_RenderUTF8_Blended(fuente, "?", colorBlanco);
        if (superficie) {
            SDL_Texture* textura = SDL_CreateTextureFromSurface(renderizador, superficie);
            SDL_Rect rectTexto = {
                rectangulo.x + (rectangulo.w - superficie->w) / 2,
                rectangulo.y + (rectangulo.h - superficie->h) / 2,
                superficie->w, superficie->h
            };
            SDL_RenderCopy(renderizador, textura, NULL, &rectTexto);
            SDL_DestroyTexture(textura);
            SDL_FreeSurface(superficie);
        }
    }
}
