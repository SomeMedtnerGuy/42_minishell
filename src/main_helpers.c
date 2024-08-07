/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 13:49:36 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/05 13:50:20 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	is_all_spaces(char *line)
{
	while (*line)
	{
		if (!ft_strchr(SPACES, *line))
			return (0);
		line += 1;
	}
	return (1);
}

int	get_line(t_root *r)
{
	setget_signo(SET, 0);
	r->line = readline(PROMPT);
	if (setget_signo(GET, 0) == CTRLC_SIGNO)
		r->exit_code = 128 + CTRLC_SIGNO;
	if (!r->line)
	{
		ft_putstr_fd(CTRD_EXIT_MSG, STDERR_FILENO);
		free_exit(r, r->exit_code);
	}
	if (is_all_spaces(r->line))
	{
		free(r->line);
		return (1);
	}
	return (0);
}
