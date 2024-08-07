/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 14:43:14 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/29 14:19:13 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_redirs(t_redir *node, t_root *r)
{
	int	fd;

	while (node)
	{
		if (!node->file)
		{
			ft_printf("ambiguous redirect\n");
			free_tree(r->tree);
			free_exit(r, 1);
		}
		fd = open(node->file, get_redir_mode(node->redir_type), 0644);
		if (fd == -1)
			exit_from_te(r, node->file, errno);
		if (dup2(fd, get_redir_fd(node->redir_type)) < 0)
		{
			close(fd);
			exit_from_te(r, "dup", errno);
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
		exit_from_te(r, NULL, 0);
	try_running_builtin(node, r);
	args = create_args(node->argv);
	if (!args)
		exit_from_te(r, "malloc", errno);
	cmd_path = validate_cmd(args[0], r->envp);
	if (!cmd_path)
	{
		ft_matrix_free((void ***)&args);
		exit_from_te(r, "echo", errno);
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
		exit_from_te(r, "pipe", errno);
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
	free_tree(r->tree);
	waitpid(cpid_l, &status, 0);
	waitpid(cpid_r, &status, 0);
	if (WIFEXITED(status))
		free_exit(r, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		free_exit(r, 128 + WTERMSIG(status));
}

void	execute_node(t_node *node, t_root *r)
{
	if (node->type == PIPE)
		execute_pipe((t_pipe *)node, r);
	else
		execute_exec((t_exec *)node, r);
}
