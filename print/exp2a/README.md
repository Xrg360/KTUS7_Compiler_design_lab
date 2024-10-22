### Overview

This is a simple lexical analyzer implemented using **Lex** (Flex) for analyzing C-like code. It identifies and categorizes tokens like keywords, identifiers, operators, and preprocessor directives from a given input file. Additionally, it handles comments by skipping them and outputs the token categories in a structured format.

### Features

- **Keywords**: Recognizes common C language keywords such as `int`, `float`, `while`, `for`, `if`, etc.
- **Identifiers**: Detects valid identifiers based on the regular expression `[a-zA-Z][a-zA-Z0-9]*`.
- **Operators**: Identifies operators such as `+`, `-`, `*`, `/`, `=`, relational operators (`<`, `>`, `<=`, `>=`, `==`), etc.
- **Preprocessor Directives**: Recognizes preprocessor directives like `#include`.
- **Comments**: Handles multi-line comments (`/* ... */`) and ignores the content inside.
- **Literals**: Detects string literals (`"..."`) and numeric constants.
- **Blocks**: Identifies the beginning (`{`) and ending (`}`) of blocks.

### How to Compile and Run

#### **Compile the Lex file:**
   ```bash
   flex lexer.l  # This generates lex.yy.c
   gcc lex.yy.c   # This compiles lex.yy.c into an executable named lexer
   ./a.out
   ```


The output would be:

```
#include<stdio.h> is a preprocessor directive
 kwd fun)
 block begins
     kwd id, id;
     fun str);
     fun str,& id,& id);
     fun str, id, id, id op-plus id);
```

### Algorithm

The lexical analyzer operates based on the following algorithm:

1. **Initialization**: 
   - Initialize a flag (`COMMENT`) to track whether the analyzer is inside a comment block.
   - Define regular expressions to match keywords, identifiers, operators, and other tokens.

2. **Preprocessor Directive Handling**: 
   - If the token starts with `#`, it is classified as a preprocessor directive.

3. **Keyword Matching**:
   - For each keyword in C (`int`, `float`, `for`, etc.), output the label `kwd` when matched.

4. **Comment Handling**:
   - When encountering `/*`, set the `COMMENT` flag and ignore content until `*/` is found, marking the end of the comment.

5. **Operator Recognition**:
   - Recognize arithmetic operators (`+`, `-`, `*`, `/`) and relational operators (`<`, `>`, `<=`, `>=`, `==`).

6. **Identifier and Function Recognition**:
   - Use a regular expression to detect valid identifiers, and if followed by `(`, mark them as function calls (`fun`).

7. **Block Detection**:
   - Detect block beginnings `{` and block endings `}`.

8. **String and Number Literals**:
   - Match string literals (`"..."`) and numeric constants and categorize them as `str` and `num` respectively.

9. **Default Behavior**:
   - Echo any unmatched character if not inside a comment block.

10. **Finalization**:
    - The analyzer finishes after processing the entire input file and returns control to the calling function.
