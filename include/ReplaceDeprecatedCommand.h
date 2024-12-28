#ifndef REPLACE_DEPRECATED_COMMAND_H
#define REPLACE_DEPRECATED_COMMAND_H

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <system_error>
#include "TransformationCommand.h"

/// @brief Command to replace deprecated function calls
class ReplaceDeprecatedCommand : public TransformationCommand {
public:
    /**
     * @brief Construct a new Replace Deprecated Command object
     * @param verbose -> Whether to log replacements
     * @param dryRun -> If true, do not write changes to files
     */
    ReplaceDeprecatedCommand(bool verbose, bool dryRun);

    /**
     * @brief Execute the replacement on the provided AST context
     * @param context -> The Clang AST context
     */
    void execute(clang::ASTContext& context) override; // Implement the execute method from TransformationCommand

    virtual ~ReplaceDeprecatedCommand();

private:
    bool verbose; ///< Whether to log replacement operations
    bool dryRun;  ///< If true, do not modify files

    /// Visitor to detect and replace deprecated function calls
    class DeprecatedFunctionVisitor : public clang::RecursiveASTVisitor<DeprecatedFunctionVisitor> {
    public:
        explicit DeprecatedFunctionVisitor(clang::Rewriter& rewriter, const clang::LangOptions& langOpts, bool verbose);

        /**
         * Visit function declarations and replace deprecated functions based on the language standard.
         */
        bool VisitDeclRefExpr(clang::DeclRefExpr* expr);

    private:
        clang::Rewriter& rewriter;
        const clang::LangOptions& langOpts;
        bool verbose;

        /**
         * Handle the replacement of a deprecated function.
         */
        void handleReplacement(const clang::FunctionDecl* funcDecl, clang::DeclRefExpr* expr);

        /**
         * Get a replacement for a deprecated function based on the C++ standard.
         */
        std::string getReplacement(const std::string& functionName) const;

        /**
         * Log the replacement details.
         */
        void logReplacement(const std::string& deprecatedFunction, const std::string& replacement, clang::SourceLocation location);
    };
};

#endif // REPLACE_DEPRECATED_COMMAND_H
