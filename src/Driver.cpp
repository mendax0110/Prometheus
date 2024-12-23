#include "Driver.h"
#include "FileUtils.h"
#include <llvm/Support/FileSystem.h>
#include <iostream>

Driver::Driver(const std::string& inputPath, const std::string& outputPath, const std::string& languageStandard)
        : inputPath(inputPath), outputPath(outputPath), languageStandard(languageStandard)
{

}

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
    auto processor = ASTProcessorFactory::createProcessor(languageStandard, outputPath);
    processor->process(FileUtils::readFile(filePath));
}

void Driver::processDirectory(const std::string& dirPath)
{
    auto files = FileUtils::getFilesInDirectory(dirPath);
    for (const auto& file : files)
    {
        processFile(file);
    }
}
