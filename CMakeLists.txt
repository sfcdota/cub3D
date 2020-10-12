cmake_minimum_required(VERSION 3.12)
set(CMAKE_C_STANDARD 99)
#-Wall -Wextra -Werror
set(CMAKE_C_FLAGS "-L /usr/local/lib -lmlx -framework OpenGL -framework AppKit")
include_directories(/usr/local/include)
file(GLOB srcs "*.c")

add_executable(cub3D ${srcs} cub3D.h)
