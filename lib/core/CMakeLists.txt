# Collect source and header files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

# Define the library
add_library(core SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Set version info (use project()'s version)
set_target_properties(core PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    LINKER_LANGUAGE CXX
)

# Public headers (so consumers can include them)
target_include_directories(core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link other dependencies if needed (currently none)
target_link_libraries(core PUBLIC)

# Installation (optional)
include(GNUInstallDirs)
install(TARGETS core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
