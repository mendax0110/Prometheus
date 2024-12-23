#include "ASTProcessorFactory.h"

std::unique_ptr<ASTProcessor> ASTProcessorFactory::createProcessor(const std::string& languageStandard, const std::string& outputPath)
{
    return std::make_unique<ASTProcessor>(outputPath, languageStandard);
}
