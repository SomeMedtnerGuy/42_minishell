/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:22:20 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/28 10:22:41 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int     ft_env(char **argv, char ***envp)
{       
	int	i;

	(void)argv;
        i = -1;
        if (!*envp)
        	return (0);
        while ((*envp)[++i])
        {
        	if (ft_strchr((*envp)[i], '='))
        		ft_printf("%s\n", (*envp)[i]);
        }
        return (0);
}
