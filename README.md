# Advanced C Compiler

**Unlock the Power of Code Compilation with our Advanced C Compiler!**

This feature-rich C compiler goes beyond traditional tools, offering a cutting-edge lexer, a robust syntax analyzer, and an advanced semantic analyzer. With a foundation built on a comprehensive Context-Free Grammar (CFG), it provides an unparalleled compilation experience. Not only does it meticulously parse and analyze code, but it also visualizes the syntax tree, providing a profound understanding of your code structure.

## Key Features

- **Lexer:** Efficiently tokenizes input C code.
- **Syntax Analyzer:** Parses the token stream to create a detailed syntax tree.
- **Semantic Analyzer:** Implements advanced type checking and semantic analysis.
- **CFG (Context-Free Grammar):** Adheres to a comprehensive set of grammar rules.
- **Syntax Tree Visualization:** Provides a visual representation of the syntax tree.
- **Visualized Analyzers:** Displays the syntax analyzer and semantic analyzer outputs as tree structures.

## Code Organization

### Lexer
Tokenizes the input C code.

### Syntax Analyzer
Parses the token stream and generates a detailed syntax tree.

### Semantic Analyzer
Performs intricate type checking and semantic analysis.

### CFG Definitions
Provides a comprehensive set of grammar rules for code parsing.

### Syntax Tree Visualization
The syntax tree is visualized within the code with comments, offering a clear representation of the code structure. Additionally, the syntax tree is printed to a file (`Parse_tree_output.txt`) for reference.

## Example

   **/*hello my name is sakr*/
   while (i < 5) {
   printf("%d\n", i);
    i++;
   }**

   -*/*Lexical phase:*

     

   -*/*Syntax Analyzer:*

   ![image](https://github.com/Sakr00/Advanced-C-Compiler/assets/111249727/d8b6f535-966e-49be-a599-bd5574a8d369)


   -*/*Semantic Analyzer:*

   ![image](https://github.com/Sakr00/Advanced-C-Compiler/assets/111249727/4ca82414-a4c5-4fb1-85b1-05f490d32391)


   -*/*Syntax Tree:*

   ![image](https://github.com/Sakr00/Advanced-C-Compiler/assets/111249727/e284805d-6ff6-4566-95ec-b101d598786a)


## Effort and Dedication

This project is a testament to our commitment to delivering a high-quality C compiler. The lexer ensures precise tokenization, the syntax analyzer constructs a detailed tree structure, and the semantic analyzer performs intricate type checking. The visualization of these processes enhances the overall understanding of code compilation.

## Getting Started

### Prerequisites

- C++ Compiler (e.g., g++)
- Git (optional)

### Compile and Run

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/your-username/advanced-c-compiler.git
   cd advanced-c-compiler
