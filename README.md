# ConsoleBattleship

A battleship game for Console (Linux, Windows)

## Gameplay
![image](https://github.com/user-attachments/assets/74707ee6-3ace-4226-9343-1bb349b3798c)

## Compile & Run
### Linux
- Required packages: `cmake`, `make`, `gcc`, `libncurses-dev`

Compile & Run
1. `cmake -DCMAKE_BUILD_TYPE=Release -S . -B release`
2. `cmake --build release`
3. `release/Battleship`

### Windows
Required programs:
1. Install cmake and add it to $PATH
2. Install MinGW and add it to $PATH

Compile & Run
1. `cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -S . -B release`
2. `cmake --build release`
3. Go into the `release` folder and double click `Battleship.exe`
