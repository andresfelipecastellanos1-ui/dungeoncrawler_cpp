@echo off
echo ========================================
echo     COMPILANDO DUNGEON CRAWLER
echo ========================================

cd build
mingw32-make clean
mingw32-make

if exist dungeon_crawler.exe (
    echo.
    echo ¡COMPILACION EXITOSA!
    echo.
    dungeon_crawler.exe
) else (
    echo.
    echo ERROR en la compilacion.
    echo.
)

pause