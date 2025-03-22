/**
 * @file lexer.cpp
 * 
 * @brief Implementation file for Lexer class and related types
 */

#include "lexer.hpp"

namespace imperium_lang {

    /**
     * @brief Assemble primitive tokens into more context-sensitive token constructs
     * 
     * @param[out] lexedTokens Assembled tokens
     * @param[in] tokens Tokens to assemble
     * @return Status code
     * @retval 0 Success
     * @retval -1 Invalid token sequence
     */
    int Lexer::lexTokens(std::vector<LexedToken> &lexedTokens, const auto& tokens) {
        /** @todo Iterate over tokens and use finite automata to construct context sensitive tokens */
    }
    
}