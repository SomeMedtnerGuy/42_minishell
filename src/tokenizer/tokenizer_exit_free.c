/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_exit_free.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 20:42:38 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/30 20:43:13 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tokenizer(t_tokenizer_data *td)
{
	free_tokenlst(td->token_lst);
	free(td->stoken);
}

void	exit_from_tokenizer(t_tokenizer_data *td, t_root *r,
				char *msg, int exit_code)
{
	if (errno)
		perror(msg);
	else
		ft_putstr_fd(msg, 2);
	free_tokenizer(td);
	free_exit(r, exit_code);
}
