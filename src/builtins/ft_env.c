/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:22:20 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/20 00:16:47 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_env(char **argv, char ***envp)
{
	int		i;
	char	*print_error;

	i = -1;
	if (!*envp)
		return (0);
	if (is_option(argv[1]))
	{
		print_error = ft_strjoin_free(ft_strjoin("env: ", argv[1]),
				ft_strdup(": invalid option"));
		if (!print_error)
			return (errno);
		ft_print_error(print_error);
		free(print_error);
		return (125);
	}
	while ((*envp)[++i])
	{
		if (ft_strchr((*envp)[i], '='))
			ft_printf("%s\n", (*envp)[i]);
	}
	return (0);
}
