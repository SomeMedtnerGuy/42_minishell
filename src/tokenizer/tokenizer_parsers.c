/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_parsers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 19:03:29 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/27 10:34:25 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_quotes(t_tokenizer_data *td, t_root *r, char c)
{
	int	i;

	td->ptr += 1;
	i = -1;
	while (td->ptr[++i] != c)
	{
		if (c == '\"' && td->ptr[i] == '$')
		{
			if (i != 0 && update_token(&td->stoken,
					ft_strldup(td->ptr, i)) != 0)
				exit_from_tokenizer(td, r, "malloc", errno);
			td->ptr += i;
			expand_cmd_env(td, r);
			i = -1;
		}
	}
	if (update_token(&td->stoken, ft_strldup(td->ptr, i)) != 0)
		exit_from_tokenizer(td, r, "malloc", errno);
	td->ptr += i + 1;
}

static void	parse_redirs(t_tokenizer_data *td)
{
	if (*td->ptr == '<')
	{
		td->ptr += 1;
		if (*td->ptr == '<')
		{
			td->type = '-';
			td->ptr += 1;
		}
		else
			td->type = '<';
	}
	else
	{
		td->ptr += 1;
		if (*td->ptr == '>')
		{
			td->type = '+';
			td->ptr += 1;
		}
		else
			td->type = '>';
	}
}

void	parse_redirs_pipes(t_tokenizer_data *td, t_root *r)
{
	if (td->stoken || td->type != 'a')
	{
		if (token_createadd(&td->token_lst, td->type, td->stoken) != 0)
			exit_from_tokenizer(td, r, "malloc", errno);
		td->type = 'a';
		td->stoken = NULL;
	}
	if (*td->ptr == '|')
	{
		if (token_createadd(&td->token_lst, '|', NULL) != 0)
			exit_from_tokenizer(td, r, "malloc", errno);
		td->ptr += 1;
	}
	else
		parse_redirs(td);
	while (ft_strchr(SPACES, *td->ptr) && *td->ptr != '\0')
		td->ptr += 1;
}

void	parse_spaces(t_tokenizer_data *td, t_root *r)
{
	if (td->stoken)
	{
		if (token_createadd(&td->token_lst, td->type, td->stoken) != 0)
			exit_from_tokenizer(td, r, "malloc", errno);
		td->type = 'a';
		td->stoken = NULL;
	}
	td->ptr += 1;
}
