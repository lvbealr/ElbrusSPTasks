#include <iostream>
#include <map>

#include "lexer.h"
#include "customWarning.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

static std::string  operators[]    = {"NOT", "AND", "OR", "XOR"};
static const size_t operatorsCount = sizeof(operators) / sizeof(operators[0]);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

lexerError initializeLexerContext(lexerContext *context, sourceType contentSourceType, Source contentSource) {
    customWarning(context, lexerError::CONTEXT_BAD_POINTER);

    context->content = new Buffer<char>;
    customWarning(context->content, lexerError::ALLOCATION_ERROR);

    context->contentSourceType = contentSourceType;
    context->contentSource     = contentSource;

    context->tokens = new std::vector<Lexeme *>;
    customWarning(context->tokens, lexerError::ALLOCATION_ERROR);
    context->tokens->reserve(10);

    bufferInitialize(context->content);

    if (context->contentSourceType == sourceType::PATH) {
        bufferError readStatus = writeFileDataToBuffer(context->content, context->contentSource.path);
        customWarning(readStatus == bufferError::NO_BUFFER_ERROR, lexerError::READ_CONTENT_ERROR);
    } else if (context->contentSourceType == sourceType::FILE_DESCRIPTOR) {
        std::string content;
        std::getline(std::cin, content);

        bufferError readStatus = writeDataToBuffer(context->content, content.c_str(), content.size());
        customWarning(readStatus == bufferError::NO_BUFFER_ERROR, lexerError::READ_CONTENT_ERROR);
    }

    return lexerError::NO_ERRORS;
}

lexerError destroyLexerContext(lexerContext *context) {
    customWarning(context,          lexerError::CONTEXT_BAD_POINTER);
    customWarning(context->content, lexerError::CONTENT_BAD_POINTER);
    customWarning(context->tokens,  lexerError::TOKENS_BAD_POINTER);

    bufferDestruct(context->content);
    delete context->content;

    for (Lexeme *lexeme : *context->tokens) {
        customWarning(lexeme, lexerError::BAD_WORD_POINTER);

        delete lexeme->word;
        delete lexeme;
    }

    delete context->tokens;

    return lexerError::NO_ERRORS;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

lexerError tokenizeContent(lexerContext *context) {
    customWarning(context,          lexerError::CONTEXT_BAD_POINTER);
    customWarning(context->content, lexerError::CONTENT_BAD_POINTER);
    customWarning(context->tokens,  lexerError::TOKENS_BAD_POINTER);
    
    size_t currentIndex = 0;

    setlocale(LC_ALL, "en_US.utf8");

    while (currentIndex < context->content->currentIndex) {
        if (std::isspace(context->content->data[currentIndex])) {
            currentIndex++;
            continue;
        }

        lexerError status = tokenizeWord(context, &currentIndex);
        customWarning(status == lexerError::NO_ERRORS, lexerError::WORD_TOKENIZE_ERROR);
    }

    return lexerError::NO_ERRORS;
}

lexerError tokenizeWord(lexerContext *context, size_t *index) {
    customWarning(context,          lexerError::CONTEXT_BAD_POINTER);
    customWarning(context->content, lexerError::CONTENT_BAD_POINTER);
    customWarning(context->tokens,  lexerError::TOKENS_BAD_POINTER);
    customWarning(index,            lexerError::INDEX_BAD_POINTER);

    Lexeme *lexeme = new Lexeme;
    customWarning(lexeme, lexerError::ALLOCATION_ERROR);

    lexeme->tokenIndex = context->tokens->size();

    std::string *word = new std::string();
    customWarning(word, lexerError::ALLOCATION_ERROR);

    while ((*index) < context->content->currentIndex && !std::isspace(context->content->data[(*index)])) {
        if (context->content->data[(*index)] == '(') {
            (*word) += context->content->data[(*index)];
            break;
        } else if (context->content->data[(*index)] == ')') {
            if (!(*word).empty()) {
                (*index)--;
                break;
            }
        }

        (*word) += context->content->data[(*index)];
        (*index)++;
    }

    if ((*word).empty()) {
        return lexerError::WORD_TOKENIZE_ERROR;
    }

    if ((*word)[0] == '(') {
        lexeme->type = lexemeType::OPEN_BRACKET_LEXEME;
        context->tokens->push_back(lexeme);
        (*index)++;

        lexeme->word = word;

        return lexerError::NO_ERRORS;
    } else if ((*word)[0] == ')') {
        lexeme->type = lexemeType::CLOSE_BRACKET_LEXEME;
        context->tokens->push_back(lexeme);
        (*index)++;

        lexeme->word = word;

        return lexerError::NO_ERRORS;
    }

    if ((*word).size() == 1 && std::isalpha((*word)[0])) {
        lexeme->type = lexemeType::VARIABLE_LEXEME;
        context->tokens->push_back(lexeme);
        (*index)++;

        lexeme->word = word;

        return lexerError::NO_ERRORS;
    }

    lexerError status = findOperator(word);
    if (status != lexerError::NO_ERRORS) {
        delete lexeme;
        delete word;

        customWarning(status == lexerError::NO_ERRORS, lexerError::WORD_TOKENIZE_ERROR);
    }

    lexeme->type = lexemeType::OPERATOR_LEXEME;
    context->tokens->push_back(lexeme);
    
    lexeme->word = word;

    (*index)++;

    return lexerError::NO_ERRORS;
}

lexerError findOperator(const std::string *word) {
    customWarning(word, lexerError::BAD_WORD_POINTER);

    for (size_t i = 0; i < operatorsCount; i++) {
        if ((*word) == operators[i]) {
            return lexerError::NO_ERRORS;
        }
    }

    return lexerError::WORD_TOKENIZE_ERROR;
}

// -------------------------------------------------------------------------------------------------------------------------------------------------- //