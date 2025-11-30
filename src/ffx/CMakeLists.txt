file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(ffx STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(ffx
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(ffx
    PRIVATE
        console
        gx
    PUBLIC
        storm
)
