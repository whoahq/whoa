if(WHOA_SYSTEM_WIN)
    file(GLOB PRIVATE_SOURCES "win/*.cpp")

    add_executable(Whoa WIN32 ${PRIVATE_SOURCES})

    target_link_libraries(Whoa
        PRIVATE
            client
            event
            gx
            net
            util
        )
endif()

if(WHOA_SYSTEM_MAC)
    file(GLOB PRIVATE_SOURCES "mac/*.cpp" "mac/*.mm")

    set_source_files_properties(${PRIVATE_SOURCES}
        PROPERTIES COMPILE_FLAGS "-x objective-c++"
    )

    add_executable(Whoa ${PRIVATE_SOURCES})

    target_link_libraries(Whoa
        PRIVATE
            client
            event
            gx
            net
            util
            "-framework AppKit"
            "-framework Carbon"
            "-framework IOKit"
    )

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mac/MainMenu.nib DESTINATION "bin")
endif()

if(WHOA_SYSTEM_LINUX)
    file(GLOB PRIVATE_SOURCES "linux/*.cpp")

    add_executable(Whoa ${PRIVATE_SOURCES})

    target_link_libraries(Whoa
        PRIVATE
            client
            event
            gx
            net
            util
    )
endif()

if(WHOA_SYSTEM_WEB)
    file(GLOB PRIVATE_SOURCES "web/*.cpp")

    add_executable(Whoa ${PRIVATE_SOURCES})

    target_link_libraries(Whoa
        PRIVATE
            client
            event
            gx
            net
            util
    )

    set_target_properties(Whoa PROPERTIES SUFFIX ".html")

    target_link_options(Whoa PRIVATE
        "SHELL:--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/web/shell.html"
        -sASYNCIFY
        -sASYNCIFY_STACK_SIZE=524288
        -sALLOW_MEMORY_GROWTH
        "-sASYNCIFY_IMPORTS=['__syscall_openat','__syscall_stat64','__syscall_lstat64','__syscall_fstat64']"
    )
endif()

target_include_directories(Whoa
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

install(TARGETS Whoa DESTINATION "bin")
