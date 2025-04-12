cmake_minimum_required(VERSION 3.16)
project(SecureLinkFullProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ========== Shared Source (Common .cpp/h files) ==========
add_library(shared STATIC
    shared/data_packet.cpp
    shared/data_packet.h
    shared/base64.cpp
    shared/base64.h
)

target_include_directories(shared PUBLIC shared)

# ========== Client Executable ==========
add_executable(client
    client/client.cpp
    client/client_utils.cpp
    client/client_utils.h
    client/client_auth.cpp
    client/client_auth.h
)

target_include_directories(client PRIVATE
    ${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(client PRIVATE shared)

# ========== Server Executable ==========
add_executable(server
    server/server.cpp
    server/server_db.cpp
    server/server_db.h
    server/server_logger.cpp
    server/server_logger.h
    server/server_state.cpp
    server/server_state.h
    server/camera_handler.cpp
    server/camera_handler.h
)

target_include_directories(server PRIVATE
    ${CMAKE_SOURCE_DIR}/shared
)

target_link_libraries(server PRIVATE shared sqlite3)

# ========== SecureLink UI ==========
add_subdirectory(SecureLinkUI)
