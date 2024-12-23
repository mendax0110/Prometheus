#include "ASTProcessor.h"

#include <utility>
#include "clang/Tooling/Tooling.h"
#include "ReplaceDeprecatedCommand.h"
#include "ASTProcessorConsumer.h"
#include "clang/Frontend/CompilerInstance.h"

ASTProcessor::ASTProcessor(std::string outputPath, std::string languageStandard)
        : outputPath(std::move(outputPath)), languageStandard(std::move(languageStandard))
{
    loadCommands();
}

void ASTProcessor::process(const std::string& sourceCode)
{
    clang::tooling::runToolOnCodeWithArgs(
            std::make_unique<ASTProcessorAction>(outputPath, commands),
            sourceCode,
            {"-std=" + languageStandard} // Dynamically use the language standard
    );
}

void ASTProcessor::loadCommands()
{
    commands.push_back(std::make_unique<ReplaceDeprecatedCommand>());
}

ASTProcessorAction::ASTProcessorAction(std::string  outputPath, std::vector<std::unique_ptr<TransformationCommand>>& commands)
        : outputPath(std::move(outputPath)), commands(commands)
{

}

std::unique_ptr<clang::ASTConsumer>
ASTProcessorAction::CreateASTConsumer(clang::CompilerInstance &compiler, llvm::StringRef inFile)
{
    clang::Rewriter rewriter(compiler.getSourceManager(), compiler.getLangOpts());

    return std::make_unique<ASTProcessorConsumer>(rewriter, commands);
}
