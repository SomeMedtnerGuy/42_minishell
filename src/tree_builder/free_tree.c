/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 16:23:08 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/28 17:31:07 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_exec(t_exec *node)
{
	t_redir	*ptr;
	t_redir	*tmp;

	ft_lstclear(&node->argv, free);
	ptr = node->redirs;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;
		free(tmp->file);
		free(tmp);
	}
	free(node);
}

static void	free_pipe(t_pipe *node)
{
	if (!node)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	free(node);
}

void	free_tree(t_node *node)
{
	if (!node)
		return ;
	if (node->type == PIPE)
		free_pipe((t_pipe *)node);
	else if (node->type == EXEC)
		free_exec((t_exec *)node);
}
