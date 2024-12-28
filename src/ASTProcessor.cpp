#include "ASTProcessor.h"

#include <utility>
#include "clang/Tooling/Tooling.h"
#include "ReplaceDeprecatedCommand.h"
#include "ASTProcessorConsumer.h"
#include "clang/Frontend/CompilerInstance.h"

ASTProcessor::ASTProcessor(std::string outputPath, std::string languageStandard, bool verbose, bool dryRun)
        : outputPath(std::move(outputPath)), languageStandard(std::move(languageStandard)),
          verbose(verbose), dryRun(dryRun)
{
    loadCommands();
}

void ASTProcessor::process(const std::string& sourceCode, const std::string& filePath, const std::vector<std::string>& compileArgs)
{
    /*std::vector<std::string> compileArgs = {
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
    };*/

    clang::tooling::runToolOnCodeWithArgs(
            std::make_unique<ASTProcessorAction>(outputPath, commands),
            sourceCode,
            compileArgs,
            filePath);
}

void ASTProcessor::loadCommands()
{
    commands.push_back(std::make_unique<ReplaceDeprecatedCommand>(verbose, dryRun));
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
