file(GLOB PRIVATE_SOURCES "*.cpp" "rec/*.cpp")

add_library(db STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(db
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(db
    PUBLIC
        common
        storm
)
