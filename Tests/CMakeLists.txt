add_executable(test_structures
    main.cpp
    tst_array.cpp
    tst_vector.cpp
    tst_stack.cpp
    tst_queue.cpp
    tst_datamanager.cpp
)

target_link_libraries(test_structures PRIVATE
    GTest::gtest
    GTest::gtest_main
    lab2core
    Qt6::Core
)

# Автокопирование зависимостей Qt (устраняет ошибку 0xc0000135)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt
    HINTS "${QT6_INSTALL_PREFIX}/bin"
)
if(WINDEPLOYQT_EXECUTABLE)
    add_custom_command(TARGET test_structures POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE}
            --no-translations
            --no-plugins
            --no-system-d3d-compiler
            $<TARGET_FILE:test_structures>
        COMMENT "Deploying Qt DLLs for test_structures..."
    )
endif()

include(GoogleTest)
gtest_discover_tests(test_structures)
