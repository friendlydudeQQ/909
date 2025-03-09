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

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(hate
        src/main.cpp
        src/converter_json.cpp
        src/index.cpp
        src/inverted_index.cpp
)

target_link_libraries(hate PRIVATE nlohmann_json::nlohmann_json)



