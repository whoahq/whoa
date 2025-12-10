if(WHOA_SYSTEM_MAC)
    file(GLOB PRIVATE_SOURCES
        "Test.cpp"
        "stub/Mac.mm"
        "gx/*.cpp"
        "gx/font/*.cpp"
        "util/*.cpp"
    )

    set_source_files_properties(${PRIVATE_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )

    add_executable(WhoaTest ${PRIVATE_SOURCES})

    target_link_libraries(WhoaTest
        PRIVATE
            client
            event
            gx
            util
            "-framework AppKit"
            "-framework Carbon"
            "-framework IOKit"
    )
endif()

if(WHOA_SYSTEM_WIN OR WHOA_SYSTEM_LINUX)
    file(GLOB PRIVATE_SOURCES
        "Test.cpp"
        "gx/*.cpp"
        "gx/font/*.cpp"
        "util/*.cpp"
    )

    add_executable(WhoaTest ${PRIVATE_SOURCES})

    target_link_libraries(WhoaTest
        PRIVATE
            client
            event
            gx
            util
    )
endif()

target_include_directories(WhoaTest
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/vendor/catch2-2.13.10
)

install(TARGETS WhoaTest DESTINATION "bin")
