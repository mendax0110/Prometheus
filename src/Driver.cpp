#include "Driver.h"
#include "FileUtils.h"
#include <llvm/Support/FileSystem.h>
#include <iostream>
#include <utility>
#include <sstream>

Driver::Driver(std::string inputPath, std::string  outputPath, std::string  languageStandard,
               bool verbose, bool dryRun, std::string  flagsFile)
        : inputPath(std::move(inputPath)), outputPath(std::move(outputPath)), languageStandard(std::move(languageStandard)),
          verbose(verbose), dryRun(dryRun), flagsFile(std::move(flagsFile)) {}

bool Driver::run()
{
    if (llvm::sys::fs::is_directory(inputPath))
    {
        processDirectory(inputPath);
    }
    else
    {
        processFile(inputPath);
    }
    return true;
}

void Driver::processFile(const std::string& filePath)
{
    std::string fileContent = FileUtils::readFile(filePath);
    if (fileContent.empty())
    {
        llvm::errs() << "Skipping empty or unreadable file: " << filePath << "\n";
        return;
    }

    auto processor = ASTProcessorFactory::createProcessor(languageStandard, outputPath, verbose, dryRun);
    std::vector<std::string> compilerArgs = getCompilerFlags();
    std::vector<std::string> linkerArgs = getLinkerFlags();
    for (const auto& flag : compilerArgs) {
        llvm::outs() << "Compiler flag: " << flag << "\n";
    }

    for (const auto& flag : linkerArgs) {
        llvm::outs() << "Linker flag: " << flag << "\n";
    }
    // Combine both compiler and linker flags into one
    compilerArgs.insert(compilerArgs.end(), linkerArgs.begin(), linkerArgs.end());
    processor->process(fileContent, filePath, compilerArgs);
}

void Driver::processDirectory(const std::string& dirPath)
{
    auto files = FileUtils::getFilesInDirectory(dirPath);
    for (const auto& file : files)
    {
        processFile(file);
    }
}

std::vector<std::string> Driver::getCompilerFlags()
{
    std::vector<std::string> compilerArgs = {
            "-std=" + languageStandard,                    // Set C++ language standard
            "-stdlib=libc++",                              // Use the C++ standard library
            "-isysroot", "/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk", // SDK root
            "-I/opt/homebrew/opt/llvm/include/c++/v1",     // Include Homebrew's LLVM C++ standard library
            "-I/opt/homebrew/Cellar/llvm/19.1.4/lib/clang/19/include", // Clang headers
            "-I/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk/usr/include", // macOS system headers
            "-I/opt/homebrew/include",                    // Homebrew default include path
            "-I/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk/usr/local/include", // Ensure sysheaders are included
            "-I/Library/Developer/CommandLineTools/SDKs/MacOSX14.sdk/Library/Frameworks",  // Include frameworks if needed
            "-D_GNU_SOURCE",
            "-D__STDC_WANT_LIB_EXT1__=1",
            "-D_LIBCPP_DISABLE_AVAILABILITY",
    };

    if (!flagsFile.empty())
    {
        auto flags = FileUtils::readFile(flagsFile);
        if (!flags.empty())
        {
            std::istringstream stream(flags);
            std::string flag;
            while (std::getline(stream, flag, '\n'))
            {
                compilerArgs.push_back(flag);
            }
        }
    }

    return compilerArgs;
}

std::vector<std::string> Driver::getLinkerFlags()
{
    std::vector<std::string> linkerArgs = {
            "-L/opt/homebrew/opt/llvm/lib",
            "-lc++",
            "-L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib",
            "-lSystem"
    };

    return linkerArgs;
}