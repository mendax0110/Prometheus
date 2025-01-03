#include "DeprecatedFunctionVisitor.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/AST/Attr.h>
#include <clang/Basic/SourceManager.h>

using namespace clang;

DeprecatedFunctionVisitor::DeprecatedFunctionVisitor(
        Rewriter& rewriter,
        const LangOptions& langOpts,
        bool verbose,
        const std::unordered_map<std::string, std::string>& replacementMap)
        : rewriter(rewriter), langOpts(langOpts), verbose(verbose), replacementMap(replacementMap)
{

}

bool DeprecatedFunctionVisitor::VisitDeclRefExpr(DeclRefExpr* expr)
{
    if (auto* funcDecl = dyn_cast<FunctionDecl>(expr->getDecl()))
    {
        if (funcDecl->hasAttr<DeprecatedAttr>())
        {
            replaceFunction(funcDecl, expr);
        }
    }
    return true;
}

void DeprecatedFunctionVisitor::replaceFunction(FunctionDecl* funcDecl, DeclRefExpr* expr)
{
    std::string deprecatedName = funcDecl->getQualifiedNameAsString();

    auto it = replacementMap.find(deprecatedName);
    if (it != replacementMap.end())
    {
        std::string replacement = it->second;
        SourceRange range = expr->getSourceRange();

        rewriter.ReplaceText(range, replacement);

        if (verbose)
        {
            SourceManager& sm = rewriter.getSourceMgr();
            SourceLocation loc = expr->getBeginLoc();
            llvm::errs() << "Replaced deprecated function: " << deprecatedName
                         << " with: " << replacement
                         << " at: " << sm.getFilename(loc) << ":"
                         << sm.getSpellingLineNumber(loc) << "\n";
        }
    }
    else if (verbose)
    {
        llvm::errs() << "No replacement found for deprecated function: " << deprecatedName << "\n";
    }
}
