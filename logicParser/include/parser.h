#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <map>

#include "binaryTreeDef.h"
#include "lexer.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum class parserError {
    NO_ERRORS           = 0,
    TREE_BAD_POINTER    = 1,
    TOKENS_BAD_POINTER  = 2,
    NODE_BAD_POINTER    = 3,
    CONTEXT_BAD_POINTER = 4,
    ALLOCATION_ERROR    = 5,
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum class Operator {
    UNDEFINED,
    NOT,
    AND,
    XOR,
    OR
};

union logicNodeData {
    char variable;
    Operator operatorType = Operator::UNDEFINED;
};

enum class nodeType {
    UNDEFINED,
    VARIABLE,
    OPERATOR
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

struct logicNode {
    nodeType      type = nodeType::UNDEFINED;
    logicNodeData data = {};
};

struct logicContext {
    binaryTree<logicNode> *tree   = nullptr;

    std::vector<Lexeme *> *tokens = nullptr;
    size_t currentToken = 0; 

    std::map<char, bool> *variables = nullptr;
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

parserError initializeLogicContext(logicContext *context, std::vector<Lexeme *> *tokens);
parserError destroyLogicContext   (logicContext *context);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

node<logicNode> *getExpression    (logicContext *context);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

parserError setVariablesValue     (logicContext *context);
parserError evaluateExpression    (logicContext *context, bool *result);
parserError evaluateSubexpression (logicContext *context, node<logicNode> *currentRoot, bool *result);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // PARSER_H_