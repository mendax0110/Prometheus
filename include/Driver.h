#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include "ASTProcessorFactory.h"

/// @brief The main driver class for the source-to-source transformation tool
class Driver
{
public:
    /**
     * @brief Construct a new Driver object
     * @param inputPath -> The path to the input file or directory
     * @param outputPath -> The path to the output directory
     * @param languageStandard -> The language standard to use
     */
    Driver(const std::string& inputPath, const std::string& outputPath, const std::string& languageStandard);

    /**
     * @brief Run the transformation process
     * @return -> True if the transformation was successful, false otherwise
     */
    bool run();

private:
    std::string inputPath;
    std::string outputPath;
    std::string languageStandard;

    /**
     * @brief Process a single file
     * @param filePath -> The path to the file to process
     */
    void processFile(const std::string& filePath);

    /**
     * @brief Process a directory
     * @param dirPath -> The path to the directory to process
     */
    void processDirectory(const std::string& dirPath);
};

#endif // DRIVER_H