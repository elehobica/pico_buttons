if (NOT TARGET pico_buttons)
    add_library(pico_buttons INTERFACE)

    target_sources(pico_buttons INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/Buttons.cpp
    )

    target_include_directories(pico_buttons INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries(pico_buttons INTERFACE
        pico_stdlib
    )
endif()