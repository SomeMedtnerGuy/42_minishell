/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:25:26 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/25 19:29:01 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_env_key_len(char *start)
{
	int	count;

	count = 0;
	while (ft_isalnum(start[count]) || start[count] == '_')
		count++;
	return (count);
}

char	*get_env_key(char *start)
{
	int	count;

	if (!start)
		return (NULL);
	if (*start == '?')
		return (ft_strdup("?"));
	count = get_env_key_len(start);
	if (!count)
		return (ft_strdup(""));
	return (ft_strldup(start, count));
}

char	*get_env_value(char *start, char **envp)
{
	char	*key;
	int		len;
	char	**ptr;

	key = get_env_key(start);
	if (!key)
		return (NULL);
	if (*key == '\0')
		return (free(key), ft_strdup("$"));
	if (!envp)
		return (free(key), NULL);
	
	len = ft_strlen(key);
	ptr = envp;
	while (*ptr)
	{
		if (ft_strncmp(key, *ptr, len) == 0 && (*ptr)[len] == '=')
			return (free(key), ft_strdup(*ptr + len + 1));
		ptr += 1;
	}
	free(key);
	return (NULL);
}