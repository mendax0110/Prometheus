#include "Driver.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: <input_path> <output_path> <language_standard>\n";
        return 1;
    }

    Driver driver(argv[1], argv[2], argv[3]);
    if (!driver.run())
    {
        std::cerr << "Error during transformation.\n";
        return 1;
    }

    std::cout << "Transformation completed successfully.\n";
    return 0;
}
