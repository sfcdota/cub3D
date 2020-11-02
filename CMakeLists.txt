project(cub3D)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_C_STANDARD 99)
#-Wall -Wextra -Werror	#-Wall -Wextra -Werror
set(CMAKE_C_FLAGS "-D BUFFER_SIZE=128 -L../libft/ -lft  -L/usr/local/lib -lmlx -framework OpenGL -framework AppKit")
#set(CMAKE_C_FLAGS "-D BUFFER_SIZE=128 -framework OpenGL -framework AppKit")


file(GLOB srcs "*.c")
#add_executable(cub3D ${srcs} cub3D.h)
file(GLOB gnl "./get_next_line/*.c")
add_executable(cub3D ${srcs} ${gnl} cub3d.h raycasting/raycasting_main.c raycasting/draw_sprites.c raycasting/draw_walls.c raycasting/draw_utils.c raycasting/events.c init_funcs.c errors.c)
include_directories(libft/)
include_directories(get_next_line/)
include_directories(/usr/local/include)
link_directories(libft)
target_link_libraries(cub3D LINK_PUBLIC ft)
