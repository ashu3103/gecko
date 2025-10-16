add_subdirectory(internal)
add_subdirectory(lexer)
add_subdirectory(parser)

## add main.cpp executable
add_executable(gecko main.cpp)

## link the library
target_link_libraries(gecko PUBLIC core internal lexer parser)
