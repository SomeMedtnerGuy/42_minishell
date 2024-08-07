/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_extras.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 10:09:15 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/06 10:11:28 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	apply_pipe_and_execute(t_node *node, t_root *r, int *p,
		int multipurp_fd)
{
	if (dup2(p[multipurp_fd], multipurp_fd) == -1)
	{
		close(p[0]);
		close(p[1]);
		exit_from_te(r, "dup", errno);
	}
	close(p[0]);
	close(p[1]);
	execute_node(node, r);
}

void	failed_execve_aftermath(char *cmd_path, char **args, t_root *r)
{
	int	status;
	char	*cmd;

	cmd = ft_strdup(args[0]);
	if (!cmd)
	{
		free(cmd_path);
		exit_from_te(r, "malloc", errno);
	}
	status = errno;
	ft_matrix_free((void ***)&args);
	free(cmd_path);
	if (status == 2)
	{
		print_buffered_error(cmd, CMD_NOT_FOUND_MSG);
		exit_from_te(r, NULL, 127);
	}
	else if (status == 13)
	{
		print_buffered_error(cmd, PERMISSION_DENIED_MSG);
		exit_from_te(r, NULL, 126);
	}
	exit_from_te(r, NULL, 1);
}
