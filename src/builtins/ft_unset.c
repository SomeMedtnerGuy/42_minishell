/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:21:46 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/17 15:56:51 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_unset(char **argv, char ***envp)
{
	int	i;
	char	*print_error;

	if (argv[1] && is_option(argv[1]))
	{
		print_error = ft_strjoin_free(ft_strjoin("unset: ", argv[1]), ft_strdup(": invalid option"));
		if (!print_error)
			return (errno);
		ft_print_error(print_error);
		free(print_error);
		return (INVALID_OPTION_CODE);
	}
	i = 0;
	while (argv[++i])
		delete_var(argv[i], *envp);
	return (0);
}
