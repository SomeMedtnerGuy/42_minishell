/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:39 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/19 12:17:41 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(char **argv, char ***envp)
{
	char	*cwd;

	(void)envp;
	if (argv[1] && is_option(argv[1]))
	{
		char	*print_error;
		print_error = ft_strjoin_free(ft_strjoin("pwd: ", argv[1]), ft_strdup(": invalid option"));
		if (!print_error)
			return (errno);
		ft_print_error(print_error);
		free(print_error);
		return (INVALID_OPTION_CODE);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (errno);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}
