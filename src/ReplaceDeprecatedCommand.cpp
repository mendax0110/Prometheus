#include "ReplaceDeprecatedCommand.h"
#include "DeprecatedFunctionVisitor.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/AST/Attr.h>
#include <clang/ASTMatchers/ASTMatchers.h>

using namespace clang;
using namespace clang::ast_matchers;

ReplaceDeprecatedCommand::ReplaceDeprecatedCommand(bool verbose, bool dryRun)
        : verbose(verbose), dryRun(dryRun)
{

}

ReplaceDeprecatedCommand::~ReplaceDeprecatedCommand() = default;

void ReplaceDeprecatedCommand::execute(ASTContext& context)
{
    Rewriter rewriter(context.getSourceManager(), context.getLangOpts());

    DeprecatedFunctionVisitor visitor(rewriter, context.getLangOpts(), verbose, replacementMap);
    visitor.TraverseDecl(context.getTranslationUnitDecl());

    if (!dryRun)
    {
        for (auto it = rewriter.buffer_begin(); it != rewriter.buffer_end(); ++it)
        {
            const FileID fileID = it->first;
            const FileEntry* fileEntry = context.getSourceManager().getFileEntryForID(fileID);
            if (fileEntry)
            {
                std::error_code ec;
                llvm::raw_fd_ostream outFile(fileEntry->tryGetRealPathName(), ec, llvm::sys::fs::OF_None);
                if (!ec)
                {
                    const RewriteBuffer& rewriteBuf = it->second;
                    outFile << std::string(rewriteBuf.begin(), rewriteBuf.end());
                }
                else
                {
                    llvm::errs() << "Error writing to file: " << ec.message() << "\n";
                }
            }
        }
    }
}
