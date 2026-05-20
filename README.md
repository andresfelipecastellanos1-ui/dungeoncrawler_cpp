# Dungeon Crawler

## Integrantes
- Andres Felipe Castellanos Forero

## Requisitos Mínimos Cumplidos
- Personaje controlable con colisiones funcionales
- 8 habitaciones conectadas usando punteros
- 3 enemigos con 2 tipos de comportamiento (persecución)
- Sistema de inventario de un solo espacio (recoger/soltar)
- Condiciones claras de victoria y derrota
- Laberintos con paredes internas en las habitaciones
- Código modular en múltiples archivos (.h y .cpp)
- Compilación con CMake
- Solo arreglos estáticos (sin new/delete)

## Controles

- **WASD** → Mover
- **E** → Recoger llave
- **Q** → Salir
## hola

## Cómo Compilar y Ejecutar (Windows)

```bash
cd build
mingw32-make
.\dungeon_crawler.exe

## Cómo Compilar y Ejecutar (linux/macOS)

```bash
cd build
cmake ..
make
./dungeon_crawler
