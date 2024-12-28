#include "Driver.h"
#include <CLI/CLI.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    CLI::App app{"Prometheus"};
    std::string inputPath;
    std::string outputPath;
    std::string languageStandard = "c++14";
    bool verbose = false;
    bool dryRun = false;
    std::string flagsFile;

    app.add_option("-i,--input", inputPath, "Input path (file or directory)")->required();
    app.add_option("-o,--output", outputPath, "Output directory")->required();
    app.add_option("--standard", languageStandard, "C++ language standard")->check(CLI::IsMember({"c++11", "c++14", "c++17", "c++20"}));
    app.add_flag("-v,--verbose", verbose, "Enable verbose output");
    app.add_flag("--dry-run", dryRun, "Preview transformations without modifying files");
    app.add_option("--flags-file", flagsFile, "File containing additional compile flags");

    CLI11_PARSE(app, argc, argv);

    std::cout << "Input Path: " << inputPath << "\n";
    std::cout << "Output Path: " << outputPath << "\n";
    std::cout << "Language Standard: " << languageStandard << "\n";
    std::cout << "Verbose: " << verbose << "\n";
    std::cout << "Dry Run: " << dryRun << "\n";


    Driver driver(inputPath, outputPath, languageStandard, verbose, dryRun, flagsFile);
    if (!driver.run())
    {
        std::cerr << "Error during transformation.\n";
        return 1;
    }

    std::cout << "Transformation completed successfully.\n";
    return 0;
}
