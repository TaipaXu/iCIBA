find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Network)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5LinguistTools)

set(QAPPLICATION_CLASS QApplication CACHE STRING "Inheritance class for SingleApplication")
add_subdirectory(../../3rd/SingleApplication SingleApplication)

qt5_create_translation(QM_FILES . ../utils/baseprint.cpp ../utils/htmlprint.cpp widgets/historydialog.cpp "i18n/iciba_zh_CN.ts")

add_executable(
    iciba-gui
    main.cpp
    core.cpp
    ../errors/systemerror.h
    ../network.cpp
    ../utils/path.cpp
    ../utils/autostartup.cpp
    ../utils/baseprint.cpp
    ../utils/htmlprint.cpp
    ../persistence/settings.cpp
    ../persistence/history.cpp
    widgets/mainwindow.cpp
    widgets/translateword.cpp
    widgets/translatesentence.cpp
    widgets/tray.cpp
    widgets/settingsdialog.cpp
    widgets/historydialog.cpp
    assets.qrc
    ${QM_FILES}
)

target_link_libraries(iciba-gui Qt5::Core Qt5::Widgets Qt5::Network Qt5::Multimedia SingleApplication)
