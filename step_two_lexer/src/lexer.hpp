/**
 * @file lexer.hpp
 * 
 * @brief Include file for Lexer class and related types
 */

#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <array>
#include "tokenizer.hpp"

namespace imperium_lang {

    enum LexedTokenType {
        StringLiteral,
        IntegerLiteral,
        FloatLiteral,
        TokenBlock,
        Operator,
    };

    /**
     * @brief Provides the string name of a `LexedTokenType`
     * 
     * @param[in] type The `LexedTokenType` to get the name of
     * @return The string name of the `LexedTokenType`
     * @retval "invalid" The token is not a known `LexedTokenType`
     */
    constexpr std::string lexedTokenTypeName(LexedTokenType type) {
        switch (type) {
            case StringLiteral: return "string-literal";
            case IntegerLiteral: return "integer-literal";
            case FloatLiteral: return "float-literal";
            case TokenBlock: return "token-block";
            case Operator: return "operator";
            default: return "invalid";
        }
    }

    struct LexedToken {
        LexedTokenType type;
        std::vector<Token> children;
    };

    /**
     * @brief Lexer class. Accepts primitive tokens and assembles them into more context-sensitive token constructs
     */
    class Lexer {
    private:
        bool inString = false;
        bool inIgnored = false;
    public:
        /**
         * @brief Default constructor
         */
        Lexer() = default;

        /**
         * @brief Destructor
         */
        ~Lexer() = default;
        /**
         * 
         */
        int lexTokens(std::vector<LexedToken> &lexedTokens, const auto& tokens);
    };
}

#endif
