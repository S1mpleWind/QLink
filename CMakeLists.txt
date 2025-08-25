cmake_minimum_required(VERSION 3.16)

project(QLink VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt Widgets 模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 用于单元测试
find_package(Qt6 REQUIRED COMPONENTS Test)

# -------------------- 主程序源文件 --------------------
set(PROJECT_SOURCES
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui

    StartScreen.h
    Startscreen.cpp

    GameScreen.cpp
    GameScreen.h

    # 游戏逻辑相关
    GameMap.h
    GameMap.cpp
    GameLogic.h
    GameLogic.cpp
    GamePlayer.h
    GamePlayer.cpp
    GameMenu.h
    GameMenu.cpp
    GamePreset.h
    GamePreset.cpp

    assets.qrc
)

# -------------------- 主程序可执行文件 --------------------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(QLink
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(QLink SHARED ${PROJECT_SOURCES})
    else()
        add_executable(QLink ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(QLink PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.QLink)
endif()

set_target_properties(QLink PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(QLink)
endif()

install(TARGETS QLink
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)



# -------------------- 单元测试目标 --------------------
# 测试类：Test
# 测试文件：test.cpp / test.h
add_executable(TestGameLogic
    test.cpp
    test.h
    GameMap.cpp
    GameMap.h
    GameLogic.cpp
    GameLogic.h
    GamePlayer.cpp
    GamePlayer.h
    GamePreset.cpp
    GamePreset.h
)

target_link_libraries(TestGameLogic PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Test)

set_target_properties(TestGameLogic PROPERTIES AUTOMOC ON AUTOUIC ON AUTORCC ON)

# ctest 注册
enable_testing()
add_test(NAME GameLogicTest COMMAND TestGameLogic)
