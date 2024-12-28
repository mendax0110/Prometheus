#ifndef ASTPROCESSOR_H
#define ASTPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include <clang/Frontend/FrontendAction.h>
#include "TransformationCommand.h"

/// @brief Class for processing the AST of a source file
class ASTProcessor
{
public:
    /**
     * @brief Construct a new ASTProcessor object
     * @param outputPath -> The path to the output directory
     */
    explicit ASTProcessor(std::string outputPath, std::string languageStandard, bool verbose, bool dryRun);

    /**
     * @brief Process the source code
     * @param sourceCode -> The source code to process
     */
    void process(const std::string& sourceCode, const std::string& filePath, const std::vector<std::string>& compileArgs);

private:
    std::string outputPath;
    std::string languageStandard;
    std::vector<std::unique_ptr<TransformationCommand>> commands;
    bool verbose;
    bool dryRun;

    /**
     * @brief Load the transformation commands
     */
    void loadCommands();
};

class ASTProcessorAction : public clang::ASTFrontendAction
{
public:
    /**
     * @brief Constructor for ASTProcessorAction
     * @param outputPath -> The path to the output directory
     * @param commands -> The transformation commands to apply
     */
    ASTProcessorAction(std::string  outputPath, std::vector<std::unique_ptr<TransformationCommand>>& commands);

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
            clang::CompilerInstance &compiler,
            llvm::StringRef inFile) override;

private:
    std::string outputPath;
    std::vector<std::unique_ptr<TransformationCommand>>& commands;
};

#endif // ASTPROCESSOR_H
