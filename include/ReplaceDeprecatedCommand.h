#ifndef REPLACE_DEPRECATED_COMMAND_H
#define REPLACE_DEPRECATED_COMMAND_H

#include <clang/Rewrite/Core/Rewriter.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <unordered_map>
#include "TransformationCommand.h"

// Forward declaration of the visitor class
namespace clang
{
    class ASTContext;
}

/**
 * @brief Command to replace deprecated function calls in C++ code.
 */
class ReplaceDeprecatedCommand : public TransformationCommand
{
public:
    /**
     * @brief Constructs a ReplaceDeprecatedCommand.
     * @param verbose If true, verbose logging is enabled.
     * @param dryRun If true, no file modifications are performed.
     */
    ReplaceDeprecatedCommand(bool verbose, bool dryRun);

    /**
     * @brief Executes the replacement process on the AST.
     * @param context The Clang AST context.
     */
    void execute(clang::ASTContext& context) override;

    /**
     * @brief Destructor.
     */
    ~ReplaceDeprecatedCommand() override;

private:
    bool verbose; ///< Whether to log replacement operations.
    bool dryRun;  ///< If true, no file modifications are performed.

    std::unordered_map<std::string, std::string> replacementMap = {
            {"std::auto_ptr", "std::unique_ptr"},
            {"std::random_shuffle", "std::shuffle"}
    };
};

#endif // REPLACE_DEPRECATED_COMMAND_H
