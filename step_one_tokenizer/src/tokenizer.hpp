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
        KEYWORD,
        IDENTIFIER,
        OPERATOR,
        LITERAL,
        COMMENT,
        WHITESPACE,
        NEWLINE,
        END_OF_FILE,
        DELIMITER,
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
            case KEYWORD: return "keyword";
            case IDENTIFIER: return "identifier";
            case OPERATOR: return "operator";
            case LITERAL: return "literal";
            case COMMENT: return "comment";
            case WHITESPACE: return "whitespace";
            case NEWLINE: return "newline";
            case END_OF_FILE: return "end-of-file";
            case DELIMITER: return "delimiter";
            default: return "unknown";
        }
    }

    struct Token {
        const TokenType type;
        const std::string value;
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
    private:
        /**
         * @brief Shifts unprocessed data to the start of the buffer, then refills
         *        the buffer with the next data from the source file.
         * 
         * @param[in, out] unprocessed View of unprocessed data
         * @param[out] bytesRead Number of bytes read
         * @return Status code
         * @retval 0 Success
         * @retval 1 End of file reached
         * @retval -1 Read Error
         */
        virtual int refillBuffer(std::string_view& unprocessed, int& bytesRead);
    };

}

#endif
