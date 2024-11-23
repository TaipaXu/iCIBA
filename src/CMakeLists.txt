find_package(Boost REQUIRED COMPONENTS program_options json)

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
)
add_executable(${TARGET} ${SOURCES})

target_link_libraries(${TARGET} PRIVATE Boost::program_options Boost::json)
