#ifndef DEPRECATED_FUNCTION_VISITOR_H
#define DEPRECATED_FUNCTION_VISITOR_H

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <unordered_map>

namespace clang
{
    class DeclRefExpr;
    class FunctionDecl;
    class LangOptions;
    class SourceManager;
}

/**
 * @brief AST visitor for detecting deprecated function calls and replacing them.
 */
class DeprecatedFunctionVisitor : public clang::RecursiveASTVisitor<DeprecatedFunctionVisitor>
{
public:
    /**
     * @brief Constructs the visitor for deprecated function replacements.
     * @param rewriter The Clang Rewriter instance.
     * @param langOpts Language options.
     * @param verbose If true, verbose output is enabled.
     * @param replacementMap A map of deprecated functions to their replacements.
     */
    DeprecatedFunctionVisitor(clang::Rewriter& rewriter,
                              const clang::LangOptions& langOpts,
                              bool verbose,
                              const std::unordered_map<std::string, std::string>& replacementMap);

    /**
     * @brief Visits DeclRefExpr nodes in the AST.
     * @param expr The DeclRefExpr node.
     * @return True to continue traversal.
     */
    bool VisitDeclRefExpr(clang::DeclRefExpr* expr);

private:
    clang::Rewriter& rewriter; ///< Clang Rewriter for source code modification.
    const clang::LangOptions& langOpts; ///< Language options.
    bool verbose; ///< Whether to log replacements.
    const std::unordered_map<std::string, std::string>& replacementMap; ///< Map of deprecated functions to replacements.

    /**
     * @brief Replaces deprecated function calls.
     * @param funcDecl The function declaration.
     * @param expr The expression containing the function reference.
     */
    void replaceFunction(clang::FunctionDecl* funcDecl, clang::DeclRefExpr* expr);
};

#endif // DEPRECATED_FUNCTION_VISITOR_H
