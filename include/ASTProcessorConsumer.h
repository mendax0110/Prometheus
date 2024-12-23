#ifndef ASTPROCESSORCONSUMER_H
#define ASTPROCESSORCONSUMER_H

#include "clang/AST/ASTConsumer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "TransformationCommand.h"

/// @brief Consumer class that applies transformation commands to the AST
class ASTProcessorConsumer : public clang::ASTConsumer
{
public:
    /**
     * @brief Constructor for ASTProcessorConsumer
     * @param rewriter -> The rewriter to modify source code
     * @param commands -> The transformation commands to apply
     */
    ASTProcessorConsumer(clang::Rewriter& rewriter,
                         std::vector<std::unique_ptr<TransformationCommand>>& commands);

    /**
     * @brief Handle the entire translation unit (AST root)
     * @param context -> The ASTContext containing the root of the AST
     */
    void HandleTranslationUnit(clang::ASTContext& context) override;

private:
    clang::Rewriter& rewriter;  ///< Rewriter to apply changes to the source code
    std::vector<std::unique_ptr<TransformationCommand>>& commands;  ///< List of commands to apply
};

#endif // ASTPROCESSORCONSUMER_H
