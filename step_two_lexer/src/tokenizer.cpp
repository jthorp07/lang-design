/**
 * @file tokenizer.cpp
 * 
 * @brief Implementation file for tokenizer class and related types
 */

#include "tokenizer.hpp"
#include "reserved_word_trie.hpp"
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
        "if"sv, "else"sv, "while"sv, "for"sv, "return"sv, "do"sv,
        /* ADTs */
        "class"sv, "function"sv, "enum"sv, "signal"sv, "regex_t"sv,
        /* Access Modifiers */
        "public"sv, "private"sv, "protected"sv,
        /* Primitive Types */
        "int"sv, "string"sv, "bool"sv, "char"sv, "float"sv, "array"sv, "bits"sv,
        /* Type Modifiers */
        "const"sv, "static"sv, "ptr"sv, "ref"sv, "final"sv,
        /* Compilation Unit Control */
        "import"sv, "export"sv, "library"sv, "module"sv,
        /* Semantic keywords */
        "callback_t"sv, "continuation_t"sv, "template"sv
    };
    constexpr auto DELIMITER = ";,.(){}[]<>:\"'"sv;
    constexpr auto WHITESPACE = " \n\t\r"sv;
    constexpr auto DIGIT = "1234567890"sv;
    constexpr auto NON_WHITESPACE_CHARACTER = 
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*_-+=|\\/?~`"sv;
    constexpr auto ESCAPE = "\\"sv;

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
     * @brief Determines if the next token is a comment
     * 
     * @param[in] unprocessed View of unprocessed data
     */
    bool isCommentFirst(const std::string_view& unprocessed);

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
        if (unprocessed.empty()) {
            type = imperium_lang::TokenType::EndOfFile;
        } else if (unprocessed.find_first_of(WHITESPACE) == 0) {
            type = imperium_lang::TokenType::Whitespace;
        } else if (unprocessed.find_first_of(DELIMITER) == 0) {
            type = imperium_lang::TokenType::Delimiter;
        } else if (isCommentFirst(unprocessed)) {
            type = imperium_lang::TokenType::Comment;
        } else if (unprocessed.find_first_of(DIGIT) == 0 && (unprocessed.find_first_not_of(DIGIT) == unprocessed.find_first_of(WHITESPACE) || unprocessed.find_first_not_of(DIGIT) == unprocessed.find_first_of(DELIMITER))) {
            type = imperium_lang::TokenType::Number;
        } else if (unprocessed.find_first_of(NON_WHITESPACE_CHARACTER) == 0) {
            type = imperium_lang::TokenType::CharSequence;
        } else {
            type = imperium_lang::TokenType::Invalid;
        }

        /** @todo Parse token */
        if (type == imperium_lang::TokenType::EndOfFile) {
            content = "";

            return 1;
        } else if (type == imperium_lang::TokenType::Whitespace) {
            std::size_t end = unprocessed.find_first_not_of(WHITESPACE);
            if (end == std::string_view::npos) {
                end = unprocessed.size();
            }
            content = std::string(unprocessed.substr(0, end));
            unprocessed.remove_prefix(end);
    
            return 0;
        } else if (type == imperium_lang::TokenType::Delimiter) {
            content = std::string(unprocessed.substr(0, 1));
            unprocessed.remove_prefix(1);
    
            return 0;
        } else if (type == imperium_lang::TokenType::Comment) {
            if (unprocessed[1] == '/') {
                std::size_t end = unprocessed.find('\n');
                if (end == std::string_view::npos) {
                    end = unprocessed.size();
                } else {
                    --end;
                }
                content = std::string(unprocessed.substr(0, end));
                unprocessed.remove_prefix(end);
            } else if (unprocessed[1] == '*') {
                std::size_t end = unprocessed.find("*/");
                if (end == std::string_view::npos) {
                    end = unprocessed.size();
                }
                content = std::string(unprocessed.substr(0, end + 2));
                unprocessed.remove_prefix(end + 2);
            }

            return 0;
        } else if (type == imperium_lang::TokenType::Number) {
            std::size_t end = unprocessed.find_first_not_of(DIGIT);
            if (end == std::string_view::npos) {
                end = unprocessed.size();
            }
            content = std::string(unprocessed.substr(0, end));
            unprocessed.remove_prefix(end);
    
            return 0;
        } else if (type == imperium_lang::TokenType::CharSequence) {
            std::size_t end = unprocessed.find_first_not_of(NON_WHITESPACE_CHARACTER);
            if (end == std::string_view::npos) {
                end = unprocessed.size();
            }
            content = std::string(unprocessed.substr(0, end));
            if (isReservedWord(content)) {
                type = imperium_lang::TokenType::ReservedWord;
            }
            unprocessed.remove_prefix(end);
    
            return 0;
        } else if (type == imperium_lang::TokenType::CharSequence) {
            std::size_t end = std::min(unprocessed.find_first_of(WHITESPACE), unprocessed.find_first_of(DELIMITER));
            if (end == std::string_view::npos) {
                end = unprocessed.size();
            }
            content = std::string(unprocessed.substr(0, end));
            unprocessed.remove_prefix(end);

            return 0;
        } else {
            std::cerr << "Error: Invalid token type\n";

            return -1;
        }

        return -1; // Error case: should never reach here
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
        std::copy(unprocessed.cbegin(), unprocessed.cend(), buffer.begin());
        source.read(buffer.data() + unprocessed.size(), imperium_lang::BUFFER_SIZE - unprocessed.size());
        bytesRead = source.gcount();
        if (bytesRead <= 0) {
            std::cerr << "Error: Failed to read from source file.\n";
            return -1;
        }
        unprocessed = std::string_view(buffer.cbegin(), unprocessed.size() + bytesRead);
        if (source.eof()) {
            return 1;
        }

        return 0;
    }

    /**
     * @brief Determines if the next token is a comment
     * 
     * @param[in] unprocessed View of unprocessed data
     */
    bool isCommentFirst(const std::string_view& unprocessed) {
        return unprocessed.find("//") == 0 || unprocessed.find("/*") == 0;
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
        std::ifstream source(sourceFile);
        if (!source) {
            std::cerr << "Error: Failed to open source file.\n";
            return -2;
        }
        int totalBytesRead;
        std::string_view unprocessed;
        auto refillStatus = refillBuffer(unprocessed, buffer, totalBytesRead, source);
        if (refillStatus == -1) {
            std::cerr << "Error: Failed to read from source file.\n";
            return -2;
        }
        bool doneReading = refillStatus == 1;
        while (true) {
            Token token;
            int bytesRead;
            int extractStatus = extractFirstToken(unprocessed, token.type, token.value, bytesRead);
            if (extractStatus == -1) {
                std::cerr << "Parse Error: Failed to extract token.\n";
                return -1;
            } else if (extractStatus == -2) {
                std::cerr << "Read Error: Failed to read from source file.\n";
                return -2;
            } else if (extractStatus == 1) {
                break;
            } else if (extractStatus == 0) {
                tokens.push_back(token);
            }
            if (unprocessed.size() < BLOCK_SIZE && !doneReading) {
                refillStatus = refillBuffer(unprocessed, buffer, totalBytesRead, source);
                if (refillStatus == -1) {
                    std::cerr << "Error: Failed to read from source file.\n";
                    return -2;
                } else if (refillStatus == 1) {
                    doneReading = true;
                }
            }
        }

        return 0;
    }
}
