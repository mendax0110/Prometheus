#include "ASTProcessorConsumer.h"
#include "ReplaceDeprecatedCommand.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/SourceManager.h"

ASTProcessorConsumer::ASTProcessorConsumer(clang::Rewriter& rewriter,
                                           std::vector<std::unique_ptr<TransformationCommand>>& commands)
        : rewriter(rewriter), commands(commands)
{

}

void ASTProcessorConsumer::HandleTranslationUnit(clang::ASTContext& context)
{
    // Execute each transformation command on the AST
    for (auto& command : commands)
    {
        command->execute(context);
    }

    // After transforming the AST, apply changes to the source code
    const clang::SourceManager& sourceManager = context.getSourceManager();
    for (auto iter = rewriter.buffer_begin(); iter != rewriter.buffer_end(); ++iter)
    {
        const clang::FileEntry* fileEntry = sourceManager.getFileEntryForID(iter->first);
        if (fileEntry)
        {
            std::error_code ec;
            llvm::raw_fd_ostream outFile(fileEntry->tryGetRealPathName(), ec, llvm::sys::fs::OpenFlags::OF_None);
            if (!ec)
            {
                outFile << std::string(iter->second.begin(), iter->second.end());
            }
            else
            {
                llvm::errs() << "Error writing to file: " << ec.message() << "\n";
            }
        }
    }
}
