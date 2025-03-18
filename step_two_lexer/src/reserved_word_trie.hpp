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

    constexpr std::size_t ALPHABET_SIZE = 256; // ASCII character set
    constexpr std::size_t MAX_NODES = 128; // Maximum number of nodes in the trie

    struct TrieNode {
        bool isEnd;
        std::array<std::size_t, ALPHABET_SIZE> children;
    };

    struct Trie {
        std::array<TrieNode, MAX_NODES> nodes;
        std::size_t nodeCount;
    };

    constexpr Trie buildTrie() {
        Trie trie{};
        trie.nodes[0].isEnd = false;
        trie.nodeCount = 1;
        
        return trie;
    }
}



#endif
