/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 14:43:14 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/09 12:44:40 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_redirs(t_redir *node, t_root *r)
{
	int	fd;

	while (node)
	{
		if (!node->file)
			exit_with_standard_error(r, "ambiguous redirect\n", 1, 0);
		fd = open(node->file, get_redir_mode(node->redir_type), 0644);
		if (fd == -1)
			exit_with_standard_error(r, node->file, errno, 0);
		if (dup2(fd, get_redir_fd(node->redir_type)) < 0)
		{
			close(fd);
			exit_with_standard_error(r, "dup", errno, 0);
		}
		close(fd);
		node = node->next;
	}
}

void	execute_exec(t_exec *node, t_root *r)
{
	char	**args;
	char	*cmd_path;

	execute_redirs(node->redirs, r);
	if (!node->argv)
		free_everything_exit(r, 0);
	try_running_builtin(node, r);
	args = create_args(node->argv);
	if (!args)
		exit_with_standard_error(r, "malloc", errno, 0);
	cmd_path = validate_cmd(args[0], r->envp);
	if (!cmd_path)
	{
		ft_matrix_free((void ***)&args);
		exit_with_standard_error(r, "echo", errno, 0);
	}
	execve(cmd_path, args, r->envp);
	failed_execve_aftermath(cmd_path, args, r);
}

void	execute_pipe(t_pipe *node, t_root *r)
{
	int		p[2];
	pid_t	cpid_l;
	pid_t	cpid_r;
	int		status;

	if (pipe(p) < 0)
		exit_with_standard_error(r, "pipe", errno, 0);
	cpid_l = fork();
	if (cpid_l == -1)
		close_pipe_and_exit(p, r, "fork");
	else if (cpid_l == 0)
		apply_pipe_and_execute(node->left, r, p, 1);
	cpid_r = fork();
	if (cpid_r == -1)
		close_pipe_and_exit(p, r, "fork");
	else if (cpid_r == 0)
		apply_pipe_and_execute(node->right, r, p, 0);
	close_pipe(p);
	free_tree(&r->tree);
	waitpid(cpid_l, &status, 0);
	waitpid(cpid_r, &status, 0);
	if (WIFEXITED(status))
		free_everything_exit(r, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		free_everything_exit(r, 128 + WTERMSIG(status));
	else
		exit_with_standard_error(r, "something unexpected occured\n", 1, 0);
}

void	execute_node(t_node *node, t_root *r)
{
	if (node->type == PIPE)
		execute_pipe((t_pipe *)node, r);
	else
		execute_exec((t_exec *)node, r);
}
