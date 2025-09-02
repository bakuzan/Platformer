cmake_minimum_required(VERSION 3.15)
project(Platformer)

set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-enum")

# Include SFML
set(SFML_DIR "D:/projects/SFML/x64/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Add your executable
file(GLOB_RECURSE SOURCES "src/**/*.cpp" "src/*.cpp")
message(STATUS "Source files: ${SOURCES}")
add_executable(${PROJECT_NAME} ${SOURCES})

# Enable maximum compiler warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /Zi /Od)
    # target_link_options(${PROJECT_NAME} PRIVATE /fsanitize=address)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror -Wswitch-enum -fsanitize=address -g -O1)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)

# Copy SFML to build location
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-system-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-graphics-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-window-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-audio-d-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-system-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-graphics-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-window-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "D:/projects/SFML/x64/bin/sfml-audio-2.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

# Copy resources folder to the build directory
file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_SOURCE_DIR}/resources/*")
message(STATUS "Resource files: ${RESOURCE_FILES}")

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)