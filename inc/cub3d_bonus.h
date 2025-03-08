/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martalop <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:58:57 by martalop          #+#    #+#             */
/*   Updated: 2025/03/08 17:30:48 by crmanzan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_BONUS_H
# define CUB3D_BONUS_H

# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/inc/libft.h"
# include <fcntl.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>
# include <unistd.h>
# include <math.h>
# include <limits.h>
# include <sys/time.h>

# define WIN_WIDTH 1600
# define WIN_HEIGHT 1000
# define VALID_MAP_CHARS "10NSEW \n"

# define FOV 60
# define TILE 64

# define MAX_MAP_SIZE 1024
# define BUFF 4096
# define MAX_LINE_LEN 1024

// movimiento para que el minimapa para que no esté pegado a la esquina
# define MINIMAP_X_OFFSET 10
# define MINIMAP_Y_OFFSET 10

// Tamaño del minimapa (no en pixeles)
// cuanto de el mapa real puedes ver en el minimapa.
# define MINIMAP_SCALE 10.0

//Tamaño real del minimapa(en pixeles)
# define MINIMAP_SIZE 200.0

# define MINI_PLAYER_HEIGHT 8
# define MINI_PLAYER_WIDTH 8

# define PLAYER_COLOR 0xFF0000FF
# define TRANSPARENT 0x1c2d36FF
# define WALLCOLOR 0xffffff00
# define FLOORCOLOR 0xffffff00
# define RAY_COLOR 0x4A001FFF

typedef struct s_anim
{
	mlx_image_t		*player_sprites[6];
	mlx_texture_t	*player_texture[6];
	char			*sprite_paths[6];
	int				is_animating;
	double			last_frame_time;
	int				current_frame;
}	t_anim;

typedef struct s_point
{
	double	x;
	double	y;
}	t_point;

typedef struct s_math_texture
{
	double	text_pos;
	double	step;
	t_point	texture_in;
}	t_math_texture;

typedef struct s_mlx_textures
{
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*west;
	mlx_texture_t	*east;
}	t_mlx_textures;

typedef struct s_textures
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	int		floor_color[3];
	int		ceiling_color[3];
}	t_textures;

typedef struct s_ray
{
	int				first_wall_pixel;
	int				last_wall_pixel;
	double			projection_height; // amount of wall pixels
	double			distance_to_wall;
	double			angle;
	t_point			*hit_point;
	char			wall_hit;
	mlx_texture_t	*wall_texture;
}	t_ray;

typedef struct s_game
{
	double			ray_increment;
	double			distance_to_plane;
	double			direction;
	t_point			player;
	mlx_t			*mlx;
	mlx_image_t		*image;
	char			**map;
	t_ray			*ray;
	int				map_width;
	int				map_height;
	t_mlx_textures	*textures;
	uint32_t		ceiling_color;
	uint32_t		floor_color;
	t_anim			*anim;
}	t_game;

typedef struct s_map
{
	char		**lines;
	char		**map;
	int			start;
	int			end;
	int			width;
	int			height;
	int			valid_map;
	int			player_x;
	int			player_y;
	char		player_c;
	t_textures	textures;
}	t_map;

// INIT
void			init_player(char player_c, int player_x, int player_y,
					t_game *info);
int				init_mlx(t_game *info);
int				init_textures(t_game *info, t_textures *textures);
t_anim			*init_anim(void);

// PARSING
int				main_checker(int argc, char *argv[], t_map **map);
int				cub_control(char *argv);
void			parse_scene_file(const char *filename, t_map *scene);
int				is_strspace(char *str);
char			*trim_whitespace(char *str);
int				is_valid_map_char(char c);
void			set_file_lines(const char *filename, t_map *scene,
					int lines_count);
int				get_lines_count(const char *filename);
void			parse_main_textures(char *line, t_map *scene, int map_started);
void			parse_map(t_map *scene);
char			**init_allocate_map(int height, int width);
int				set_map_chars(t_map *scene, int i, int j, int *num_players);
void			parse_map_errors(int num_players, t_map **scene);
char			*pad_line_to_width(const char *line, int width);
int				extract_texture_path(const char *trimmed, char *result);
void			error_invalid_identifier(char *trimmed, t_map *scene);
int				ft_strncmp_isspace(char *trimmed, char *identifier);
void			process_line(t_map *scene, int i, int *num_players,
					char **copy_map);
int				parse_color_values(char **current, int *color);

// RAYCASTING
double			degree_to_radian(double degree);
t_point			*horizontal_hit(t_point player, char **map, double angle, \
																t_game *info);
t_point			*vertical_hit(t_point player, char **map, double angle, \
																t_game *info);
double			point_distance(t_point hit, t_point player);
t_game			*init_raycasting(t_map *map);
int				cast_ray(t_game *info, char **map, t_ray *ray);
void			print_column(t_ray *ray, t_game *info, int x);
double			adjust_angle(double angle);
void			print_scene(t_game *info, char **map, t_ray *ray);
void			adjust_pixels(int *first_wall_pixel, int *last_wall_pixel);
uint32_t		get_ceiling_color(int *ceiling_color);
uint32_t		get_floor_color(int *floor_color);
mlx_texture_t	*get_wall_texture(t_ray *ray, t_mlx_textures *textures);
uint32_t		get_texture_pixel(mlx_texture_t *texture, int x, int y);
int				safe_map_point(double x, double y, int width, int height);
int				safe_hit_point(double x, double y, int width, int height);
int				is_wall(double x, double y, t_game *info);

// KEYBOARD
void			player_movements(void *param);

//MINIMAP
void			draw_minimap(mlx_image_t *image, char **map, t_game *info);
void			draw_player(t_game *info);
void			draw_minimap_rays(t_game *game);

// ANIMATION
void			update_animation(void *param);
void			free_anim(t_game *game);
t_anim			*init_anim(void);
void			load_player_sprite(t_game *game);

// UTILS
void			free_map(char **map);
void			free_scene(t_map **scene);
void			exit_error(char *msg);
void			free_game(t_game *game);
int				is_notvalid(char *str);
void			free_mlx_textures(t_mlx_textures *textures);
void			free_anim(t_game *game);
void			print_map(char **map);

#endif
