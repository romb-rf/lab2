cmake_minimum_required(VERSION 3.16)

project(lab2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Загрузка Google Test ---
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# --- 2. Поиск Qt ---
find_package(Qt6 REQUIRED COMPONENTS Quick Widgets Core)
qt_standard_project_setup(REQUIRES 6.8)

# --- 3. Статическая библиотека с данными и логикой (без GUI) ---
# Она будет использоваться и в приложении, и в тестах
add_library(lab2core STATIC
    datamanager.cpp
    arraystructure.cpp
    vectorstructure.cpp
    stackstructure.cpp
    queuestructure.cpp
)
target_include_directories(lab2core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(lab2core PUBLIC Qt6::Core)

# --- 4. Основное приложение (GUI) ---
qt_add_executable(applab2
    main.cpp
    visual.cpp
    cellwidget.cpp
    visualwindow.cpp
)

qt_add_qml_module(applab2
    URI lab2
    QML_FILES
        Main.qml
    SOURCES
        arraystructure.h
        vectorstructure.h
        stackstructure.h
        queuestructure.h
        datamanager.h
        element.h
        cellwidget.h
        visual.h
        visualwindow.h
)

target_link_libraries(applab2 PRIVATE
    lab2core
    Qt6::Quick
    Qt6::Widgets
)
# Копирование Qt DLL и плагинов для applab2
find_program(WINDEPLOYQT_EXECUTABLE windeployqt
    HINTS "${QT6_INSTALL_PREFIX}/bin"
)
if(WINDEPLOYQT_EXECUTABLE)
    add_custom_command(TARGET applab2 POST_BUILD
        COMMAND ${WINDEPLOYQT_EXECUTABLE}
            $<TARGET_FILE:applab2>
            --qmldir "${CMAKE_SOURCE_DIR}"   # чтобы подхватились QML-модули
        COMMENT "Deploying Qt dependencies for applab2..."
    )
endif()
# Настройки платформы (оставлены без изменений)
set_target_properties(applab2 PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --- 5. Поддержка тестов ---
enable_testing()
add_subdirectory(Tests)

# --- 6. Установка ---
include(GNUInstallDirs)
install(TARGETS applab2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
