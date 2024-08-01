/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:02 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/28 10:20:24 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int     ft_cd(char **argv, char ***envp)
{
	char	*cwd;

	(void)argv;
	(void)envp;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (errno);
        printf("implementation of builtin 'cd' missing.");
        return (0);
}
