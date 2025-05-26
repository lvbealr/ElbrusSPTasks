#include <iostream>

#include "binaryTreeDef.h"
#include "binaryTree.h"
#include "parser.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

static node<logicNode> *getPrimaryExpression(logicContext *context, size_t priority);
static node<logicNode> *getUnaryExpression  (logicContext *context, size_t priority);
static node<logicNode> *getBinaryExpression (logicContext *context, size_t priority);

typedef node<logicNode> *(* getterFunc)(logicContext *context, size_t priority);

static getterFunc   nextFunction[] = {getPrimaryExpression, getUnaryExpression, getBinaryExpression};
static const size_t MAX_PRIORITY   = sizeof(nextFunction) / sizeof(nextFunction[0]) - 1;

static Operator    getOperator        (const std::string& op);
static size_t      getOperatorPriority(const std::string& op);
static parserError freeLogicNode      (node<logicNode> *node);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

parserError initializeLogicContext(logicContext *context, std::vector<Lexeme *> *tokens) {
    customWarning(context, parserError::CONTEXT_BAD_POINTER);
    customWarning(tokens,  parserError::TOKENS_BAD_POINTER);

    context->tokens = tokens;

    context->tree   = new binaryTree<logicNode>();
    customWarning(context->tree, parserError::ALLOCATION_ERROR);

    context->currentToken = 0;

    context->variables = new std::map<char, bool>;
    customWarning(context->variables, parserError::ALLOCATION_ERROR);

    return parserError::NO_ERRORS;
}

