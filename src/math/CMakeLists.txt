file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(math STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(math
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)
