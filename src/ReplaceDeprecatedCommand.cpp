#include "ReplaceDeprecatedCommand.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/Decl.h>
#include <clang/AST/Expr.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Rewrite/Core/Rewriter.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <system_error>

/// @brief Visitor to find and replace deprecated function calls
class DeprecatedFunctionVisitor : public clang::RecursiveASTVisitor<DeprecatedFunctionVisitor>
{
public:
    explicit DeprecatedFunctionVisitor(clang::Rewriter& rewriter) : rewriter(rewriter)
    {

    }

    /**
     * @brief Visit a DeclRefExpr node
     * @param expr -> The DeclRefExpr node to visit
     * @return True to continue visiting the AST, false to stop
     */
    bool VisitDeclRefExpr(clang::DeclRefExpr* expr)
    {
        // Check if the function is deprecated
        if (auto* funcDecl = llvm::dyn_cast<clang::FunctionDecl>(expr->getDecl()))
        {
            if (isDeprecated(funcDecl))
            {
                // Replace deprecated function usage with a modern equivalent
                rewriter.ReplaceText(expr->getSourceRange(), getReplacement(funcDecl));
            }
        }
        return true;
    }

private:
    clang::Rewriter& rewriter;

    /**
     * @brief Check if the given function declaration is deprecated
     * @param funcDecl -> The function declaration to check
     * @return True if the function is deprecated, false otherwise
     */
    bool isDeprecated(const clang::FunctionDecl* funcDecl) const
    {
        // Check for attribute "deprecated"
        if (funcDecl->hasAttr<clang::DeprecatedAttr>())
        {
            return true;
        }

        // Add additional custom checks here if needed
        return false;
    }

    /**
     * @brief Get the replacement text for a deprecated function
     * @param funcDecl -> The deprecated function declaration
     * @return Replacement string for the function
     */
    std::string getReplacement(const clang::FunctionDecl* funcDecl) const
    {
        // Replace known deprecated functions with their modern counterparts
        std::string functionName = funcDecl->getNameAsString();
        if (functionName == "auto_ptr")
        {
            return "std::unique_ptr";
        }

        // Return the original name if no replacement is found (fallback)
        return functionName;
    }
};

void ReplaceDeprecatedCommand::execute(clang::ASTContext& context)
{
    clang::Rewriter rewriter(context.getSourceManager(), context.getLangOpts());
    DeprecatedFunctionVisitor visitor(rewriter);

    // Traverse the AST
    visitor.TraverseDecl(context.getTranslationUnitDecl());

    // Write the changes back to the files
    const auto& sm = context.getSourceManager();
    for (auto iter = rewriter.buffer_begin(); iter != rewriter.buffer_end(); ++iter)
    {
        const auto* fileEntry = sm.getFileEntryForID(iter->first);
        if (fileEntry)
        {
            llvm::errs() << "Rewriting file: " << fileEntry->tryGetRealPathName() << "\n";
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
