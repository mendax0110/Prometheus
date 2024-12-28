# Prometheus

Prometheus is a powerful and flexible C++ code transformation tool designed to process and modify C++ source code efficiently. It integrates with Clang’s AST (Abstract Syntax Tree) to identify deprecated functions and apply custom transformations.

## Features

- **AST Processing:** Analyze and modify C++ code via the Abstract Syntax Tree (AST).
- **Code Transformations:** Replace deprecated functions with modern alternatives (e.g., replacing `auto_ptr` with `std::unique_ptr`).
- **Flexible Configuration:** Dynamically choose the C++ language standard (e.g., C++11, C++14, C++17, etc.).
- **Batch Processing:** Process individual files or entire directories of C++ source code files.
- **Rewriting Code:** The program can rewrite and output transformed source code to specified directories.
- **Easy to Use:** Simple command-line interface for seamless operation.

## Build Instructions

To build and run Prometheus, follow these steps:

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/mendax0110/Prometheus.git
    ```

2. **Change Directory to the Cloned Repository:**
    ```bash
    cd Prometheus
    ```

3. **Add CLI11 as a Submodule in the `external` Folder:**
   Prometheus uses the CLI11 library for the command-line interface. You will need to add it as a Git submodule:
    ```bash
    git submodule add https://github.com/CLIUtils/CLI11.git external/CLI11
    ```

4. **Initialize and Clone the Submodule:**
   After adding the submodule, initialize and clone it:
    ```bash
    git submodule update --init --recursive
    ```

5. **Create the Build Directory:**
    ```bash
    mkdir build
    ```

6. **Change Directory to the Build Directory:**
    ```bash
    cd build
    ```

7. **Generate CMake Files:**
    ```bash
    cmake ..
    ```

8. **Build the Project:**
    ```bash
    cmake --build .
    ```

## Usage

### Command-Line Interface

Prometheus can be run from the command line with several configurable options. Here's the general command format:

```bash
./Prometheus --input <input_path> --output <output_path> --standard <c++_version> [options]
