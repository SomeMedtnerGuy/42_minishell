/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:22:49 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/01 16:09:38 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_exit(char **argv, char ***envp)
{
	ft_matrix_free((void ***)envp);
	ft_matrix_free((void ***)&argv);
	exit(0);
}
