#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <memory>
#include <random>
#include "../include/Carta.h"
#include "../include/Nivel.h"
#include "../include/ReglasPareja.h"
#include "../include/EstrategiaPuntuacion.h"
#include "../include/EstadoJuego.h"
#include "../include/TablaLideres.h"
#include "../include/Configuracion.h"

SDL_Color COLOR_FONDO = {240, 242, 245, 255};
SDL_Color COLOR_MORADO = {99, 102, 241, 255};
SDL_Color COLOR_BLANCO = {255, 255, 255, 255};
SDL_Color COLOR_GRIS = {156, 163, 175, 255};
SDL_Color COLOR_TEXTO_OSCURO = {55, 65, 81, 255};

enum Pantalla { PANTALLA_MENU, PANTALLA_JUEGO, PANTALLA_VICTORIA, PANTALLA_LIDERES };

struct Juego {
    Pantalla pantallaActual;
    std::unique_ptr<Nivel> nivel;
    std::unique_ptr<ReglasPareja> reglasPareja;
    std::unique_ptr<EstrategiaPuntuacion> estrategiaPuntuacion;
    std::vector<std::unique_ptr<Carta>> cartas;
    EstadoJuego estadoJuego;
    TablaLideres tablaLideres;
    Configuracion configuracion;
    int dificultadSeleccionada;
    int categoriaSeleccionada;
    int indicesVolteados[2];
    int numVolteadas;
    Uint32 ultimoTiempo;
    Uint32 tiempoVoltearAtras;
    bool estaRevelando;
    Uint32 tiempoInicioRevelado;
    bool juegoIniciado;
    int anchoP, altoP;
};

void dibujarTexto(SDL_Renderer *renderizador, TTF_Font *fuente, const char *texto,
              int x, int y, SDL_Color color, int centrado) {
    SDL_Surface *superficie = TTF_RenderUTF8_Blended(fuente, texto, color);
    if (!superficie) return;
    SDL_Texture *textura = SDL_CreateTextureFromSurface(renderizador, superficie);
    SDL_Rect rect = {x, y, superficie->w, superficie->h};
    if (centrado) {
        rect.x = x - superficie->w / 2;
        rect.y = y - superficie->h / 2;
    }
    SDL_RenderCopy(renderizador, textura, NULL, &rect);
    SDL_DestroyTexture(textura);
    SDL_FreeSurface(superficie);
}

void dibujarBotonRedondeado(SDL_Renderer *renderizador, TTF_Font *fuente, SDL_Rect rect,
                            const char *texto, SDL_Color colorFondo, SDL_Color colorTexto, bool relleno) {
    if (relleno) {
        SDL_SetRenderDrawColor(renderizador, colorFondo.r, colorFondo.g, colorFondo.b, 255);
        SDL_RenderFillRect(renderizador, &rect);
    } else {
        SDL_SetRenderDrawColor(renderizador, colorFondo.r, colorFondo.g, colorFondo.b, 255);
        for (int i = 0; i < 3; i++) {
            SDL_Rect borde = {rect.x + i, rect.y + i, rect.w - i*2, rect.h - i*2};
            SDL_RenderDrawRect(renderizador, &borde);
        }
    }
    if (texto && strlen(texto) > 0) {
        dibujarTexto(renderizador, fuente, texto, rect.x + rect.w / 2, rect.y + rect.h / 2, colorTexto, 1);
    }
}

void inicializarJuego(Juego& juego) {
    juego.pantallaActual = PANTALLA_MENU;
    juego.dificultadSeleccionada = 1;
    juego.categoriaSeleccionada = 0;
    juego.numVolteadas = 0;
    juego.tiempoVoltearAtras = 0;
    juego.estaRevelando = false;
    juego.juegoIniciado = false;
    juego.configuracion.cargar("data/config.txt");
    juego.tablaLideres.cargarBinario("data/scores.bin");
}

