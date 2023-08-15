if (NOT TARGET buttons)
    add_library(buttons INTERFACE)

    target_sources(buttons INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/Buttons.cpp
    )

    target_include_directories(buttons INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries(buttons INTERFACE
        pico_stdlib
    )
endif()