cmake_minimum_required(VERSION 3.10)
project(Prometheus)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Automatically find Clang and LLVM (assumes they are installed and in the system PATH)
find_package(Clang REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

# Add the CLI11 library
add_subdirectory(external/CLI11)

# Include Clang and LLVM directories
include_directories(${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
include_directories(external/CLI11)

# Define source files
set(SOURCE_FILES
        src/main.cpp
        src/ASTProcessor.cpp
        src/ASTProcessorFactory.cpp
        src/ASTProcessorConsumer.cpp
        src/Configuration.cpp
        src/Driver.cpp
        src/FileUtils.cpp
        src/ReplaceDeprecatedCommand.cpp
)

# Add the include directory
include_directories(include)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link necessary Clang and LLVM libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        clangTooling
        clangBasic
        clangAST
        clangFrontend
        LLVM
        clangAST
        clangBasic
        clangFrontend
        clangLex
        clangTooling
        clangRewrite
        clangRewriteFrontend
        clangDriver
        clangSerialization
        clangAnalysis
        clangEdit
        clangSema
        clangParse
        clangASTMatchers
        clangToolingCore
        clangFormat
        clangCodeGen
        clangDynamicASTMatchers
        clangSupport
)

# Link the CLI11 library
target_link_libraries(${PROJECT_NAME} PRIVATE
        CLI11
)
