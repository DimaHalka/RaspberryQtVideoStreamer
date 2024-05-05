set(SOURCES
    Client.cpp
)

add_executable(client ${SOURCES})

target_link_libraries(client PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Widgets)

