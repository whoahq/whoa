file(GLOB PRIVATE_SOURCES "*.cpp")

if(WHOA_SYSTEM_WEB)
    # For web builds, exclude FMOD-dependent files and use stubs
    list(REMOVE_ITEM PRIVATE_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/SESound.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/SESoundInternal.cpp
    )

    file(GLOB WEB_SOURCES "web/*.cpp")
    list(APPEND PRIVATE_SOURCES ${WEB_SOURCES})
endif()

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
)

if(NOT WHOA_SYSTEM_WEB)
    target_link_libraries(sound
        PUBLIC
            fmod
    )
endif()
