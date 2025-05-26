#include <iostream>

#include "lexer.h"
#include "parser.h"

int main() {
    lexerContext context;
    Source source;
    source.path = "input.txt";

    initializeLexerContext(&context, sourceType::PATH, source);
    tokenizeContent(&context);

    logicContext lContext;
    initializeLogicContext(&lContext, context.tokens);
    lContext.tree->root = getExpression(&lContext);

    std::cout << "Expression: " << context.content->data << std::endl;

    setVariablesValue(&lContext);

    bool result = false;
    parserError evalStatus = evaluateExpression(&lContext, &result);

    std::cout << "Evaluation result: " << (evalStatus == parserError::NO_ERRORS ? (result ? "true" : "false") : "error") << std::endl;

    destroyLexerContext(&context);
    destroyLogicContext(&lContext);

    return EXIT_SUCCESS;
}