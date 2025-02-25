/**
 * @file tokenizer.hpp
 * 
 * @brief Include file for tokenizer class and related types
 */

#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <array>
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
         * @brief Refills the internal buffer with the next content from the source
         *        file, if the file has not already been fully read
         * 
         * @param[out] bytesRead Number of bytes read
         * @return Status code
         * @retval 0 Success
         * @retval -1 Read Error
         */
        int refillBuffer(int& bytesRead);
    };

}

#endif
