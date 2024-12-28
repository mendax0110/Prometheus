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
    if (fileContent.empty()) {
        llvm::errs() << "Skipping empty or unreadable file: " << filePath << "\n";
        return;
    }

    auto processor = ASTProcessorFactory::createProcessor(languageStandard, outputPath, verbose, dryRun);
    std::vector<std::string> compileArgs = getCompileFlags();
    processor->process(fileContent, filePath, compileArgs);
}

void Driver::processDirectory(const std::string& dirPath)
{
    auto files = FileUtils::getFilesInDirectory(dirPath);
    for (const auto& file : files)
    {
        processFile(file);
    }
}

std::vector<std::string> Driver::getCompileFlags()
{
    std::vector<std::string> compileArgs = {
            "-std=" + languageStandard,
            "-stdlib=libc++",
            "-isysroot", "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk",
            "-I/opt/homebrew/opt/llvm/include/c++/v1",
            "-I/opt/homebrew/Cellar/llvm/19.1.4/lib/clang/19/include",
            "-I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include",
            //"-L/opt/homebrew/opt/llvm/lib",
            //"-lc++",
            "-I/opt/homebrew/include",
            //"-L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib",
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
                compileArgs.push_back(flag);
            }
        }
    }

    return compileArgs;
}