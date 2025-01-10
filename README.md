# Ewhu Interpreter
Ewhu supports arithmetic operations, bitwise operations, logical operations, loop statements, conditional statements, functions, and recursion. It supports integer, fractional, string, and array variable types. Ewhu includes built-in functions for input, output, and array operations, and has error detection capabilities to handle variable name errors and runtime errors.

### Control Statements
 **Loops:**
  ```ewhu
  while() { ... }
  break;
  continue;
  ```
 **Conditional Statements:**
  ```ewhu
  if() {
      ...
  } else {
      ...
  }
  ```

###  Functions
 **Define a Function:**
  ```ewhu
  func function_name(args) { ... }
  ```
 **Call a Function:**
  ```ewhu
  function_name(args);
  ```
 **Return:**
  ```ewhu
  return x;
  ```

###  Operators
 **Infix Operators:** `+`, `-`, `*`, `**`, `/`, `%`, `>`, `<`, `==`, `!=`, `>=`, `<=`, `[]` (for array element access)

 **Prefix Operators:** `+`, `-`, `!`

###  Data Types

 **Integer:** 
  - `int`: implemented as `long long`

 **Boolean:**
  - `bool`: implemented as `long long`

 **Fraction:**
  - `fraction`: implemented as two `long long` values

 **String:**
  - `string`: implemented as `std::string`

 **Array (heterogeneous):**
  - `array`: implemented as `std::vector<std::shared_ptr<Object>>`

###  Built-in Functions
 Print any data type:
  ```ewhu
  print(any) -> null
  ```
 Input function (Python-style):
  ```ewhu
  input(string) -> string
  ```
 Evaluate string (Python-style):
  ```ewhu
  eval(string) -> any
  ```
 Type conversion to int (with type checking):
  ```ewhu
  int(any) -> int
  ```
 Print variables in current scope:
  ```ewhu
  scope() -> none
  ```

###  Array Operation Functions
 Append element to the end:
  ```ewhu
  append(array, any) -> null
  ```
 Pop and return the last element:
  ```ewhu
  pop(array) -> element
  ```
 Return the length of the array:
  ```ewhu
  len(array) -> int
  ```

## Execution Modes
The Ewhu interpreter supports two execution modes:
1. **Interactive Mode:** The interpreter provides real-time feedback on execution results.
2. **File Mode:** Execute a `.ewhu` file via the command line, outputting the results.

## Additional Features
For demonstration purposes, Ewhu can output the abstract syntax tree (AST) constructed from Ewhu code. Using the third-party library `rapidjson`, the AST is outputted as a `.json` file named `ast.json` in the run directory. This file can be visualized using relevant plugins or by visiting [jsoncrack.com](https://jsoncrack.com/).

## bench

```bash
valgrind --tool=callgrind ./Ewhu -b [script]
```
## Count line
```bash
(Get-ChildItem -Recurse -Include *.h, *.cpp | Where-Object { $_.FullName -notmatch '\\(rapidjson|build)\\' } | Get-Content | Measure-Object -Line).Lines
```
now: 3698