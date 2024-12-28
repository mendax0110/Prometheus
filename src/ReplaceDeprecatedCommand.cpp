#include "ReplaceDeprecatedCommand.h"

ReplaceDeprecatedCommand::ReplaceDeprecatedCommand(bool verbose, bool dryRun)
        : verbose(verbose), dryRun(dryRun) {}

ReplaceDeprecatedCommand::~ReplaceDeprecatedCommand() = default;

void ReplaceDeprecatedCommand::execute(clang::ASTContext& context)
{
    clang::Rewriter rewriter(context.getSourceManager(), context.getLangOpts());
    const auto& langOpts = context.getLangOpts();
    DeprecatedFunctionVisitor visitor(rewriter, langOpts, verbose);

    visitor.TraverseDecl(context.getTranslationUnitDecl());

    if (!dryRun)
    {
        const auto& sm = context.getSourceManager();
        for (auto iter = rewriter.buffer_begin(); iter != rewriter.buffer_end(); ++iter)
        {
            const auto* fileEntry = sm.getFileEntryForID(iter->first);
            if (fileEntry)
            {
                llvm::errs() << "Writing changes to file: " << fileEntry->tryGetRealPathName() << "\n";
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
}


ReplaceDeprecatedCommand::DeprecatedFunctionVisitor::DeprecatedFunctionVisitor(clang::Rewriter& rewriter, const clang::LangOptions& langOpts, bool verbose)
        : rewriter(rewriter), langOpts(langOpts), verbose(verbose) {}

bool ReplaceDeprecatedCommand::DeprecatedFunctionVisitor::VisitDeclRefExpr(clang::DeclRefExpr* expr)
{
    if (auto* funcDecl = llvm::dyn_cast<clang::FunctionDecl>(expr->getDecl()))
    {
        // Check if the function is deprecated
        if (funcDecl->hasAttr<clang::DeprecatedAttr>())
        {
            handleReplacement(funcDecl, expr);
        }
    }
    return true;
}

void ReplaceDeprecatedCommand::DeprecatedFunctionVisitor::handleReplacement(const clang::FunctionDecl* funcDecl, clang::DeclRefExpr* expr)
{
    std::string deprecatedName = funcDecl->getNameAsString();
    std::string replacement = getReplacement(deprecatedName);

    if (!replacement.empty())
    {
        // Replace the text in the source
        rewriter.ReplaceText(expr->getSourceRange(), replacement);
        if (verbose)
        {
            logReplacement(deprecatedName, replacement, expr->getBeginLoc());
        }
    }
}

std::string ReplaceDeprecatedCommand::DeprecatedFunctionVisitor::getReplacement(const std::string& functionName) const
{
    if (langOpts.CPlusPlus17)
    {
        if (functionName == "std::auto_ptr")
        {
            return "std::unique_ptr";
        }
    }
    if (langOpts.CPlusPlus20)
    {
        if (functionName == "std::random_shuffle")
        {
            return "std::shuffle";
        }
    }

    // Add additional rules as needed
    return "";
}

void ReplaceDeprecatedCommand::DeprecatedFunctionVisitor::logReplacement(const std::string& deprecatedFunction, const std::string& replacement, clang::SourceLocation location)
{
    llvm::errs() << "Replaced deprecated function: " << deprecatedFunction
                 << " with: " << replacement
                 << " at: " << location.printToString(rewriter.getSourceMgr()) << "\n";
}
