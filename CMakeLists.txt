cmake_minimum_required(VERSION 3.16)
project(Photo_Master VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Поиск Qt6 (теперь с PrintSupport)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql PrintSupport)

# Поиск PostgreSQL
find_package(PostgreSQL REQUIRED)

# Исходники
set(SOURCES
    main.cpp
    databasehelper.cpp
    loginform.cpp
    mainwindow.cpp
    clientsform.cpp
    executorsform.cpp
    servicesform.cpp
    ordersform.cpp
    helpform.cpp
)

set(HEADERS
    databasehelper.h
    loginform.h
    mainwindow.h
    clientsform.h
    executorsform.h
    servicesform.h
    ordersform.h
    helpform.h
)

add_executable(Photo_Master ${SOURCES} ${HEADERS})

target_link_libraries(Photo_Master PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
    PostgreSQL::PostgreSQL
)

if(WIN32)
    target_link_libraries(Photo_Master PRIVATE ${PostgreSQL_LIBRARIES})
endif()