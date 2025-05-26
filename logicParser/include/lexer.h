#ifndef LEXER_H_
#define LEXER_H_

#include <string>
#include <vector>
#include <memory>
#include <unistd.h>

#include "buffer.h"

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum class lexerError {
    NO_ERRORS           = 0,
    CONTEXT_BAD_POINTER = 1,
    ALLOCATION_ERROR    = 2,
    READ_CONTENT_ERROR  = 3,
    CONTENT_BAD_POINTER = 4,
    TOKENS_BAD_POINTER  = 5,
    WORD_TOKENIZE_ERROR = 6,
    INDEX_BAD_POINTER   = 7,
    BAD_WORD_POINTER    = 8,
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum class sourceType {
    PATH,
    FILE_DESCRIPTOR
};

union Source {
    char *path;
    int fileDescriptor = STDIN_FILENO;
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

enum class lexemeType {
    UNDEFINED,
    VARIABLE_LEXEME,
    OPERATOR_LEXEME,
    OPEN_BRACKET_LEXEME,
    CLOSE_BRACKET_LEXEME,
};

struct Lexeme {
    lexemeType   type       = lexemeType::UNDEFINED;
    std::string *word       = {};
    size_t       tokenIndex = {};
};

struct lexerContext {
    Buffer<char>          *content           = {};
    sourceType             contentSourceType = sourceType::PATH;
    Source                 contentSource     = {};
    std::vector<Lexeme *> *tokens            = {};
};

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

lexerError initializeLexerContext(lexerContext *context, sourceType contentSourceType, Source contentSource);
lexerError destroyLexerContext   (lexerContext *context);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

lexerError tokenizeContent       (lexerContext *context);
lexerError tokenizeWord          (lexerContext *context, size_t *index);
lexerError findOperator          (const std::string *word);

// -------------------------------------------------------------------------------------------------------------------------------------------------- //

#endif // LEXER_H_