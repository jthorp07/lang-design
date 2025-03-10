/**
 * @file tokenizer.hpp
 * 
 * @brief Include file for tokenizer class and related types
 */

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <array>
#include <string>
#include <string_view>
#include <vector>

namespace imperium_lang {

    constexpr int BLOCK_SIZE = 4096;
    constexpr int BUFFER_SIZE = BLOCK_SIZE * 16 * 16;

    enum TokenType {
        Keyword,
        Identifier,
        Operator,
        Literal,
        Comment,
        Whitespace,
        EndOfFile,
        Delimiter,
        Invalid,
    };

    /**
     * @brief Provides the string name of a `TokenType`
     * 
     * @param[in] type The `TokenType` to get the name of
     * @return The string name of the `TokenType`
     * @retval "unknown" The token is not a known TokenType
     */
    constexpr std::string tokenTypeToString(TokenType type) {
        switch (type) {
            case Keyword: return "keyword";
            case Identifier: return "identifier";
            case Operator: return "operator";
            case Literal: return "literal";
            case Comment: return "comment";
            case Whitespace: return "whitespace";
            case EndOfFile: return "end-of-file";
            case Delimiter: return "delimiter";
            case Invalid: return "invalid";
            default: return "invalid";
        }
    }

    struct Token {
        TokenType type;
        std::string value;
    };

    /**
     * @brief Tokenizer class
     * 
     * This class is responsible for tokenizing the content of a source file.
     */
    class Tokenizer {
    private:
        const std::string sourceFile;
        std::array<char, BUFFER_SIZE> buffer{};
    public:
        /**
         * @brief Constructor
         * 
         * @param[in] sourceFile The source file to tokenize
         */
        Tokenizer(const std::string& sourceFile);

        /**
         * @brief Tokenize the source file
         * 
         * @param[out] tokens The tokens extracted from the source file
         * @return Status code
         * @retval 0 Success
         * @retval -1 Parse Error
         * @retval -2 Read Error
         */
        int tokenize(std::vector<Token>& tokens);
    };

}

#endif
