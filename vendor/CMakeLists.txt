add_subdirectory(freetype-2.0.9)
add_subdirectory(lua-5.1.3)

# FMOD

add_library(fmod SHARED IMPORTED GLOBAL)

# Determine which flavor of FMOD to use:
# - macOS versions from 10.9 down prefer FMOD Ex
# - macOS versions from 10.10 up prefer FMOD Core
# - Windows versions from Vista down prefer FMOD Ex
# - Windows versions from 7 up prefer FMOD Core
# - All versions of Linux prefer FMOD Core
# - Any missed detection prefers FMOD Core
if(WHOA_SYSTEM_MAC AND WHOA_SYSTEM_VERSION LESS WHOA_MAC_10_10)
    set(WHOA_FMOD_EX 1)
    target_compile_definitions(fmod INTERFACE -DWHOA_FMOD_EX=1)
elseif(WHOA_SYSTEM_WIN AND WHOA_SYSTEM_VERSION LESS WHOA_WIN_7)
    set(WHOA_FMOD_EX 1)
    target_compile_definitions(fmod INTERFACE -DWHOA_FMOD_EX=1)
else()
    set(WHOA_FMOD_CORE 1)
    target_compile_definitions(fmod INTERFACE -DWHOA_FMOD_CORE=1)
endif()

if(WHOA_FMOD_EX)
    set(FMOD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fmodex-4.24.16")
else()
    set(FMOD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/fmodcore-2.02.18")
endif()

if(WHOA_SYSTEM_WIN)
    if(WHOA_ARCH_32)
        if(WHOA_FMOD_EX)
            set_target_properties(fmod PROPERTIES
                IMPORTED_IMPLIB ${FMOD_DIR}/win-x86/lib/fmodex_vc.lib
                IMPORTED_LOCATION ${FMOD_DIR}/win-x86/lib/fmodex.dll
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/win-x86/inc
            )
            install(FILES ${FMOD_DIR}/win-x86/lib/fmodex.dll DESTINATION "bin")
        else()
            set_target_properties(fmod PROPERTIES
                IMPORTED_IMPLIB ${FMOD_DIR}/win-x86/lib/fmod_vc.lib
                IMPORTED_LOCATION ${FMOD_DIR}/win-x86/lib/fmod.dll
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/win-x86/inc
            )
            install(FILES ${FMOD_DIR}/win-x86/lib/fmod.dll DESTINATION "bin")
        endif()
    elseif(WHOA_ARCH_64)
        if(WHOA_FMOD_EX)
            set_target_properties(fmod PROPERTIES
                IMPORTED_IMPLIB ${FMOD_DIR}/win-x86_64/lib/fmodex64_vc.lib
                IMPORTED_LOCATION ${FMOD_DIR}/win-x86_64/lib/fmodex64.dll
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/win-x86_64/inc
            )
            install(FILES ${FMOD_DIR}/win-x86_64/lib/fmodex64.dll DESTINATION "bin")
        else()
            set_target_properties(fmod PROPERTIES
                IMPORTED_IMPLIB ${FMOD_DIR}/win-x86_64/lib/fmod_vc.lib
                IMPORTED_LOCATION FMOD_DIR}/win-x86_64/lib/fmod.dll
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/win-x86_64/inc
            )
            install(FILES ${FMOD_DIR}/win-x86_64/lib/fmod.dll DESTINATION "bin")
        endif()
    endif()
elseif(WHOA_SYSTEM_LINUX)
    if(WHOA_ARCH_32)
        if(WHOA_FMOD_EX)
            set_target_properties(fmod PROPERTIES
                IMPORTED_LOCATION ${FMOD_DIR}/linux-x86/lib/libfmodex-4.24.16.so
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/linux-x86/inc
            )
            install(FILES ${FMOD_DIR}/linux-x86/lib/libfmodex-4.24.16.so DESTINATION "bin")
        else()
            # TODO Add ARM support for FMOD Core on Linux

            set_target_properties(fmod PROPERTIES
                IMPORTED_LOCATION ${FMOD_DIR}/linux-x86/lib/libfmod.so.13.18
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/linux-x86/inc
            )
            install(FILES ${FMOD_DIR}/linux-x86/lib/libfmod.so.13.18 DESTINATION "bin")
        endif()
    elseif(WHOA_ARCH_64)
        if(WHOA_FMOD_EX)
            set_target_properties(fmod PROPERTIES
                IMPORTED_LOCATION ${FMOD_DIR}/linux-x86_64/lib/libfmodex-4.24.16.so
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/linux-x86_64/inc
            )
            install(FILES ${FMOD_DIR}/linux-x86_64/lib/libfmodex-4.24.16.so DESTINATION "bin")
        else()
            # TODO Add ARM support for FMOD Core on Linux

            set_target_properties(fmod PROPERTIES
                IMPORTED_LOCATION ${FMOD_DIR}/linux-x86_64/lib/libfmod.so.13.18
                INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/linux-x86_64/inc
            )
            install(FILES ${FMOD_DIR}/linux-x86_64/lib/libfmod.so.13.18 DESTINATION "bin")
        endif()
    endif()
elseif(WHOA_SYSTEM_MAC)
    # The Mac lib is universal

    if(WHOA_FMOD_EX)
        set_target_properties(fmod PROPERTIES
            IMPORTED_LOCATION ${FMOD_DIR}/mac/lib/libfmodex.dylib
            INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/mac/inc
        )
        install(FILES ${FMOD_DIR}/mac/lib/libfmodex.dylib DESTINATION "bin")
    else()
        set_target_properties(fmod PROPERTIES
            IMPORTED_LOCATION ${FMOD_DIR}/mac/lib/libfmod.dylib
            INTERFACE_INCLUDE_DIRECTORIES ${FMOD_DIR}/mac/inc
        )
        install(FILES ${FMOD_DIR}/mac/lib/libfmod.dylib DESTINATION "bin")
    endif()
endif()
