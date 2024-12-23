# Prometheus

Prometheus is a powerful and flexible C++ code transformation tool designed to process and modify C++ source code efficiently.
It integrates with Clang’s AST (Abstract Syntax Tree) to identify deprecated functions and apply custom transformations.

## Features

- **AST Processing:** Analyze and modify C++ code via the Abstract Syntax Tree (AST).
- **Code Transformations:** Replace deprecated functions with modern alternatives (e.g., replacing `auto_ptr` with `std::unique_ptr`).
- **Flexible Configuration:** Dynamically choose the C++ language standard (e.g., C++11, C++14, C++17, etc.).
- **Batch Processing:** Process individual files or entire directories of C++ source code files.
- **Rewriting Code:** The program can rewrite and output transformed source code to specified directories.
- **Easy to Use:** Simple command-line interface for seamless operation.

### Build Instructions

To build and run Prometheus, follow these steps:

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/mendax0110/Prometheus.git
    ```

2. **Change Directory to the Cloned Repository:**
    ```bash
    cd Prometheus
    ```

3. **Create the Build Directory:**
    ```bash
    mkdir build
    ```

4. **Change Directory to the Build Directory:**
    ```bash
    cd build
    ```

5. **Generate CMake Files:**
    ```bash
    cmake ..
    ```

6. **Build the Project:**
    ```bash
    cmake --build .
    ```

### Usage
```bash
./Prometheus <input_path> <output_path> -std=<c++_version>
```