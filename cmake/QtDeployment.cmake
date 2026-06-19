include_guard(GLOBAL)

function(qt_add_default_windows_prefixes)
    if(NOT WIN32)
        return()
    endif()

    set(_qt_search_roots)

    foreach(_qt_env_var QTDIR Qt_DIR Qt6_DIR Qt5_DIR)
        if(DEFINED ENV{${_qt_env_var}} AND NOT "$ENV{${_qt_env_var}}" STREQUAL "")
            list(APPEND _qt_search_roots "$ENV{${_qt_env_var}}")
        endif()
    endforeach()

    list(APPEND _qt_search_roots
        "C:/Qt"
        "$ENV{SystemDrive}/Qt"
        "$ENV{ProgramFiles}/Qt"
        "$ENV{ProgramFiles\(x86\)}/Qt"
    )

    set(_qt_prefixes)

    foreach(_qt_search_root IN LISTS _qt_search_roots)
        if(IS_DIRECTORY "${_qt_search_root}")
            file(GLOB _qt_installed_versions
                LIST_DIRECTORIES TRUE
                "${_qt_search_root}/[0-9]*"
            )
            list(SORT _qt_installed_versions COMPARE NATURAL ORDER DESCENDING)

            foreach(_qt_installed_version IN LISTS _qt_installed_versions)
                file(GLOB _qt_install_prefixes
                    LIST_DIRECTORIES TRUE
                    "${_qt_installed_version}/msvc*"
                    "${_qt_installed_version}/mingw*"
                    "${_qt_installed_version}/clang*"
                )
                list(SORT _qt_install_prefixes COMPARE NATURAL ORDER DESCENDING)

                list(APPEND _qt_prefixes ${_qt_install_prefixes})
            endforeach()

            list(APPEND _qt_prefixes "${_qt_search_root}")
        endif()
    endforeach()

    list(APPEND CMAKE_PREFIX_PATH ${_qt_prefixes})
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
endfunction()

function(qt_find_windows_deploy_tool out_variable)
    if(NOT WIN32)
        set(${out_variable} "" PARENT_SCOPE)
        return()
    endif()

    get_target_property(_qt_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qt_qmake_executable}" DIRECTORY)

    find_program(_qt_windeployqt_executable
        NAMES windeployqt
        HINTS "${_qt_bin_dir}"
        REQUIRED
    )

    set(${out_variable} "${_qt_windeployqt_executable}" PARENT_SCOPE)
endfunction()

function(qt_deploy_runtime target_name)
    if(NOT WIN32)
        return()
    endif()

    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "qt_deploy_runtime target does not exist: ${target_name}")
    endif()

    qt_find_windows_deploy_tool(_qt_windeployqt_executable)

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND "${_qt_windeployqt_executable}"
            $<IF:$<CONFIG:Debug>,--debug,--release>
            --no-translations
            "$<TARGET_FILE:${target_name}>"
        COMMENT "Deploying Qt runtime dependencies for ${target_name}"
        VERBATIM
    )
endfunction()
