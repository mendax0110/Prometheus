#ifndef TRANSFORMATIONCOMMAND_H
#define TRANSFORMATIONCOMMAND_H

#include <clang/AST/ASTContext.h>

/// @brief Base class for transformation commands
class TransformationCommand
{
public:
    /**
     * @brief Destroy the TransformationCommand object
     */
    virtual ~TransformationCommand() = default;

    /**
     * @brief Execute the transformation command
     * @param context -> The AST context
     */
    virtual void execute(clang::ASTContext& context) = 0;
};

#endif // TRANSFORMATIONCOMMAND_H
