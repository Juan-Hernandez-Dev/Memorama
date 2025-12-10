# Juego de Memoria Educativo - SDL2

Juego de memoria (memorama) educativo implementado en C++ con SDL2, siguiendo principios de Programación Orientada a Objetos.

## Características Implementadas

### ✅ Arquitectura POO Completa

- **Jerarquía de Niveles**: `Level` → `LevelEasy`, `LevelMedium`, `LevelHard`
- **Jerarquía de Reglas de Parejas**: `PairRule` → `MathPairRule`, `GeoPairRule`, `VocabPairRule`, `SciencePairRule`
- **Jerarquía de Cartas**: `Card` → `TextCard`
- **Estrategias de Puntuación**: `ScoringStrategy` → `ClassicScore`, `ComboScore`, `SpeedScore`
- **Sobrecarga de Operadores**: `operator==` para comparar cartas, `operator<` para ordenar récords

### ✅ Sistema de Persistencia

- **Banco de Preguntas** (`data/banco.txt`): 165+ pares educativos en 4 categorías
- **Configuración** (`data/config.txt`): Parámetros del juego configurables
- **Récords Binarios** (`data/scores.bin`): Guardado binario de puntuaciones
- **Récords Texto** (`data/scores.txt`): Exportación legible de top 10
- **Estado del Juego** (`data/gamestate.bin`): Guardado automático del estado

### ✅ Mecánicas del Juego

- **3 Niveles de Dificultad**:

  - Fácil: 8 pares, 90 segundos, revelado 5s
  - Medio: 12 pares, 120 segundos, revelado 4s
  - Difícil: 18 pares, 180 segundos, revelado 3s

- **4 Categorías Educativas**:

  - Matemáticas (45 pares)
  - Geografía (38 pares)
  - Vocabulario Inglés-Español (52 pares)
  - Ciencias - Elementos Químicos (30 pares)

- **Sistema de Puntuación**:

  - +100 puntos por acierto
  - +50 puntos adicionales por combo
  - -10 puntos por fallo
  - -1 punto por segundo transcurrido
  - Bonus de tiempo al finalizar

- **Revelado Inicial**: Todas las cartas se muestran al inicio según el tiempo del nivel

### ✅ Interfaz Gráfica SDL2

- Menú principal con selección de dificultad y categoría
- Pantalla de juego con estadísticas en tiempo real
- Pantalla de victoria/derrota con estadísticas finales
- Tabla de récords

## Compilación y Ejecución

### Requisitos

- Windows con vcpkg instalado
- SDL2 y SDL2_ttf (instalados vía vcpkg)
- CMake 3.15+
- Compilador C++17

### Compilar y Ejecutar

```cmd
run.bat
```

El script automáticamente:

1. Configura CMake con vcpkg
2. Compila el proyecto
3. Ejecuta el juego

## Estructura del Proyecto

```
.
├── include/           # Headers de clases POO
│   ├── Card.h
│   ├── Config.h
│   ├── GameState.h
│   ├── Leaderboard.h
│   ├── Level.h
│   ├── PairRule.h
│   └── ScoringStrategy.h
├── src/              # Implementaciones
│   ├── main.cpp
│   ├── Card.cpp
│   ├── Config.cpp
│   ├── GameState.cpp
│   ├── Leaderboard.cpp
│   └── PairRule.cpp
├── data/             # Archivos de datos
│   ├── banco.txt     # Banco de preguntas
│   ├── config.txt    # Configuración
│   ├── scores.bin    # Récords (binario)
│   ├── scores.txt    # Récords (texto)
│   └── gamestate.bin # Estado del juego
├── ui/               # Diseños de pantallas
├── CMakeLists.txt
├── run.bat
└── README.md
```

## Cómo Jugar

1. **Menú Principal**: Selecciona dificultad y categoría
2. **Inicio del Juego**: Memoriza las cartas durante el revelado inicial
3. **Juego**: Haz clic en dos cartas para voltearlas
4. **Objetivo**: Encuentra todos los pares antes de que se acabe el tiempo
5. **Puntuación**: Mantén combos altos para maximizar tu puntaje

## Controles

- **Mouse**: Clic para seleccionar cartas y botones
- **ESC**: Cerrar ventana

## Características Técnicas

- **Polimorfismo**: Uso extensivo de clases base abstractas
- **Herencia**: Jerarquías de clases bien definidas
- **Encapsulamiento**: Datos privados con getters/setters
- **Smart Pointers**: Uso de `std::unique_ptr` para gestión de memoria
- **STL**: Uso de `std::vector`, `std::map`, `std::string`
- **Persistencia**: Lectura/escritura binaria y texto
- **Validaciones**: Manejo de archivos faltantes con valores por defecto

## Autor

Proyecto educativo - Juego de Memoria con SDL2
