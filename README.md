
# Snake Game in C++ with SDL2

A classic Snake game implemented in C++ using SDL2 for graphics and SDL2_ttf for font rendering.

## Getting Started

This is a simple Snake game implemented in C++ using SDL2 and SDL2_ttf libraries.

### Prerequisites
Ensure you have the following installed:
- C++ Compiler (e.g., g++)
- [SDL2](https://www.libsdl.org/) (version 2.0 or higher)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/) (version 2.0 or higher)

You can typically install SDL2 and SDL2_ttf using your system's package manager. For example, on Ubuntu:
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev
```
On macOS using Homebrew:
```bash
brew install sdl2 sdl2_ttf
```

## Project Structure

```
├── snake.cc        # Main C++ source file for the Snake game.
├── Makefile             # Makefile for building the project.

```

### Installation
1. Clone the repository:
   ```bash
   git clone git@github.com:Luke23-45/Coding-a-Snake-game-with-SDL2.git
   ```
2. Navigate to the project directory:
   ```bash
   cd Coding-a-Snake-game-with-SDL2
   ```

## Building

This project uses `make` for building. To build the project, run the following command in your terminal:

1. Ensure you are in the project directory (`Colorfull-Ball-with-Particle-Effect-Animation`).
2. Compile the code using `make`:
   ```bash
   make
   ```
   This command will compile `snake_game.cc` using the `Makefile` and create an executable named `snake_game`.

3. Run the executable:
   ```bash
   ./main
   ```
   Or if you are on Windows and `make` configured for Windows:
   ```bash
  main.exe
   ```

4. To clean up the build artifacts (object files and executable):
   ```bash
   make clean
   ```

## Features

- Classic Snake game logic: Control a snake to eat food and grow longer.
- Score tracking: Displays the current score on the screen.
- Wall and self-collision detection: Game ends upon collision with walls or the snake's own body.
- Food generation: Food appears randomly in locations not occupied by the snake.
- Keyboard controls for snake movement (Up, Down, Left, Right arrow keys).
- Score display using SDL2_ttf for a better visual experience.
- Grid-based movement for a retro game feel.

## Key Controls

| Action            | Key        |
| ----------------- | ---------- |
| Exit Game         | `ESC` key  |
| Move Up           | `Up Arrow` |
| Move Right        | `Right Arrow`|
| Move Down         | `Down Arrow` |
| Move Left         | `Left Arrow` |


## Code Structure

The project consists of a single C++ source file, `snake.cc`, which contains all the game logic and rendering code.

- **`snake.cc`**:
    - Includes necessary headers: `SDL2/SDL.h`, `SDL2/SDL_ttf.h`, `<iostream>`, `<deque>`, `<cstdlib>`, `<string>`, `<algorithm>`.
    - Defines constants for window dimensions (`WINDOW_WIDTH`, `WINDOW_HEIGHT`), tile size (`TILE_SIZE`), and update interval (`UPDATE_INTERVAL_MS`).
    - Implements `Segment` structure to represent snake segments with `x` and `y` coordinates.
    - Implements `Food` structure to represent food with `x` and `y` coordinates.
    - Includes helper functions:
        - `Collision()`: Checks collision between snake segment and food.
        - `SDL_RenderFillCircle()`: Renders a filled circle using SDL2 rendering functions.
        - `GenerateFood()`: Generates a new food location that does not intersect with the snake.
    - `main()` function:
        - Initializes SDL and SDL_ttf.
        - Creates a window and renderer using SDL2.
        - Loads a font using SDL2_ttf for score display.
        - Initializes the snake as a deque of `Segment` objects.
        - Sets up game variables: direction, food position, score, game state.
        - Enters the main game loop, which handles:
            - Event processing (keyboard input for direction control and exit).
            - Fixed timestep game logic updates: snake movement, collision detection, food spawning, score update.
            - Rendering: clearing the screen, drawing grid lines, snake segments, food, and score text.
        - Cleans up resources: frees memory, closes font, destroys renderer and window, quits SDL and SDL_ttf.


## Demo Video
*Demo video link will be added here if available.*

## License

This project is licensed under the MIT License. Feel free to use, modify, and distribute the code.

## Acknowledgements

- SDL2 for graphics rendering and event handling.
- SDL2_ttf for font rendering and text display.

