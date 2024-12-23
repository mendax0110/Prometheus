#ifndef REPLACEDEPRECATEDCOMMAND_H
#define REPLACEDEPRECATEDCOMMAND_H

#include "TransformationCommand.h"
#include <clang/Rewrite/Core/Rewriter.h>

/// @brief Command to replace deprecated functions
class ReplaceDeprecatedCommand : public TransformationCommand
{
public:
    /**
     * @brief Construct a new ReplaceDeprecatedCommand object
     */
    void execute(clang::ASTContext& context) override;
};

#endif // REPLACEDEPRECATEDCOMMAND_H
