/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value_hd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 09:15:54 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/31 11:04:28 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_gnl(int fd)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

static char	*search_and_return_value(char *key, int len)
{
	int		fd;
	char	*line;
	char	*value;

	fd = open(ENVP_FILENAME, O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	while (line)
	{
		if (ft_strncmp(key, line, len) == 0 && (line)[len] == '=')
		{
			value = ft_strdup(line + len + 1);
			value[ft_strlen(value) - 1] = '\0';
			return (close_gnl(fd), free(line), free(key), value);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	free(key);
	return (NULL);
}

char	*get_env_value_hd(char *start)
{
	char	*key;
	int		len;

	key = get_env_key(start);
	if (!key)
		return (NULL);
	if (*key == '\0')
		return (free(key), ft_strdup("$"));
	len = ft_strlen(key);
	return (search_and_return_value(key, len));
}
