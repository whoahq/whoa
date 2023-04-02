file(GLOB PRIVATE_SOURCES
    "*.cpp"
)

add_library(console STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(console
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(console
    PUBLIC
        common
        storm
    PRIVATE
        client
)
