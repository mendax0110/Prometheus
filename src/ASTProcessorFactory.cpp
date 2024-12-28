#include "ASTProcessorFactory.h"

std::unique_ptr<ASTProcessor> ASTProcessorFactory::createProcessor(const std::string& languageStandard,
                                                                   const std::string& outputPath,
                                                                   bool verbose, bool dryRun)
{
    return std::make_unique<ASTProcessor>(outputPath, languageStandard, verbose, dryRun);
}

