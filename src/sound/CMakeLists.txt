file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(sound STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(sound
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(sound
    PRIVATE
        ui
        util
    PUBLIC
        fmod
)
