/**
 * @file step_one.cpp
 * 
 * @brief Driver file to run a demo of the project reflecting the progress made in step one.
 */

#include <iostream>
#include <vector>
#include "tokenizer.hpp"

int main(int argc, char** argv) {

    // Extract tokens from demo file
    if (argc < 2) {
        std::cerr << "Error: No source file provided.\n";
        return 1;
    }
    imperium_lang::Tokenizer tokenizer{argv[1]};
    std::vector<imperium_lang::Token> tokens{};
    const auto status = tokenizer.tokenize(tokens);
    if (status != 0) {
        std::cerr << "Error: Tokenization failed.\n";
        return -1;
    }

    // Output token data
    if (tokens.empty()) {
        std::cout << "No tokens were returned.\n";
    } else {
        std::cout << "Tokens:\n";
        for (const auto& token : tokens) {
            std::cout << "Type: " << imperium_lang::tokenTypeToString(token.type) << ", Value: " << token.value << "\n";
        }
    }

    return 0;
}
