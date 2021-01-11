cmake_minimum_required(VERSION 3.1)
project(parade)


# Find SDL2 
#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules")
find_package(SDL2 REQUIRED)

find_library(_SDL2TTF_LIB SDL2_ttf)
list(APPEND SDL2_LIBRARIES ${_SDL2TTF_LIB})
find_library(_SDL2IMAGE_LIB SDL2_image)
list(APPEND SDL2_LIBRARIES ${_SDL2IMAGE_LIB})
find_library(_SDL2MIXER_LIB SDL2_mixer)
list(APPEND SDL2_LIBRARIES ${_SDL2MIXER_LIB})

include_directories(${SDL2_INCLUDE_DIRS})
find_package(Boost REQUIRED COMPONENTS filesystem)


# Define executable target
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2main_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})
add_executable(parade main.cpp 
CAnimate.cpp
CRectangle.cpp
CText.cpp
CTexture.cpp)

target_link_libraries(parade ${SDL2_LIBRARIES} Boost::filesystem)

install(TARGETS parade DESTINATION /home/pi/bmos/parade)
install(DIRECTORY fonts DESTINATION /home/pi/bmos/parade)

install(DIRECTORY scripts DESTINATION /home/pi/bmos
        PATTERN "scripts/*"
        PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ
                    GROUP_EXECUTE GROUP_READ)
