#ifndef ASTPROCESSORFACTORY_H
#define ASTPROCESSORFACTORY_H

#include <memory>
#include <string>
#include "ASTProcessor.h"

/// @brief Factory class for creating ASTProcessor objects
class ASTProcessorFactory
{
public:
    /**
     * @brief Create an ASTProcessor object
     * @param languageStandard -> The language standard to use
     * @param outputPath -> The path to the output directory
     * @return -> A unique pointer to the created ASTProcessor object
     */
    static std::unique_ptr<ASTProcessor> createProcessor(const std::string& languageStandard,
                                                         const std::string& outputPath,
                                                         bool verbose, bool dryRun);
};

#endif // ASTPROCESSORFACTORY_H
