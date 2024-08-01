/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:22:59 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/24 17:08:16 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_exit(t_root *r, int exit_code)
{
	ft_matrix_free((void ***)&r->envp);
	free(r->cwd); //TODO: Hopefully not necessary
	exit(exit_code);
}
