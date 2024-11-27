cmake_minimum_required(VERSION 3.10)

project(Ewhu)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_VERBOSE_MAKEFILE ON)


# Function to create a library from a folder
function(add_folder_library folder_name)
    file(GLOB SOURCE_FILES ${folder_name}/*.cpp)
    add_library(${folder_name} STATIC ${SOURCE_FILES})
    target_include_directories(${folder_name} PRIVATE ${folder_name})
endfunction()

# Create libraries for each folder


add_library(lexer STATIC lexer/lexer.cpp)
target_include_directories(lexer PRIVATE lexer)

add_library(object STATIC object/object.cpp object/scope.cpp)
target_include_directories(object PRIVATE object)

add_library(ast STATIC ast/node.cpp)
target_include_directories(ast PRIVATE ast)

add_library(parser STATIC parser/boolean.cpp parser/expression.cpp parser/group.cpp
            parser/identifier.cpp parser/if_statement.cpp parser/infix.cpp parser/integer.cpp
             parser/parser.cpp parser/prefix.cpp parser/program.cpp parser/statement_block.cpp
              parser/string.cpp)
target_include_directories(parser PRIVATE parser)

target_link_libraries(parser PUBLIC ast)
add_library(evaluator STATIC evaluator/evaluator.cpp evaluator/fraction.cpp evaluator/identifier.cpp
            evaluator/integer.cpp evaluator/infix.cpp evaluator/if_statement.cpp evaluator/prefix.cpp
            evaluator/program.cpp evaluator/statement_block.cpp) 
target_include_directories(evaluator PRIVATE evaluator)
target_link_libraries(evaluator PUBLIC parser object)

# Add the main executable
add_executable(Ewhu Ewhu.cpp)
target_compile_options(Ewhu PRIVATE -O3)
# Link the libraries to the executable

target_link_libraries(Ewhu PUBLIC evaluator lexer object parser ast)


target_include_directories(Ewhu PRIVATE ${PROJECT_SOURCE_DIR}/rapidjson/include)
# If there are dependencies between libraries, link them accordingly
# Example: target_link_libraries(folder2 PRIVATE folder1)
