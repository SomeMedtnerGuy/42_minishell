/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:28:37 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/05 21:33:30 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	get_redir_mode(char type)
{
	if (type == '<' || type == '-')
		return (O_RDONLY);
	else if (type == '>')
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else
		return (O_WRONLY | O_CREAT | O_APPEND);
}

int	get_redir_fd(char type)
{
	if (type == '<' || type == '-')
		return (0);
	else
		return (1);
}

void	close_pipe(int *p)
{
	close(p[0]);
	close(p[1]);
}

void	close_pipe_and_exit(int *p, t_root *r, char *msg)
{
	close_pipe(p);
	exit_from_te(r, msg, errno);
}

void	try_running_builtin(t_exec *node, t_root *r)
{
	int	builtin_status;

	if (get_builtin(node->argv->content))
	{
		if (ft_strncmp(node->argv->content, "exit", 5) == 0)
			exit_from_te(r, NULL, r->prev_exit_code);
		builtin_status = run_builtin(node->argv, &r->envp);
		free_tree(r->tree);
		free_exit(r, builtin_status);
	}
}
