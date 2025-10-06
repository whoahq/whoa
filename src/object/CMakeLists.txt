file(GLOB PRIVATE_SOURCES
    "*.cpp"
    "client/*.cpp"
)

add_library(object STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(object
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(object
    PRIVATE
        db
)
