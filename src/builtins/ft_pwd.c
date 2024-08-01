/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:39 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/01 16:11:22 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_pwd(char **argv, char ***envp)
{
	char	*cwd;

	(void)argv;
	(void)envp;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (errno);
	ft_printf("%s\n", cwd);
	free(cwd);
	return (0);
}
