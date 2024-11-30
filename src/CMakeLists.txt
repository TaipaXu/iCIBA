option(USE_NLOHMANN_JSON "Use nlohmann::json instead of Boost.JSON" OFF)

find_package(Boost REQUIRED COMPONENTS program_options)

if(USE_NLOHMANN_JSON)
    find_package(nlohmann_json REQUIRED)
    set(JSON_LIB nlohmann_json::nlohmann_json)
    add_definitions(-DUSE_NLOHMANN_JSON)
    message(STATUS "Using nlohmann::json")
else()
    find_package(Boost REQUIRED COMPONENTS json QUIET)
    if(Boost_JSON_FOUND)
        set(JSON_LIB Boost::json)
        add_definitions(-DUSE_BOOST_JSON)
        message(STATUS "Using Boost.JSON")
    else()
        message(FATAL_ERROR "Boost.JSON not found and USE_NLOHMANN_JSON is OFF. Please install Boost.JSON or enable USE_NLOHMANN_JSON.")
    endif()
endif()

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

target_link_libraries(${TARGET} PRIVATE
    Boost::program_options
    ${JSON_LIB}
)
