# wak

`wak` is a simple game math library

## Installation

You can easily include library using CMake's FetchContent module.

```cmake
include(FetchContent)

FetchContent_Declare(
    wak
    GIT_REPOSITORY https://github.com/Sopiro/wak.git
    GIT_TAG v1.0.0
)

FetchContent_MakeAvailable(wak)

...

target_link_libraries(${PROJECT_NAME} PRIVATE wak)

```
