/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 10:13:59 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/06 10:36:44 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_buffered_error(char *origin, char *msg)
{
	char	*final_error;

	final_error = ft_strjoin(origin, msg);
	if (!final_error)
	{
		perror("malloc");
		return ;
	}
	free(origin);
	if (dup2(2, 1) < 0)
	{
		perror("dup");
		return ;
	}
	printf("%s\n", final_error);
	free(final_error);
}
