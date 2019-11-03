find_package(Qt5Core)
find_package(Qt5Network)
find_package(Qt5LinguistTools)

qt5_create_translation(QM_FILES . ../utils/baseprint.cpp ../utils/terminalprint.cpp "i18n/iciba_zh_CN.ts")

add_executable(
    iciba
    main.cpp
    core.cpp
    ../errors/systemerror.h
    ../network.cpp
    ../utils/path.cpp
    ../utils/baseprint.cpp
    ../utils/terminalprint.cpp
    ../utils/autostartup.cpp
    ../persistence/settings.cpp
    ../persistence/history.cpp
    assets.qrc
    ${QM_FILES}
)

target_link_libraries(iciba Qt5::Core Qt5::Network)
