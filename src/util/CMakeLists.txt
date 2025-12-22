file(GLOB PRIVATE_SOURCES
    "*.cpp"
)

if(WHOA_SYSTEM_MAC)
    file(GLOB MAC_SOURCES "*.mm")

    set_source_files_properties(${MAC_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )

    list(APPEND PRIVATE_SOURCES ${MAC_SOURCES})
endif()

add_library(util STATIC
    ${PRIVATE_SOURCES}
)

target_include_directories(util
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${ZLIB_INCLUDE_DIRS}
        ${BZIP2_INCLUDE_DIRS}
)

target_link_libraries(util
    PUBLIC
        freetype-2.0
        lua-5.1
        common
        storm
        tempest
        ${ZLIB_LIBRARIES}
        ${BZIP2_LIBRARIES}
)

if(WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_MAC)
    target_link_libraries(util
        PRIVATE
            Threads::Threads
    )
endif()
