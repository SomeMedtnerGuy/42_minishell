/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 18:05:51 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/27 10:22:27 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	update_token(char **token, char *start)
{
	if (!start)
		return (-1);
	if (!(*token))
		*token = start;
	else
	{
		*token = ft_strjoin_free(*token, start);
		if (!(*token))
			return (-1);
	}
	return (0);
}

static int	tokenize_env(t_tokenizer_data *td, char *env_value)
{
	int	i;

	i = -1;
	while (env_value[++i])
	{
		if (ft_strchr(SPACES, env_value[i]))
		{
			if (i != 0 && update_token(&td->stoken,
					ft_strldup(env_value, i)) != 0)
				return (-1);
			if (token_createadd(&td->token_lst,
					td->type, td->stoken) != 0)
				return (-1);
			td->stoken = NULL;
			env_value += i + 1;
			i = -1;
		}
	}
	if (i != 0 && update_token(&td->stoken,
			ft_strldup(env_value, i)) != 0)
		return (-1);
	return (0);
}

char	*expand_exit_code(t_tokenizer_data *td, t_root *r)
{
	char	*code;

	code = ft_itoa(r->prev_exit_code);
	if (!code)
		return (NULL);
	td->ptr += 1;
	return (code);
}

void	expand_cmd_env(t_tokenizer_data *td, t_root *r)
{
	char	*env_value;

	td->ptr += 1;
	if (*(td->ptr) == '?')
		env_value = expand_exit_code(td, r);
	else
	{
		env_value = get_env_value(td->ptr, r->envp);
		td->ptr += get_env_key_len(td->ptr);
	}
	if (errno)
		exit_from_tokenizer(td, r, "malloc", errno);
	if (env_value)
	{
		if (tokenize_env(td, env_value) != 0)
		{
			free(env_value);
			exit_from_tokenizer(td, r, "malloc", errno);
		}
		free(env_value);
	}
}
