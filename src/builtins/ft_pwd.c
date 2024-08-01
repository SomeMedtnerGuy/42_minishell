/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:39 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/28 10:20:55 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int     ft_pwd(char **argv, char ***envp)
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
