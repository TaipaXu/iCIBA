find_package(Qt6 REQUIRED COMPONENTS Core Network)

configure_file(./config.hpp.in ./config.hpp)

set(TARGET iciba)
set(SOURCES
        main.cpp
        core.hpp
        core.cpp

        models/wordresult.hpp
        models/sentenceresult.hpp

        network.hpp
        network.cpp

        utils/baseprint.hpp
        utils/baseprint.cpp
        utils/terminalprint.hpp
        utils/terminalprint.cpp

        errors/systemerror.hpp
)

add_executable(
    ${TARGET}
    ${SOURCES}
)

target_link_libraries(${TARGET} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network)
