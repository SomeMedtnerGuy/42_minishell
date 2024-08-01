/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/29 16:31:04 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/29 18:25:29 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	print_syntax_error(char c)
{
	ft_putstr_fd(SYNTAX_ERROR, 2);
	ft_putchar_fd(c, 2);
	ft_putstr_fd("\'\n", 2);
}

static void	handle_quotes(t_flags *f, char q)
{
	if (q == '\"' && !f->sq)
	{
		f->important = 0;
		f->dq = (f->dq + 1) % 2;
	}
	else if (q == '\'' && !f->dq)
	{
		f->important = 0;
		f->sq = (f->sq + 1) % 2;
	}
}

static void	handle_pipe_redirs(t_flags *f, char **ptr)
{
	if (f->start && **ptr == '|')
	{
		print_syntax_error(**ptr);
		f->error = 1;
	}
	if (f->important)
	{
		print_syntax_error(**ptr);
		f->error = 1;
	}
	if ((**ptr == '>' && *(*ptr + 1) == '>')
		|| (**ptr == '<' && *(*ptr + 1) == '<'))
		*ptr += 1;
	f->important = 1;
}

void	init_flags(t_flags *f)
{
	f->sq = 0;
	f->dq = 0;
	f->important = 0;
	f->start = 1;
	f->error = 0;
}

int	handle_syntax(char *ptr)
{
	t_flags	f;

	init_flags(&f);
	while (*ptr)
	{
		if (ft_strchr(SPACES, *ptr))
			;
		else if (*ptr == '\"' || *ptr == '\'')
			handle_quotes(&f, *ptr);
		else if (*ptr == '>' || *ptr == '<' || *ptr == '|')
			handle_pipe_redirs(&f, &ptr);
		else
			f.important = 0;
		if (f.start)
			f.start = 0;
		if (f.error)
			return (1);
		ptr += 1;
	}
	if (f.important || f.sq || f.dq)
		return (ft_putstr_fd(SYNTAX_ERROR_END, 2), 1);
	return (0);
}
