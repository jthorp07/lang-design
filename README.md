# An Unnamed Language

This repository is intended to document my personal journey of designing a programming language,
a compilor and maybe interpretor for the language, an IDE or VSCode extension for the language,
and a package manager for the language. The root README will contain my preliminary hopes for 
what the final "product" should look like, and the rest of the story will be documented in the
repository's wiki.

## Contents

1. [Language Overview](#language-overview)
    1. [Features](#features)
    2. [Names](#names)
2. [Implementation Plan](#implementation-plan)
3. [Reserved Words](#reserved-words)
4. [Language Structures](#language-structures)
5. [Primitive Types](#primitive-types)
6. [Syntax Examples](#syntax-examples)

## Language Overview

The grand vision here is to make a platform independent compiled langauge with a C-Like syntax,
but also with semantics that allow the language to be both developer friendly and highly
performant at runtime at the expense of compile time. The compiler for the language should be
able to identify these language semantics, but will not have many (if any) optimizations at
least for this project.

### Features

The following is a broad, not-well categorized list of "things" I would like the language to
have. This list will be better revised and categorized both later in this file and in later
design documents:

- Classes:
    - Named constructors
    - Derived fields
    - Special getter/setter semantics
- Interfaces
- Enums
- Generics
- Type inferencing:
    - Inferred types at declaration
    - Contextual type narrowing
- Limited memory management:
    - Smart Pointers
    - Passing by value/reference
- Garbage collection?
- First class functions
- Continuation passing
- Signals (Similar to the JavaScript signals proposal)
- Modularity:
    - libraries (binaries)
    - modules (sub-libraries)
    - executables (binaries)
- Pattern Matching

### Names

Naturally a language needs a name, and moreso for a programming language a file extension. When
I think about what should define this language, my mind immediately goes to the tradeoffs
between developer ease of use, power, performance, and build times. The phrase "Rome wasn't
build in a day" comes to mind- and therefore to stick with the theme I've decided to call this
The Imperium Language and to have the primary source code file extension be `.imp`.

This part of the nomenclature is very much subject to change, but in an ideal ecosystem I would
like to see the following file types all present:
- `.imp` Source code files
- `.implib` Library declaration files (for package manager)
- `.impi` Include/Declaration files

## Implementation Plan

1. Convert Source Code to AST
    1. Text parser/Tokenizer
    2. Abstract language wrapper (AST)
2. Compile Language to C++ With Exceptions:
    1. No signals
    2. No continuations
    3. No lambdas
    4. No generics
    5. No type inference
3. Add excluded features from last step
4. Flesh out library/module interfaces
5. Add a standard library

| **Step** | **Deliverable**              | **Language** | **Notes**                                 |
| -------- | ---------------------------- | ------------ | ----------------------------------------- |
| 1        | Parser/Tokenizer             | C++          | Optional output of full token list        |
| 2        | AST                          | C++          | Optional tree output                      |
| 3        | Primitive Compilation        | C++          | Global env and single file only           |
| 4        | Add Environments/Scope       | C++          |                                           |
| 5        | Add lambdas, generics        | C++          |                                           |
| 6        | Add signals, type inference  | C++          |                                           |
| 7        | Add continuation semantics   | C++          | *Iterative implementation - not true CPS* |
| 8        | Add compilation units        | C++          |                                           |
| 9        | Make self-contained          | IMP          | *might switch with 10*                    |
| 10       | Implement standard library   | IMP          | *might switch with 9*                     |
| 11       | Implement package manager    | IMP          | Needs broken down into substeps           |


[return to top](#an-unnamed-language)
## Reserved Words

- **Language Constructs:**
    - class
    - enum
    - continuation
    - signal\<type>
    - derived\<type ...>
- **Primitive Type Keywords:**
    - int
    - float
    - array\<type>
- **Type Modifiers:**
    - const
    - static
    - ptr
    - ref
    - final
- **Operators:**
    - \+
    - \-
    - \*
    - /
    - %
    - []
    - \+\+
    - \-\-
    - ::

[return to top](#an-unnamed-language)
## Language Structures

[return to top](#an-unnamed-language)
## Primitive Types

The following primitives will always be available:

| Type | Description | Notes |
| ---- | ----------- | ----- |
| int  | 32-bit integer | |
| float | 64-bit double precision float | |
| char | character | TODO: Choose encoding/size |
| string | array of characters terminated by '\0' | C string go brr |
| function\<ret>\<params> | function primitive | |
| byte | a single byte | |
| ptr\<type> | pointer | |

[return to top](#an-unnamed-language)
## Syntax Examples


Continuation Passing (Note: Won't actually shrink the stack if compiled to C/C++)
```
// Explicit continuation struct
int continuation fibonacci(int n) {
    [
        fibTail = (int n, int value, int nextValue) => {
            if (n == 0) {
                return value;
            } else {
                return fibTail(n - 1, nextValue, value + nextValue);
            }
        }
        fibBase = (int n) => {
            return fibTail(n, 0, 1);
        }
    ],
    fibBase,
    [
        n
    ]
}

// Implicit through pure tail recursion
int fibTail(int n, int value, int nextValue) {
    if (n == 0) {
        return value;
    } else {
        return fibTail(n - 1, nextValue, value + nextValue);
    }
}
```

Example Class Definition
```
class MyClass {
constructors:
fields:
properties:
methods:
    int myMethod();
utilities:
}
```