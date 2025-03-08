/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse_map_utils.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: crmanzan <marvin@42.fr>					+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/03/04 21:14:30 by crmanzan		   #+#	  #+#			  */
/*	 Updated: 2025/03/04 21:14:30 by crmanzan		  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "../inc/cub3d_bonus.h"

char	*pad_line_to_width(const char *line, int width)
{
	char	*trimmed;
	char	*padded;
	int		i;

	trimmed = ft_strtrim(line, "\n");
	if (!trimmed)
		exit_error("Error:\nMemory allocation for trimmed line failed.\n");
	padded = malloc(width + 1);
	if (!padded)
	{
		free(trimmed);
		exit_error("Error:\nMemory allocation for padded line failed.\n");
	}
	i = 0;
	while (trimmed[i] && i < width)
	{
		padded[i] = trimmed[i];
		i++;
	}
	while (i < width)
		padded[i++] = ' ';
	padded[width] = '\0';
	free(trimmed);
	return (padded);
}

void	parse_map_errors(int num_players, t_map **scene)
{
	if (num_players == 0)
	{
		free_scene(scene);
		exit_error("Error:\nNo player position found.\n");
	}
	if (num_players > 1)
	{
		free_scene(scene);
		exit_error("Error:\nMultiple player positions found.\n");
	}
}

static int	errors_map_chars(t_map *scene, int i, int j)
{
	char	*trimmed;

	if (is_strspace(scene->lines[i]))
	{
		free_scene(&scene);
		printf("\033[31mError:\nEmpty lines not allowed in map.\n\033[0m");
		return (0);
	}
	if (!is_notvalid(scene->lines[i]))
	{
		trimmed = ft_strtrim(scene->lines[i], "\n");
		free_scene(&scene);
		printf("\033[31mError:\nInvalid line [%s] in map.\n\033[0m", trimmed);
		free(trimmed);
		return (0);
	}
	if (!ft_strchr(VALID_MAP_CHARS, scene->lines[i][j]))
	{
		printf("\033[31mError:\nInvalid character \
[%c] in map.\n\033[0m", scene->lines[i][j]);
		free_scene(&scene);
		return (0);
	}
	return (1);
}

int	set_map_chars(t_map *scene, int i, int j, int *num_players)
{
	if (ft_strchr("NSEW", scene->lines[i][j]))
	{
		scene->player_x = j;
		scene->player_y = i - scene->start;
		scene->player_c = scene->lines[i][j];
		(*num_players)++;
	}
	return (errors_map_chars(scene, i, j));
}

char	**init_allocate_map(int height, int width)
{
	char	**map;
	int		i;

	map = malloc(sizeof(char *) * (height + 1));
	if (!map)
		exit_error("Error:\nMemory allocation for map failed.\n");
	i = 0;
	while (i < height)
	{
		map[i] = malloc(sizeof(char) * (width + 1));
		if (!map[i])
		{
			free_map(map);
			exit_error("Error:\nMemory allocation for map row failed.\n");
		}
		ft_bzero(map[i], width);
		map[i][width] = '\0';
		i++;
	}
	map[height] = NULL;
	return (map);
}
