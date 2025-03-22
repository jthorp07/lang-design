/**
 * @file lexer.cpp
 * 
 * @brief Implementation file for Lexer class and related types
 */

#include "lexer.hpp"

namespace {

    /**
     * @brief Assemble a string literal token
     * 
     * @param[in] start Start of primitive tokens to assemble
     * @param[in] end End of primitive tokens to assemble
     * @param[out] lexedToken Assembled string literal token
     * @return Status code
     * @retval 0 Sucess
     * @retval -1 End of tokens reached
     */
    int assembleStringLiteral(const auto* start, const auto* end, imperium_lang::LexedToken& lexedToken) {
        /** @todo Start should be entering delimiter, then add tokens until an end delimiter or end of tokens reached  */
    }

}

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