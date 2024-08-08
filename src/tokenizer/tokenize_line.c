/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 15:47:13 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/27 10:31:45 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	init_data(t_tokenizer_data *td, t_root *r)
{
	td->ptr = r->line;
	td->token_lst = NULL;
	td->type = 'a';
	td->stoken = NULL;
}

static int	finish_tokenizer(t_tokenizer_data *td, t_root *r)
{
	if (td->stoken || td->type != 'a')
	{
		if (token_createadd(&td->token_lst, td->type,
				ft_strdup(td->stoken)) != 0)
			exit_from_tokenizer(td, r, "malloc", errno);
	}
	r->token_lst = td->token_lst;
	return (0);
}

void	tokenize_line(t_root *r)
{
	t_tokenizer_data	td;

	init_data(&td, r);
	while (*td.ptr)
	{
		if (*td.ptr == '\'' || *td.ptr == '\"')
			parse_quotes(&td, r, *td.ptr);
		else if (*td.ptr == '$' && td.type != '-')
			expand_cmd_env(&td, r);
		else if (ft_strchr(TOKEN_CHARS, *td.ptr))
			parse_redirs_pipes(&td, r);
		else if (ft_strchr(SPACES, *td.ptr))
			parse_spaces(&td, r);
		else
		{
			if (update_token(&td.stoken, ft_strldup(td.ptr, 1)) != 0)
				exit_from_tokenizer(&td, r, "malloc", errno);
			td.ptr += 1;
		}
	}
	if (finish_tokenizer(&td, r) != 0)
		return ;
	return (free(td.stoken), free(r->line));
}
