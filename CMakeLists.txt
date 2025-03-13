cmake_minimum_required(VERSION 3.29)
project(hate)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.11.0
)

FetchContent_MakeAvailable(gtest)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(hate
        src/main.cpp
        src/converter_json.cpp
        src/index.cpp
        src/inverted_index.cpp
        src/search_server.cpp
)

target_link_libraries(hate PRIVATE nlohmann_json::nlohmann_json)

enable_testing()

add_executable(test_hate
        tests/test_inverted_index.cpp
        src/index.cpp
        src/inverted_index.cpp
        src/search_server.cpp
        tests/test_index.cpp
)

target_link_libraries(test_hate PRIVATE gtest gtest_main nlohmann_json::nlohmann_json)

add_test(NAME test_hate COMMAND test_hate)











