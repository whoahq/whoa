file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(async STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(async
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(async
    PRIVATE
        event
        util
    PUBLIC
        common
        storm
)