void iniciarJuego(Juego& juego) {
    juego.pantallaActual = PANTALLA_JUEGO;
    juego.estadoJuego.reiniciar();
    juego.cartas.clear();
    juego.numVolteadas = 0;
    juego.tiempoVoltearAtras = 0;
    juego.juegoIniciado = false;

    switch (juego.dificultadSeleccionada) {
        case 0: juego.nivel = std::make_unique<NivelFacil>(); break;
        case 1: juego.nivel = std::make_unique<NivelMedio>(); break;
        case 2: juego.nivel = std::make_unique<NivelDificil>(); break;
    }

    switch (juego.categoriaSeleccionada) {
        case 0: juego.reglasPareja = std::make_unique<ReglasMatematicas>(); break;
        case 1: juego.reglasPareja = std::make_unique<ReglasGeografia>(); break;
        case 2: juego.reglasPareja = std::make_unique<ReglasVocabulario>(); break;
        case 3: juego.reglasPareja = std::make_unique<ReglasCiencias>(); break;
    }

    juego.estrategiaPuntuacion = std::make_unique<PuntuacionCombo>();

    auto parejas = juego.reglasPareja->cargarParejas("data/banco.txt");
    if (parejas.size() < (size_t)juego.nivel->obtenerNumParejas()) {
        juego.pantallaActual = PANTALLA_MENU;
        return;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(parejas.begin(), parejas.end(), g);

    for (int i = 0; i < juego.nivel->obtenerNumParejas(); i++) {
        juego.cartas.push_back(std::make_unique<CartaTexto>(parejas[i].pregunta, parejas[i].pregunta));
        juego.cartas.push_back(std::make_unique<CartaTexto>(parejas[i].respuesta, parejas[i].pregunta));
    }

    std::shuffle(juego.cartas.begin(), juego.cartas.end(), g);

    int anchoC, altoC, espacio, cols, filas;
    if (juego.dificultadSeleccionada == 0) {
        anchoC = 120; altoC = 70; espacio = 12; cols = 4; filas = 4;
    } else if (juego.dificultadSeleccionada == 1) {
        anchoC = 110; altoC = 65; espacio = 12; cols = 4; filas = 6;
    } else {
        anchoC = 100; altoC = 60; espacio = 10; cols = 6; filas = 6;
    }

    int anchoTablero = cols * anchoC + (cols - 1) * espacio;
    int altoTablero = filas * altoC + (filas - 1) * espacio;
    int inicioX = (juego.anchoP - anchoTablero) / 2;
    int inicioY = 220;

    for (size_t i = 0; i < juego.cartas.size(); i++) {
        int col = i % cols;
        int fila = i / cols;
        SDL_Rect rect = {
            inicioX + col * (anchoC + espacio),
            inicioY + fila * (altoC + espacio),
            anchoC, altoC
        };
        juego.cartas[i]->establecerRectangulo(rect);
    }

    juego.estadoJuego.establecerTiempoRestante(juego.nivel->obtenerLimiteTiempo());
    juego.estadoJuego.establecerNivel(juego.nivel->obtenerNombre());
    juego.estadoJuego.establecerCategoria(juego.reglasPareja->obtenerCategoria());
    juego.ultimoTiempo = SDL_GetTicks();

    juego.estaRevelando = true;
    juego.tiempoInicioRevelado = SDL_GetTicks();
    for (auto& carta : juego.cartas) {
        carta->voltear();
    }
}

void manejarClickCarta(Juego& juego, int indiceCarta) {
    if (juego.numVolteadas >= 2) return;
    if (juego.cartas[indiceCarta]->estaVolteada() || juego.cartas[indiceCarta]->estaEmparejada()) return;
    if (juego.estaRevelando) return;

    juego.cartas[indiceCarta]->voltear();
    juego.indicesVolteados[juego.numVolteadas] = indiceCarta;
    juego.numVolteadas++;

    if (juego.numVolteadas == 2) {
        juego.estadoJuego.incrementarIntentos();
        int idx1 = juego.indicesVolteados[0];
        int idx2 = juego.indicesVolteados[1];

        if (*juego.cartas[idx1] == *juego.cartas[idx2]) {
            juego.cartas[idx1]->establecerEmparejada(true);
            juego.cartas[idx2]->establecerEmparejada(true);
            juego.estadoJuego.establecerParejasEmparejadas(juego.estadoJuego.obtenerParejasEmparejadas() + 1);
            juego.estadoJuego.incrementarIntentosCorrectos();

            int nuevoCombo = juego.estadoJuego.obtenerComboActual() + 1;
            juego.estadoJuego.establecerComboActual(nuevoCombo);

            int puntos = juego.estrategiaPuntuacion->calcularAcierto(nuevoCombo);
            juego.estadoJuego.establecerPuntuacion(juego.estadoJuego.obtenerPuntuacion() + puntos);

            juego.numVolteadas = 0;

            if (juego.estadoJuego.obtenerParejasEmparejadas() >= juego.nivel->obtenerNumParejas()) {
                int bonus = juego.estrategiaPuntuacion->calcularBonusTiempo(juego.estadoJuego.obtenerTiempoRestante());
                juego.estadoJuego.establecerPuntuacion(juego.estadoJuego.obtenerPuntuacion() + bonus);
                juego.pantallaActual = PANTALLA_VICTORIA;
            }
        } else {
            juego.estadoJuego.establecerComboActual(1);
            int penalidad = juego.estrategiaPuntuacion->calcularFallo();
            juego.estadoJuego.establecerPuntuacion(juego.estadoJuego.obtenerPuntuacion() + penalidad);
        }
    }
}

void actualizarJuego(Juego& juego) {
    if (juego.pantallaActual != PANTALLA_JUEGO) return;

    if (juego.estaRevelando) {
        Uint32 transcurrido = SDL_GetTicks() - juego.tiempoInicioRevelado;
        if (transcurrido >= (Uint32)(juego.nivel->obtenerTiempoRevelado() * 1000)) {
            juego.estaRevelando = false;
            for (auto& carta : juego.cartas) {
                if (!carta->estaEmparejada()) {
                    carta->voltear();
                }
            }
            juego.juegoIniciado = true;
            juego.ultimoTiempo = SDL_GetTicks();
        }
        return;
    }

    if (!juego.juegoIniciado) return;

    Uint32 tiempoActual = SDL_GetTicks();
    if (tiempoActual - juego.ultimoTiempo >= 1000) {
        int nuevoTiempo = juego.estadoJuego.obtenerTiempoRestante() - 1;
        juego.estadoJuego.establecerTiempoRestante(nuevoTiempo);
        juego.estadoJuego.establecerPuntuacion(juego.estadoJuego.obtenerPuntuacion() - 1);
        juego.ultimoTiempo = tiempoActual;

        if (nuevoTiempo <= 0) {
            juego.pantallaActual = PANTALLA_VICTORIA;
        }
    }

    if (juego.numVolteadas == 2) {
        if (juego.tiempoVoltearAtras == 0) {
            juego.tiempoVoltearAtras = SDL_GetTicks();
        } else if (SDL_GetTicks() - juego.tiempoVoltearAtras > 1000) {
            int idx1 = juego.indicesVolteados[0];
            int idx2 = juego.indicesVolteados[1];
            if (!juego.cartas[idx1]->estaEmparejada()) {
                juego.cartas[idx1]->voltear();
                juego.cartas[idx2]->voltear();
            }
            juego.numVolteadas = 0;
            juego.tiempoVoltearAtras = 0;
        }
    }
}

void renderizarMenu(SDL_Renderer *renderizador, TTF_Font *fuenteGrande, TTF_Font *fuenteMediana,
                    TTF_Font *fuentePequena, Juego& juego) {
    int centroX = juego.anchoP / 2;

    dibujarTexto(renderizador, fuenteGrande, "MEMORAMA", centroX, 100, COLOR_MORADO, 1);
    dibujarTexto(renderizador, fuenteMediana, "Dificultad:", centroX, 200, COLOR_TEXTO_OSCURO, 1);

    const char *nombresDif[] = {"Fácil", "Medio", "Difícil"};
    for (int i = 0; i < 3; i++) {
        SDL_Rect r = {centroX - 240 + i * 160, 240, 140, 50};
        bool seleccionado = (juego.dificultadSeleccionada == i);
        dibujarBotonRedondeado(renderizador, fuentePequena, r, nombresDif[i],
                              seleccionado ? COLOR_MORADO : COLOR_BLANCO,
                              seleccionado ? COLOR_BLANCO : COLOR_MORADO, seleccionado);
    }

    dibujarTexto(renderizador, fuenteMediana, "Categoría:", centroX, 340, COLOR_TEXTO_OSCURO, 1);

    const char *nombresCat[] = {"Matemáticas", "Geografía", "Vocabulario", "Ciencias"};
    const char *subtitulosCat[] = {"(Operaciones)", "(Capitales)", "(Inglés)", "(Elementos)"};

    for (int i = 0; i < 4; i++) {
        SDL_Rect r = {centroX - 380 + i * 190, 380, 170, 100};
        bool seleccionado = (juego.categoriaSeleccionada == i);

        dibujarBotonRedondeado(renderizador, fuentePequena, r, "",
                              seleccionado ? COLOR_MORADO : COLOR_BLANCO,
                              seleccionado ? COLOR_BLANCO : COLOR_TEXTO_OSCURO, seleccionado);
        dibujarTexto(renderizador, fuentePequena, nombresCat[i], r.x + r.w/2, r.y + r.h/2 - 10,
                    seleccionado ? COLOR_BLANCO : COLOR_TEXTO_OSCURO, 1);
        dibujarTexto(renderizador, fuentePequena, subtitulosCat[i], r.x + r.w/2, r.y + r.h/2 + 15,
                    seleccionado ? COLOR_BLANCO : COLOR_GRIS, 1);
    }

    int statsY = 540;
    int statsEspacio = juego.anchoP / 5;

    dibujarTexto(renderizador, fuentePequena, "Tiempo", centroX - statsEspacio*1.5, statsY, COLOR_GRIS, 1);
    dibujarTexto(renderizador, fuenteMediana, "01:22", centroX - statsEspacio*1.5, statsY + 30, COLOR_MORADO, 1);

    dibujarTexto(renderizador, fuentePequena, "Precisión", centroX - statsEspacio*0.5, statsY, COLOR_GRIS, 1);
    dibujarTexto(renderizador, fuenteMediana, "100%", centroX - statsEspacio*0.5, statsY + 30, COLOR_MORADO, 1);

    dibujarTexto(renderizador, fuentePequena, "Racha", centroX + statsEspacio*0.5, statsY, COLOR_GRIS, 1);
    dibujarTexto(renderizador, fuenteMediana, "x1", centroX + statsEspacio*0.5, statsY + 30, COLOR_MORADO, 1);

    dibujarTexto(renderizador, fuentePequena, "Puntaje", centroX + statsEspacio*1.5, statsY, COLOR_GRIS, 1);
    dibujarTexto(renderizador, fuenteMediana, "0", centroX + statsEspacio*1.5, statsY + 30, COLOR_MORADO, 1);

    SDL_Rect botonJugar = {centroX - 150, 640, 300, 60};
    dibujarBotonRedondeado(renderizador, fuenteMediana, botonJugar, "INICIAR JUEGO", COLOR_MORADO, COLOR_BLANCO, true);
}

void renderizarJuego(SDL_Renderer *renderizador, TTF_Font *fuenteMediana, TTF_Font *fuentePequena,
                     TTF_Font *fuenteCarta, Juego& juego) {
    char buffer[100];
    int centroX = juego.anchoP / 2;

    SDL_Rect contenedorStats = {centroX - 400, 60, 800, 100};
    SDL_SetRenderDrawColor(renderizador, 248, 249, 250, 255);
    SDL_RenderFillRect(renderizador, &contenedorStats);

    int statsY = 80;
    int statEspacio = 200;
    int inicioX = centroX - 300;

    dibujarTexto(renderizador, fuentePequena, "Tiempo", inicioX, statsY, COLOR_GRIS, 0);
    sprintf(buffer, "%02d:%02d", juego.estadoJuego.obtenerTiempoRestante() / 60,
            juego.estadoJuego.obtenerTiempoRestante() % 60);
    dibujarTexto(renderizador, fuenteMediana, buffer, inicioX, statsY + 25, COLOR_MORADO, 0);

    dibujarTexto(renderizador, fuentePequena, "Precisión", inicioX + statEspacio, statsY, COLOR_GRIS, 0);
    sprintf(buffer, "%.0f%%", juego.estadoJuego.obtenerPrecision());
    dibujarTexto(renderizador, fuenteMediana, buffer, inicioX + statEspacio, statsY + 25, COLOR_MORADO, 0);

    dibujarTexto(renderizador, fuentePequena, "Racha", inicioX + statEspacio*2, statsY, COLOR_GRIS, 0);
    sprintf(buffer, "x%d", juego.estadoJuego.obtenerComboActual());
    dibujarTexto(renderizador, fuenteMediana, buffer, inicioX + statEspacio*2, statsY + 25, COLOR_MORADO, 0);

    dibujarTexto(renderizador, fuentePequena, "Ptaje", inicioX + statEspacio*3, statsY, COLOR_GRIS, 0);
    sprintf(buffer, "%d", juego.estadoJuego.obtenerPuntuacion());
    dibujarTexto(renderizador, fuenteMediana, buffer, inicioX + statEspacio*3, statsY + 25, COLOR_MORADO, 0);

    if (juego.estaRevelando) {
        dibujarTexto(renderizador, fuenteMediana, "¡Memoriza las cartas!", centroX, 190, COLOR_TEXTO_OSCURO, 1);
    }

    for (auto& carta : juego.cartas) {
        carta->renderizar(renderizador, fuenteCarta);
    }
}

void renderizarVictoria(SDL_Renderer *renderizador, TTF_Font *fuenteGrande, TTF_Font *fuenteMediana,
                        TTF_Font *fuentePequena, Juego& juego) {
    bool gano = juego.estadoJuego.obtenerParejasEmparejadas() >= juego.nivel->obtenerNumParejas();
    int centroX = juego.anchoP / 2;
    int centroY = juego.altoP / 2;

    dibujarTexto(renderizador, fuenteGrande, gano ? "¡VICTORIA!" : "¡DERROTA!",
                centroX, centroY - 200, COLOR_MORADO, 1);

    char buffer[100];
    sprintf(buffer, "Puntuación Final: %d", juego.estadoJuego.obtenerPuntuacion());
    dibujarTexto(renderizador, fuenteMediana, buffer, centroX, centroY - 100, COLOR_TEXTO_OSCURO, 1);

    sprintf(buffer, "Precisión: %.1f%%", juego.estadoJuego.obtenerPrecision());
    dibujarTexto(renderizador, fuenteMediana, buffer, centroX, centroY - 50, COLOR_TEXTO_OSCURO, 1);

    sprintf(buffer, "Mejor Combo: x%d", juego.estadoJuego.obtenerComboMaximo());
    dibujarTexto(renderizador, fuenteMediana, buffer, centroX, centroY, COLOR_TEXTO_OSCURO, 1);

    sprintf(buffer, "Intentos: %d", juego.estadoJuego.obtenerIntentosTotales());
    dibujarTexto(renderizador, fuenteMediana, buffer, centroX, centroY + 50, COLOR_TEXTO_OSCURO, 1);

    SDL_Rect botonJugar = {centroX - 150, centroY + 120, 300, 60};
    dibujarBotonRedondeado(renderizador, fuenteMediana, botonJugar, "Jugar de Nuevo", COLOR_MORADO, COLOR_BLANCO, true);

    SDL_Rect botonMenu = {centroX - 150, centroY + 200, 300, 60};
    dibujarBotonRedondeado(renderizador, fuenteMediana, botonMenu, "Menú Principal", COLOR_GRIS, COLOR_BLANCO, true);
}

void renderizarLideres(SDL_Renderer *renderizador, TTF_Font *fuenteGrande, TTF_Font *fuenteMediana,
                       TTF_Font *fuentePequena, Juego& juego) {
    int centroX = juego.anchoP / 2;

    dibujarTexto(renderizador, fuenteGrande, "RÉCORDS", centroX, 80, COLOR_MORADO, 1);

    auto registros = juego.tablaLideres.obtenerMejoresRegistros(10);

    int y = 180;
    for (size_t i = 0; i < registros.size() && i < 10; i++) {
        char buffer[200];
        sprintf(buffer, "%zu. %s - %d pts (%.1f%%) x%d",
                i + 1, registros[i].nombreJugador.c_str(), registros[i].puntuacion,
                registros[i].precision, registros[i].comboMaximo);
        dibujarTexto(renderizador, fuentePequena, buffer, centroX, y, COLOR_TEXTO_OSCURO, 1);
        y += 40;
    }

    SDL_Rect botonVolver = {centroX - 100, juego.altoP - 100, 200, 50};
    dibujarBotonRedondeado(renderizador, fuenteMediana, botonVolver, "Volver", COLOR_MORADO, COLOR_BLANCO, true);
}

int SDL_main(int argc, char *argv[]) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        return 1;
    }

    SDL_DisplayMode dm;
    SDL_GetDesktopDisplayMode(0, &dm);

    SDL_Window *ventana = SDL_CreateWindow("Memorama",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        dm.w, dm.h, SDL_WINDOW_FULLSCREEN_DESKTOP);

    SDL_Renderer *renderizador = SDL_CreateRenderer(ventana, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font *fuenteGrande = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 48);
    TTF_Font *fuenteMediana = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    TTF_Font *fuentePequena = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 18);
    TTF_Font *fuenteCarta = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 13);

    if (!fuenteGrande || !fuenteMediana || !fuentePequena || !fuenteCarta) {
        return 1;
    }

    Juego juego;
    juego.anchoP = dm.w;
    juego.altoP = dm.h;
    inicializarJuego(juego);

    int ejecutando = 1;
    SDL_Event evento;

    while (ejecutando) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                ejecutando = 0;
            } else if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE) {
                ejecutando = 0;
            } else if (evento.type == SDL_MOUSEBUTTONDOWN) {
                if (juego.pantallaActual == PANTALLA_MENU) {
                    int centroX = juego.anchoP / 2;
                    for (int i = 0; i < 3; i++) {
                        SDL_Rect r = {centroX - 240 + i * 160, 240, 140, 50};
                        if (mouseX >= r.x && mouseX <= r.x + r.w &&
                            mouseY >= r.y && mouseY <= r.y + r.h) {
                            juego.dificultadSeleccionada = i;
                        }
                    }
                    for (int i = 0; i < 4; i++) {
                        SDL_Rect r = {centroX - 380 + i * 190, 380, 170, 100};
                        if (mouseX >= r.x && mouseX <= r.x + r.w &&
                            mouseY >= r.y && mouseY <= r.y + r.h) {
                            juego.categoriaSeleccionada = i;
                        }
                    }
                    SDL_Rect botonJugar = {centroX - 150, 640, 300, 60};
                    if (mouseX >= botonJugar.x && mouseX <= botonJugar.x + botonJugar.w &&
                        mouseY >= botonJugar.y && mouseY <= botonJugar.y + botonJugar.h) {
                        iniciarJuego(juego);
                    }
                } else if (juego.pantallaActual == PANTALLA_JUEGO) {
                    for (size_t i = 0; i < juego.cartas.size(); i++) {
                        SDL_Rect r = juego.cartas[i]->obtenerRectangulo();
                        if (mouseX >= r.x && mouseX <= r.x + r.w &&
                            mouseY >= r.y && mouseY <= r.y + r.h) {
                            manejarClickCarta(juego, i);
                        }
                    }
                } else if (juego.pantallaActual == PANTALLA_VICTORIA) {
                    int centroX = juego.anchoP / 2;
                    int centroY = juego.altoP / 2;
                    SDL_Rect botonJugar = {centroX - 150, centroY + 120, 300, 60};
                    if (mouseX >= botonJugar.x && mouseX <= botonJugar.x + botonJugar.w &&
                        mouseY >= botonJugar.y && mouseY <= botonJugar.y + botonJugar.h) {
                        iniciarJuego(juego);
                    }
                    SDL_Rect botonMenu = {centroX - 150, centroY + 200, 300, 60};
                    if (mouseX >= botonMenu.x && mouseX <= botonMenu.x + botonMenu.w &&
                        mouseY >= botonMenu.y && mouseY <= botonMenu.y + botonMenu.h) {
                        juego.pantallaActual = PANTALLA_MENU;
                    }
                } else if (juego.pantallaActual == PANTALLA_LIDERES) {
                    int centroX = juego.anchoP / 2;
                    SDL_Rect botonVolver = {centroX - 100, juego.altoP - 100, 200, 50};
                    if (mouseX >= botonVolver.x && mouseX <= botonVolver.x + botonVolver.w &&
                        mouseY >= botonVolver.y && mouseY <= botonVolver.y + botonVolver.h) {
                        juego.pantallaActual = PANTALLA_MENU;
                    }
                }
            }
        }

        actualizarJuego(juego);

        SDL_SetRenderDrawColor(renderizador, COLOR_FONDO.r, COLOR_FONDO.g, COLOR_FONDO.b, 255);
        SDL_RenderClear(renderizador);

        if (juego.pantallaActual == PANTALLA_MENU) {
            renderizarMenu(renderizador, fuenteGrande, fuenteMediana, fuentePequena, juego);
        } else if (juego.pantallaActual == PANTALLA_JUEGO) {
            renderizarJuego(renderizador, fuenteMediana, fuentePequena, fuenteCarta, juego);
        } else if (juego.pantallaActual == PANTALLA_VICTORIA) {
            renderizarVictoria(renderizador, fuenteGrande, fuenteMediana, fuentePequena, juego);
        } else if (juego.pantallaActual == PANTALLA_LIDERES) {
            renderizarLideres(renderizador, fuenteGrande, fuenteMediana, fuentePequena, juego);
        }

        SDL_RenderPresent(renderizador);
        SDL_Delay(16);
    }

    juego.configuracion.guardar("data/config.txt");
    juego.tablaLideres.guardarBinario("data/scores.bin");
    juego.tablaLideres.exportarTexto("data/scores.txt");
    juego.estadoJuego.guardar("data/gamestate.bin");

    TTF_CloseFont(fuenteGrande);
    TTF_CloseFont(fuenteMediana);
    TTF_CloseFont(fuentePequena);
    TTF_CloseFont(fuenteCarta);
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(ventana);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
