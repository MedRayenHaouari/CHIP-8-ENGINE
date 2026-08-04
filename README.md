# CHIP-8-ENGINE

A high-performance, flexible, single-header (`stb`-style) CHIP-8 core and testing platform written in C.

## Downloads

* **To download the core.h file**
```bash
wget https://raw.githubusercontent.com/MedRayenHaouari/CHIP-8-ENGINE/main/core/core.h
```

* **To download the whole program**
```bash
wget https://github.com/MedRayenHaouari/CHIP-8-ENGINE.git
```


## Features

* **Single-Header (`stb`-style) Integration:** Drop `core.h` into your project and define the implementation in a single translation unit.
* **Dual Architecture Modes:** Switch between **Legacy** (original 1970s VIP quirks for vintage ROM compatibility) and **Modern** execution semantics.
* **Scalable Performance Profiles:** 
  * `FAST`: Utilizes speed optimizations and pre-computed lookup tables (`LUT`) for accelerated rendering.
  * `SMALL`: Minimizes memory footprint with a compact 1-bit-per-pixel layout and many memory optimizations.
  * `DEFAULT`: Balanced performance and standard pixel buffers.

## Getting Started

### Option1:
In one of your C source files, define the implementation macro and your configuration flags before including the header:

```c
#define CHIP8_IMPLEMENTATION // for chip 8 implementation (functions)
#define CHIP8_MODERN // Or define CHIP8_LEGACY depending on your target ROMs
#define FAST         // Optional: Enable high-performance lookup table mode or you can choose the SMALL flag for a small binary size and usage

#include "core.h"
```

### Option2:

Configure your build using preprocessor definitions to shape the engine to your target environment:

* **Architectures:**
  * `-DCHIP8_MODERN`: Enables modern shift, bitwise, jump offset, and index register behaviors.
  * `-DCHIP8_LEGACY`: Restores legacy hardware behaviors.
* **Performance Optimizations: (Optional)**
  * `-DFAST`: Enables fast rendering optimizations via pre-calculated lookup tables and more.
  * `-DSMALL`: Compacts video RAM allocation for constrained systems.

## Project Structure

* `core/core.h` — The core single-header CHIP-8 engine
* `include/` — Contains costum libraries for personalized types, color and symbols to use when printing to console
* `platform/` — Contains main program where you can run Chip 8 ROMs using the engine
* `build.sh` — For building the full program. Flags must be provided

## License

This project is open-source and available under the MIT License.
