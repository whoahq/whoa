file(GLOB PRIVATE_SOURCES "*.cpp")

add_library(component STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(component
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(component
    PRIVATE
        db
        model
    PUBLIC
        storm
)