parserError destroyLogicContext(logicContext *context) {
    customWarning(context,          parserError::CONTEXT_BAD_POINTER);
    customWarning(context->tree,    parserError::TREE_BAD_POINTER);
    customWarning(context->tokens,  parserError::TOKENS_BAD_POINTER);

    treeDestruct(context->tree);
    delete context->tree;

    context->currentToken = 0;

    delete context->variables;

    return parserError::NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

node<logicNode> *getExpression(logicContext *context) {
    customWarning(context,                      nullptr);
    customWarning(context->tokens,              nullptr);
    customWarning(context->tree,                nullptr);
    customWarning(context->tokens->size() != 0, nullptr);
    
    return nextFunction[MAX_PRIORITY](context, 0);
}

static node<logicNode> *getBinaryExpression(logicContext *context, size_t priority) {
    customWarning(context,                      nullptr);
    customWarning(context->tokens,              nullptr);
    customWarning(context->tree,                nullptr);
    customWarning(context->tokens->size() != 0, nullptr);

    node<logicNode> *leftNode = getUnaryExpression(context, priority);
    customWarning(leftNode, nullptr);

    while (context->currentToken < context->tokens->size()) {
        Lexeme *lexeme = (*context->tokens)[context->currentToken];
        customWarning(lexeme, nullptr);

        if (lexeme->type != lexemeType::OPERATOR_LEXEME || *lexeme->word == "NOT") {
            break;
        }

        size_t opPriority = getOperatorPriority(*lexeme->word);
        if (opPriority <= priority) {
            break;
        }

        context->currentToken++;

        if (context->currentToken >= context->tokens->size()) {
            freeLogicNode(leftNode);
            return nullptr;
        }

        node<logicNode> *rightNode = getBinaryExpression(context, opPriority);

        if (!rightNode) {
            freeLogicNode(leftNode);
            customWarning(rightNode, nullptr);
        }

        node<logicNode> *result = new node<logicNode>;

        if (!result) {
            freeLogicNode(leftNode);
            freeLogicNode(rightNode);

            customWarning(result, nullptr);
        }

        result->data = {
            .type = nodeType::OPERATOR, 
            .data = {.operatorType = getOperator(*lexeme->word)}
        };

        result->left  = leftNode;
        result->right = rightNode;

        leftNode = result;
    }

    return leftNode;
}

static node<logicNode> *getUnaryExpression(logicContext *context, size_t priority) {
    customWarning(context,                      nullptr);
    customWarning(context->tokens,              nullptr);
    customWarning(context->tree,                nullptr);
    customWarning(context->tokens->size() != 0, nullptr);

    Lexeme *lexeme = (*context->tokens)[context->currentToken];
    customWarning(lexeme, nullptr);

    if (lexeme->type == lexemeType::OPERATOR_LEXEME && *lexeme->word == "NOT") {
        size_t opPriority = getOperatorPriority("NOT");

        if (opPriority < priority) {
            return nullptr;
        }

        context->currentToken++;

        node<logicNode> *operand = getUnaryExpression(context, opPriority);
        customWarning(operand, nullptr);

        node<logicNode> *result = new node<logicNode>;
        customWarning(result, nullptr);

        result->data  = {
            .type = nodeType::OPERATOR, 
            .data = {.operatorType = Operator::NOT}
        };

        result->left  = operand;
        result->right = nullptr;

        return result;
    }

    return getPrimaryExpression(context, priority);
}

static node<logicNode> *getPrimaryExpression(logicContext *context, size_t priority) {
    customWarning(context,                      nullptr);
    customWarning(context->tokens,              nullptr);
    customWarning(context->tree,                nullptr);
    customWarning(context->tokens->size() != 0, nullptr);

    Lexeme *lexeme = (*context->tokens)[context->currentToken];
    customWarning(lexeme, nullptr);

    if (lexeme->type == lexemeType::VARIABLE_LEXEME) {
        node<logicNode> *result = new node<logicNode>;
        customWarning(result, nullptr);

        result->data = {
            .type = nodeType::VARIABLE,
            .data = {.variable = (*lexeme->word)[0]}
        };

        result->left  = nullptr;
        result->right = nullptr;

        context->currentToken++;

        context->variables->insert({(*lexeme->word)[0], false});

        return result;
    } else if (lexeme->type == lexemeType::OPEN_BRACKET_LEXEME) {
        context->currentToken++;

        node<logicNode> *result = getBinaryExpression(context, 0);

        if (!result || context->currentToken >= context->tokens->size() || 
            (*context->tokens)[context->currentToken]->type != lexemeType::CLOSE_BRACKET_LEXEME) {
            freeLogicNode(result);
            return nullptr;
        }

        context->currentToken++;
        return result;
    }

    return nullptr;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

parserError setVariablesValue(logicContext *context) {
    customWarning(context,            parserError::CONTEXT_BAD_POINTER);
    customWarning(context->variables, parserError::TOKENS_BAD_POINTER);

    size_t variablesCount = context->variables->size();

    if (variablesCount == 0) {
        return parserError::NO_ERRORS;
    }

    for (size_t i = 0; i < variablesCount; i++) {
        auto it = context->variables->begin();
        std::advance(it, i);

        char variable = it->first;
        bool value    = it->second;

        std::cout << "Enter value for variable '" << variable << "' (0 or 1): ";
        int inputValue = {};
        std::cin >> inputValue;

        if (inputValue == 0) {
            value = false;
        } else if (inputValue == 1) {
            value = true;
        } else {
            std::cerr << "Invalid input. Please enter 0 or 1." << std::endl;
            i--;
            continue;
        }

        it->second = value;
    }

    return parserError::NO_ERRORS;
}

parserError evaluateExpression(logicContext *context, bool *result) {
    customWarning(context,              parserError::CONTEXT_BAD_POINTER);
    customWarning(context->tree,        parserError::TREE_BAD_POINTER);
    customWarning(result,               parserError::NODE_BAD_POINTER);
    customWarning(context->tree->root,  parserError::NODE_BAD_POINTER);
    customWarning(context->variables,   parserError::TOKENS_BAD_POINTER);

    return evaluateSubexpression(context, context->tree->root, result);
}

parserError evaluateSubexpression(logicContext *context, node<logicNode> *currentRoot, bool *result) {
    customWarning(context,              parserError::CONTEXT_BAD_POINTER);
    customWarning(currentRoot,          parserError::NODE_BAD_POINTER);
    customWarning(result,               parserError::NODE_BAD_POINTER);
    customWarning(context->variables,   parserError::TOKENS_BAD_POINTER);

    if (currentRoot->data.type == nodeType::VARIABLE) {
        auto it = context->variables->find(currentRoot->data.data.variable);
        if (it != context->variables->end()) {
            *result = it->second;
            return parserError::NO_ERRORS;
        } else {
            return parserError::NODE_BAD_POINTER;
        }
    }

    if (currentRoot->data.type == nodeType::OPERATOR) {
        bool leftResult  = false;
        bool rightResult = false;

        if (currentRoot->left) {
            evaluateSubexpression(context, currentRoot->left, &leftResult);
        }

        if (currentRoot->right) {
            evaluateSubexpression(context, currentRoot->right, &rightResult);
        }

        switch (currentRoot->data.data.operatorType) {
            case Operator::NOT:
                *result = !leftResult;
                break;
            case Operator::AND:
                *result = leftResult && rightResult;
                break;
            case Operator::XOR:
                *result = leftResult ^ rightResult;
                break;
            case Operator::OR:
                *result = leftResult || rightResult;
                break;
            default:
                return parserError::NODE_BAD_POINTER;
        }

        return parserError::NO_ERRORS;
    }

    return parserError::NODE_BAD_POINTER;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#define GET_OPERATOR(NAME, OPERATOR) do {   \
    if (op == NAME) {                       \
        return OPERATOR;                    \
    }                                       \
} while (0)

static Operator getOperator(const std::string& op) {
    GET_OPERATOR("NOT", Operator::NOT);
    GET_OPERATOR("AND", Operator::AND);
    GET_OPERATOR("XOR", Operator::XOR);
    GET_OPERATOR("OR",  Operator::OR);

    return Operator::UNDEFINED;
}

#undef GET_OPERATOR

#define GET_PRIORITY(OPERATOR, PRIORITY) do {   \
    if (op == OPERATOR) {                       \
        return PRIORITY;                        \
    }                                           \
} while (0)

static size_t getOperatorPriority(const std::string& op) {
    GET_PRIORITY("NOT", 4);
    GET_PRIORITY("AND", 3);
    GET_PRIORITY("XOR", 2);
    GET_PRIORITY("OR",  1);

    return 0;
}

#undef GET_PRIORITY

static parserError freeLogicNode(node<logicNode> *node) {
    customWarning(node, parserError::NODE_BAD_POINTER);

    freeLogicNode(node->left);
    freeLogicNode(node->right);

    delete node;

    return parserError::NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //