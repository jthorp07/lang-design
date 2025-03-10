/**
 * @file tokenizer.cpp
 * 
 * @brief Implementation file for tokenizer class and related types
 */

#include "tokenizer.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <array>

// Allow the use of string_view literals
using namespace std::literals::string_view_literals;

namespace {
    constexpr auto KEYWORD_TOKENS = {
        /* Control Flow */
        "if"sv, "else"sv, "while"sv, "for"sv, "return"sv,
        /* Declaration */
        "class"sv, "function"sv, "var"sv,
        /* Primitive Types */
        "int"sv, "string"sv, "bool"sv, "char"sv, "float"sv, "array"sv
        /* Type Modifiers */
        "const"sv, "static"sv, "ptr"sv, "ref"sv
        /* Compilation Unit Control */
        "import"sv, "export"sv, "library"sv, 
    };

    constexpr auto OPERATOR_TOKENS = {
        /* Arithmetic */
        "+"sv, "-"sv, "*"sv, "/"sv, "%"sv,
        /* Mutator */
        "="sv, "++"sv, "--"sv,
        /* Bitwise */
        "&"sv, "|"sv, "^"sv, "~"sv,
        /* Boolean */
        "=="sv, "!="sv, "<"sv, "<="sv, ">"sv, ">="sv, "&&"sv, "||"sv
    };

    constexpr auto DELIMITER = ";,.(){}[]"sv;
    constexpr auto WHITESPACE = " \n\t\r"sv;

        /**
     * @brief Extracts the next token from the buffer
     * 
     * @param[in, out] unprocessed View of unprocessed data
     * @param[out] type The extracted token's type
     * @param[out] content The extracted token's content
     * @param[out] bytesRead Number of bytes read
     * @return Status code
     * @retval 0 Success
     * @retval 1 End of file reached
     * @retval -1 Parse Error
     * @retval -2 Read Error
     */
    int extractFirstToken(std::string_view& unprocessed, imperium_lang::TokenType& type, std::string& content, int& bytesRead);

    /**
     * @brief Shifts unprocessed data to the start of a buffer, then refills
     *        the buffer with the next data from the source file.
     * 
     * @param[in, out] unprocessed View of unprocessed data
     * @param[in, out] buffer Buffer to fill
     * @param[out] bytesRead Number of bytes read
     * @param[in] source The source stream to read from
     * @return Status code
     * @retval 0 Success
     * @retval 1 End of file reached
     * @retval -1 Read Error
     */
    int refillBuffer(std::string_view& unprocessed, auto& buffer, int& bytesRead, auto& source);

    /**
     * @brief Determines if the next token is an operator
     * 
     * @param[in] unprocessed View of unprocessed data
     */
    bool isOperatorFirst(const std::string_view& unprocessed);

    /**
     * @brief Determines if the next token is a keyword
     * 
     * @param[in] unprocessed View of unprocessed data
     */
    bool isKeywordFirst(const std::string_view& unprocessed);

    /**
     * @brief Extracts a whitespace token from the beginning of the buffer
     * 
     * @param[in, out] unprocessed View of unprocessed data
     * @param[out] content The extracted whitespace content
     * @param[out] bytesRead Number of bytes read
     * @return Status code
     * @retval 0 Success
     * @retval 1 End of file reached
     * @retval -1 Parse Error
     * @retval -2 Read Error
     */
    int extractWhitespaceToken(std::string_view& unprocessed, std::string& content, int& bytesRead);

    /**
     * @brief Extracts the next token from the buffer
     * 
     * @param[in, out] unprocessed View of unprocessed data
     * @param[out] type The extracted token's type
     * @param[out] content The extracted token's content
     * @param[out] bytesRead Number of bytes read
     * @return Status code
     * @retval 0 Success
     * @retval 1 End of file reached
     * @retval -1 Parse Error
     * @retval -2 Read Error
     */
    int extractFirstToken(std::string_view& unprocessed, imperium_lang::TokenType& type, std::string& content, int& bytesRead) {

        /** @todo Identify type of token to parse */
        if (unprocessed.find_first_of(WHITESPACE) == 0) {
            type = imperium_lang::TokenType::Whitespace;
        } else if (unprocessed.find_first_of(DELIMITER) == 0 && unprocessed[1] == '=') {
            type = imperium_lang::TokenType::Delimiter;
        } else if (isOperatorFirst(unprocessed)) {
            type = imperium_lang::TokenType::Operator;
        } else if (isKeywordFirst(unprocessed)) {
            type = imperium_lang::TokenType::Keyword;
        } else {
            type = imperium_lang::TokenType::Identifier;
        }

        /** @todo Parse token */
        switch(type) {
            case imperium_lang::TokenType::Whitespace:
                return extractWhitespaceToken(unprocessed, content, bytesRead);
            default:
                std::cerr << "Error: Invalid token type\n";
                return -1;
        }

        return 0;
    }

    /**
     * @brief Shifts unprocessed data to the start of a buffer, then refills
     *        the buffer with the next data from the source file.
     * 
     * @param[in, out] unprocessed View of unprocessed data
     * @param[in, out] buffer Buffer to fill
     * @param[out] bytesRead Number of bytes read
     * @param[in] source The source stream to read from
     * @return Status code
     * @retval 0 Success
     * @retval 1 End of file reached
     * @retval -1 Read Error
     */
    int refillBuffer(std::string_view& unprocessed, auto& buffer, int& bytesRead, auto& source) {

        /** @todo Move all unprocessed data to start of buffer */
        std::copy(unprocessed.cbegin(), unprocessed.cend(), buffer.begin());

        /** @todo Read new data into buffer */
        source.read(buffer.data() + unprocessed.size(), imperium_lang::BUFFER_SIZE - unprocessed.size());
        bytesRead = source.gcount();
        if (bytesRead <= 0) {
            std::cerr << "Error: Failed to read from source file.\n";
            return -1;
        }

        /** @todo Reassign unprocessed to cover all content in buffer */
        unprocessed = std::string_view(buffer.cbegin(), unprocessed.size() + bytesRead);
        if (source.eof()) {
            return 1;
        }

        return 0;
    }

    /**
     * @brief Determines if the next token is an operator
     * 
     * @param[in] unprocessed View of unprocessed data
     */
    bool isOperatorFirst(const std::string_view& unprocessed) {
        return std::any_of(OPERATOR_TOKENS.begin(), OPERATOR_TOKENS.end(), 
            [&unprocessed](const auto& token) {
                return unprocessed.find(token) == 0;
            });
    }

    /**
     * @brief Determines if the next token is a keyword
     * 
     * @param[in] unprocessed View of unprocessed data
     */
    bool isKeywordFirst(const std::string_view& unprocessed) {
        return std::any_of(KEYWORD_TOKENS.begin(), KEYWORD_TOKENS.end(), 
            [&unprocessed](const auto& token) {
                return unprocessed.find(token) == 0;
            });
    }

    /**
     * @brief Extracts a whitespace token from the beginning og the buffer
     * 
     * @param[in, out] unprocessed View of unprocessed data
     * @param[out] content The extracted whitespace value
     * @param[out] bytesRead Number of bytes read
     * @return Status code
     * @retval 0 Success
     * @retval 1 End of file reached
     * @retval -1 Parse Error
     * @retval -2 Read Error
     */
    int extractWhitespaceToken(std::string_view& unprocessed, std::string& content, int& bytesRead) {
        auto end = unprocessed.find_first_not_of(WHITESPACE);
        if (end == std::string_view::npos) {
            end = unprocessed.size();
        }
        content = std::string(unprocessed.substr(0, end));
        unprocessed.remove_prefix(end);

        return 0;
    }
}

namespace imperium_lang {

    /**
     * @brief Constructor
     * 
     * @param[in] sourceFile The source file to tokenize
     */
    Tokenizer::Tokenizer(const std::string& sourceFile) : sourceFile(sourceFile) {}

    /**
     * @brief Tokenize the source file
     * 
     * @param[out] tokens The tokens extracted from the source file
     * @return Status code
     * @retval 0 Success
     * @retval -1 Parse Error
     * @retval -2 Read Error
     */
    int Tokenizer::tokenize(std::vector<Token>& tokens) {

        tokens.clear();
        
        /** 
            @todo Implement the tokenization algorithm

            Algorithm:
            1. Open stream to source file
            2. Fill buffer with first chunk of data from source
            3. Enter main parse loop:
                a. Parse tokens from buffer
                b. If buffer is near empty and not done reading, refill buffer
                c. If end of file is reached, finish tokenizing
        */

        /** @todo Open stream to source file */
        std::ifstream source(sourceFile);
        if (!source) {
            std::cerr << "Error: Failed to open source file.\n";
            return -2;
        }
    
        /** @todo Initialize buffer with start of source data */
        int bytesRead;
        std::string_view unprocessed;
        auto refillStatus = refillBuffer(unprocessed, buffer, bytesRead, source);
        if (refillStatus == -1) {
            std::cerr << "Error: Failed to read from source file.\n";
            return -2;
        }

        /** @todo Finish main parse loop */
        bool doneReading = refillStatus == 1;
        while (true) {
            /** @todo Parse tokens from start of content */

            /** @todo If buffer near empty and not done reading, refill buffer */

            /** @todo If end of file is reached, finish tokenizing */
            break;
        }

        return 0;
    }

}