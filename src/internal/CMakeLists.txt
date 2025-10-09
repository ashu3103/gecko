# Collect source and header files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")

# Define the library
add_library(internal SHARED ${SOURCE_FILES} ${HEADER_FILES})

# Set version info (use project()'s version)
set_target_properties(internal PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    LINKER_LANGUAGE CXX
)

# Public headers (so consumers can include them)
target_include_directories(internal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link other dependencies if needed (currently none)
target_link_libraries(internal PUBLIC)

# Installation (optional)
include(GNUInstallDirs)
install(TARGETS internal
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
