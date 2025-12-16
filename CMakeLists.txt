cmake_minimum_required(VERSION 3.7.2)
project(physics)

set(CMAKE_CXX_STANDARD 17)

set_property(DIRECTORY . PROPERTY CMAKE_CONFIGURE_DEPENDS main.cpp)

if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-list --all --count

        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_COUNT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

else()
    set(GIT_COMMIT_COUNT "0")
endif()

add_definitions(-DVERSION_PROJECT=${GIT_COMMIT_COUNT})

# Пытаемся найти предустановленный SFML
find_package(SFML COMPONENTS graphics)

if (SFML_FOUND)
    message(STATUS "The SFML installed in the system will be used")
else ()
    # Если SFML не найден - выкачиваем и собираем из исходников
    message(STATUS "Get SFML from the source code")
    include(FetchContent)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    # Обход потенциальных проблем при сборке sfml-graphics
    # (stb_image.h: 'argument': conversion from 'int' to 'short')
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
endif ()

# К сожалению, вплоть до C++20 https://en.cppreference.com/w/cpp/numeric/constants
# нет удобного кроссплатформенного способа получить число pi.
# define ниже нужен для включения M_PI
if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

add_executable(physics
        main.cpp World.cpp Ball.cpp Physics.cpp Dust.cpp Color.cpp Painter.cpp
        black_box/Application.cpp black_box/View.cpp black_box/PainterImpl.cpp)
target_link_libraries(physics PRIVATE sfml-graphics)

target_compile_options(physics PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

set(CPACK_PACKAGE_NAME "lab5")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

if(WIN32)
    set(CPACK_GENERATOR "ZIP")
endif()

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kiegechev Pavel <kiegechev09@gmail.com>")
endif()

install(TARGETS physics DESTINATION bin)

include(CPack)