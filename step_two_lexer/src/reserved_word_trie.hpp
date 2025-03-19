/**
 * @file reserved_word_trie.hpp
 * 
 * @brief Include file for compile time reserved word trie
 */

#ifndef RESERVED_WORD_TRIE_HPP
#define RESERVED_WORD_TRIE_HPP

#include <array>
#include <string_view>

// Allow string_view literals
using namespace std::literals::string_view_literals;

namespace {
    constexpr auto RESERVED_WORDS = {
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

    constexpr std::size_t ALPHABET_SIZE = 128; // ASCII character set
    constexpr std::size_t MAX_NODES = 256; // Maximum number of nodes in the trie. Increase if needed.

    struct TrieNode {
        bool isEnd;
        std::array<std::size_t, ALPHABET_SIZE> children;
    };

    struct Trie {
        std::array<TrieNode, MAX_NODES> nodes;
        std::size_t nodeCount;
    };

    /**
     * @brief Builds a trie at compile time
     * 
     * @param[in] words The words to add to the trie
     */
    constexpr Trie buildTrie(const auto& words) {

        Trie trie{};
        trie.nodes[0].isEnd = false;
        for (std::size_t i = 0; i < ALPHABET_SIZE; ++i)
            trie.nodes[0].children[i] = 0;
        trie.nodeCount = 1;
        for (const auto& word : words) {
            std::size_t current = 0;
            for (char c : word) {
                std::size_t index = static_cast<std::size_t>(c);
                if (trie.nodes[current].children[index] == 0) {
                    std::size_t newNode = trie.nodeCount;
                    trie.nodes[current].children[index] = newNode;
                    trie.nodes[newNode].isEnd = false;
                    for (std::size_t j = 0; j < ALPHABET_SIZE; ++j)
                        trie.nodes[newNode].children[j] = 0;
                    ++trie.nodeCount;
                }
                current = trie.nodes[current].children[index];
            }
            trie.nodes[current].isEnd = true;
        }
        return trie;
    }

    constexpr Trie RESERVED_WORD_TRIE = buildTrie(RESERVED_WORDS);
}

/**
 * @brief Checks if a given character sequence is a reserved word
 * 
 * @param[in] charSequence The character sequence to check
 */
constexpr bool isReservedWord(const std::string_view& charSequence) {
    std::size_t current = 0;
    for (char c : charSequence) {
        std::size_t index = static_cast<std::size_t>(c);
        if (RESERVED_WORD_TRIE.nodes[current].children[index] == 0)
            return false;
        current = RESERVED_WORD_TRIE.nodes[current].children[index];
    }
    return RESERVED_WORD_TRIE.nodes[current].isEnd;
}

#endif
