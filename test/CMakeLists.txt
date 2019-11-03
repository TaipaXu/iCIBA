find_package(Qt5Core)

add_executable(
    ici-cli
    main.cpp
)

target_link_libraries(ici-cli Qt5::Core)
