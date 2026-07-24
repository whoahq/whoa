file(GLOB PRIVATE_SOURCES
    "*.cpp"
    "guid/*.cpp"
    "time/*.cpp"
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
)

target_link_libraries(util
    PUBLIC
        freetype-2.0
        lua-5.1
        common
        storm
        tempest
    PRIVATE
        zlib-1.2
)

if(WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_MAC)
    target_link_libraries(util
        PRIVATE
            Threads::Threads
    )
endif()

if(WHOA_SYSTEM_WEB)
    set(FETCHFS_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/web/library_fetchfs.js)
    target_link_options(util
        PUBLIC
            --js-library ${FETCHFS_LIBRARY}
    )
    # Ensure rebuild when JS library changes
    set_property(TARGET util APPEND PROPERTY LINK_DEPENDS ${FETCHFS_LIBRARY})
endif()
