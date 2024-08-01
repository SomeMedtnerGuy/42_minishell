/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenlst_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 19:05:25 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/24 23:54:59 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token	*tokennew(char type, char *content)
{
	t_token	*token;

	token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->content = content;
	token->next = NULL;
	return (token);
}

t_token	*tokenlast(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

void	tokenadd_back(t_token **token, t_token *new)
{
	if (!(*token))
		*token = new;
	else
		tokenlast(*token)->next = new;
}

int	token_createadd(t_token **tokenlst, char type, char *tokenstr)
{
	t_token	*new;

	new = tokennew(type, tokenstr);
	if (!new)
		return (1);
	tokenadd_back(tokenlst, new);
	return (0);
}

void	free_tokenlst(t_token *tokenlst)
{
	t_token	*tmp;

	while (tokenlst)
	{
		free(tokenlst->content);
		tmp = tokenlst;
		tokenlst = tokenlst->next;
		free(tmp);
	}
}
