# cpp_physics_engine

A modular and efficient C++ physics simulation engine designed for educational and experimental purposes. This project provides a framework for simulating physical entities and their interactions, with a focus on extensibility and performance.

## Features

- 2D physics simulation with support for various entity types (boxes, circles, edges, etc.)
- Modular entity system for easy extension
- Collision detection and response
- Example simulations and stress tests
- CMake-based build system

## Getting Started

### Prerequisites

- C++17 compatible compiler
- CMake 3.10 or higher
- SDL2 and SDL2_gfx libraries (for visualization/examples)

### Building

```sh
git clone https://github.com/Galfurian/cpp_physics_sim.git
cd cpp_physics_sim
mkdir build
cd build
cmake ..
make
```

### Running Examples

After building, you can run the example simulations:

```sh
./examples/ramp
./examples/stress_test
```

## Project Structure

- `include/` – Header files for the engine and entities
- `examples/` – Example simulation programs
- `cmake/` – CMake modules and find scripts
- `build/` – Build output (not tracked in version control)

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, new features, or improvements.

## License

This project is licensed under the MIT License. See `LICENSE.md` for details.
