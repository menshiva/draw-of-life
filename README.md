<h1 align="center">
    Draw of Life<br>
    <a href="https://www.codacy.com/gh/menshiva/draw-of-life/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=menshiva/draw-of-life&amp;utm_campaign=Badge_Grade">
        <img alt="Code quality"
            src="https://app.codacy.com/project/badge/Grade/737cc72a9a0d45ba8ac49d29b3ddd968">
    </a>
</h1>
<p align="center">
    <a href="#key-features-">Key Features 🍪</a> |
    <a href="#requirements-">Requirements 🧬</a> |
    <a href="#build-and-run-">Build and Run 🚀</a> |
    <a href="#controls-">Controls 📃</a> |
    <a href="#credits-">Credits ✍</a>
</p>
<h1 align="center"><img src="demo/demo.gif" alt="Preview"></h1>

## Key Features 🍪

### Cell data packing
Cell data (such as color, number of alive neighbors, etc.) is packed in a single 32-bit unsigned integer.
Used for saving memory and simplifying the algorithm for next epoch generation.

| 00000000  | 00000000  | 00000000  | 000         | 0000            | 0        |
|-----------|-----------|-----------|-------------|-----------------|----------|
| R channel | G channel | B channel | Unused bits | Alive neighbors | Is alive |

You can see the exact implementation in [cells.h](src/cells.h) file.

### Generations history buffer

Current epoch is stored in buffer before the new one is generated, so you are able to return to previous generation(s)
or undo changes.

You can see the exact implementation in [gens_buffer.h](src/gens_buffer.h) file.

Buffer size is 128 by default (which means that it can hold current generation and 127 previous generations).
You can change it in [preference file](src/prefs.h).

### Averaging neighbor colors

When cell becomes a live cell on next generation we need to assign color to it. My approach is to get average color
of all alive neighbor cells.

You can see the exact implementation in `generateCellHexColor` function in [cells.h](src/cells.h) file.

## Requirements 🧬

* C11 compiler
* [CMake](https://cmake.org/)
* [SDL2](https://www.libsdl.org/)

## Build and Run 🚀

### Preferences (optional)

You can also customize preferences before compilation by modifying [prefs.h](src/prefs.h) file.

### Install SDL2

#### Windows (Visual Studio)

No actions needed.

#### Linux

Download and install **libsdl2-dev** via your package manager, for example:

```bash
sudo apt install libsdl2-dev
```

### Build and run the application

```bash
mkdir build
cd build
cmake .. && make all
./DrawOfLife
```

## Controls 📃

### Mouse
* **Left mouse button click (hold) on grid:** Draw with selected color (mark cell(s) as alive).
* **Left mouse button click on color palette:** Change draw color.
* **Right mouse button click (hold) on grid:** Erase (mark cell(s) as dead).

### Keyboard
* **Left arrow / Right arrow:** Change draw color.
* **Up arrow / Down arrow:** Next / previous generation.
* **G:** Toggle grid.
* **C:** Clear all (mark all cells as dead).
* **Space:** Start / stop animation with generating next epochs.
* **Esc:** Close app.

## Credits ✍

### Third-party libraries / frameworks

* [SDL2](https://www.libsdl.org/)

### Used algorithms

* [Conway's Game of Life rules](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)
* [Averaging RGB colors](https://sighack.com/post/averaging-rgb-colors-the-right-way)

### Author

* Ivan Menshikov (menshiva@fit.cvut.cz)
