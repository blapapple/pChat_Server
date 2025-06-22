cmake_minimum_required(VERSION 3.12)
project(ChatServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置 JSON-CPP 路径（手动指定）
set(JSONCPP_INCLUDE_DIR "/usr/local/include/jsoncpp")
set(JSONCPP_LIBRARY "jsoncpp")

# 设置 MySQL Connector/C++ 路径
set(MYSQL_CONNECTOR_CPP_INCLUDE_DIR "/usr/include/mysql-cppconn-8")
set(MYSQL_CONNECTOR_CPP_LIBRARY "mysqlcppconn")

# 验证 MySQL Connector/C++ 头文件
if(EXISTS "${MYSQL_CONNECTOR_CPP_INCLUDE_DIR}/jdbc/cppconn/exception.h")
    message(STATUS "Found MySQL Connector/C++ headers: ${MYSQL_CONNECTOR_CPP_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "MySQL Connector/C++ headers not found. Install with: sudo apt install libmysqlcppconn-dev")
endif()

# 验证 JSON-CPP 头文件
if(EXISTS "${JSONCPP_INCLUDE_DIR}/json/json.h")
    message(STATUS "Found JSON-CPP headers: ${JSONCPP_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "JSON-CPP headers not found at ${JSONCPP_INCLUDE_DIR}. "
        "Install with: sudo apt install libjsoncpp-dev")
endif()

# 查找必要的依赖项
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# 查找 hiredis
find_package(PkgConfig REQUIRED)
pkg_check_modules(HIREDIS REQUIRED hiredis)
if(HIREDIS_FOUND)
    message(STATUS "Found hiredis: ${HIREDIS_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "hiredis not found - please install libhiredis-dev")
endif()

# 查找 Boost (系统安装的版本)
find_package(Boost 1.84 REQUIRED COMPONENTS system filesystem)
message(STATUS "Found Boost: ${Boost_VERSION}")

# 源文件列表
set(SOURCES
    AsioIOServicePool.cpp
    ChatGrpcClient.cpp
    ChatServer.cpp
    ChatServiceImpl.cpp
    ConfigMgr.cpp
    CServer.cpp
    CSession.cpp
    LogicSystem.cpp
    MsgNode.cpp
    MysqlDao.cpp
    MysqlMgr.cpp
    RedisMgr.cpp
    StatusGrpcClient.cpp
    UserInfoMgr.cpp
    UserMgr.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

# 头文件列表
set(HEADERS
    AsioIOServicePool.h
    ChatGrpcClient.h
    ChatServiceImpl.h
    ConfigMgr.h
    const.h
    CServer.h
    CSession.h
    data.h
    LogicSystem.h
    MsgNode.h
    MysqlDao.h
    MysqlMgr.h
    RedisMgr.h
    Singleton.h
    StatusGrpcClient.h
    UserInfoMgr.h
    UserMgr.h
    message.grpc.pb.h
    message.pb.h
)

# 包含目录（关键修改）
include_directories(
    ${MYSQL_CONNECTOR_CPP_INCLUDE_DIR}  # 必须放在最前面
    ${JSONCPP_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Protobuf_INCLUDE_DIRS}
    ${gRPC_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${HIREDIS_INCLUDE_DIRS}
)

# 可执行文件
add_executable(ChatServer ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(ChatServer
    ${MYSQL_CONNECTOR_CPP_LIBRARY}  # MySQL Connector/C++ 库
    ${JSONCPP_LIBRARY}
    ${Protobuf_LIBRARIES}
    ${gRPC_LIBRARIES}
    ${gRPC_GRPCPP_LIBRARY}
    ${Boost_LIBRARIES}
    ${HIREDIS_LIBRARIES}
    Threads::Threads
    ${ZLIB_LIBRARIES}
    pthread
    dl
)

# 安装配置文件
install(FILES config.ini DESTINATION ${CMAKE_INSTALL_PREFIX}/config)

# 设置调试和发布配置的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wextra -O3 -DNDEBUG")

# 安装目标
install(TARGETS ChatServer DESTINATION bin)