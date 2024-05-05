set(SOURCES
    Server.cpp
)

add_executable(server ${SOURCES})

target_link_libraries(server PRIVATE ${OpenCV_LIBS})
target_link_libraries(server PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Widgets)

