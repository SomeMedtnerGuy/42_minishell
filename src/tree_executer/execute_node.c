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

void	free_from_te(t_root *r, char *msg, int exit_code)
{
	perror(msg);
	free_tree(r->tree);
	free_exit(r, exit_code);
}

char	**create_args(t_list *argv)
{
	char	**args;
	int		i;

	args = (char **)ft_calloc(ft_lstsize(argv) + 1, sizeof(char *));
	if (!args)
		return (NULL);
	i = -1;
	while (argv)
	{
		args[++i] = ft_strdup((char *)argv->content);
		if (!args[i])
			return (ft_matrix_free((void ***)&args), NULL);
		argv = argv->next;
	}
	return (args);
}

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

void	execute_redirs(t_redir *node, t_root *r)
{
	int	fd;

	while (node)
	{
		if (!node->file)
		{
			
			//TODO: print ambiguous. Somehow should not continue running. 
			// Maybe even exit from here? with exit code 1.
		}
		fd = open(node->file, get_redir_mode(node->redir_type), 0644);
		if (fd == -1)
			free_from_te(r, node->file, errno);
		if (dup2(fd, get_redir_fd(node->redir_type)) < 0)
		{
			close(fd);
			free_from_te(r, "dup", errno);
		}
		close(fd);
		node = node->next;
	}
}

void	execute_exec(t_exec *node, t_root *r)
{
	char	**args;
	char	*cmd_path;
	int	builtin_status;

	execute_redirs(node->redirs, r);
	if (!node->argv)
	{
		free_tree(r->tree);
		free_exit(r, 0);
	}
	if (get_builtin(node->argv->content))
	{
		//TODO: make exit work
		//idea: 
		//dup node->argv
		//free_tree
		//run builtin with dup
		//exit will free dup and envp
		builtin_status = run_builtin(node->argv, &r->envp);
		free_tree(r->tree);
		free_exit(r, builtin_status);
	}
	args = create_args(node->argv);
	if (!args)
		free_from_te(r, "malloc", errno);
	cmd_path = validate_cmd(args[0], r->envp);
	if (!cmd_path)
	{
		ft_matrix_free((void ***)&args);
		free_from_te(r, "echo", errno);
	}
	execve(cmd_path, args, r->envp);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	ft_matrix_free((void ***)&args);
	free(cmd_path);
	free_tree(r->tree);
	free_exit(r, 127);
}

void	execute_pipe(t_pipe *node, t_root *r)
{
	int	p[2];
	pid_t	cpid_l;
	pid_t	cpid_r;
	int	status;

	if (pipe(p) < 0)
		free_from_te(r, "pipe", errno);
	cpid_l = fork();
	if (cpid_l == -1)
	{
		close(p[0]);
		close(p[1]);
		free_from_te(r, "fork", errno);
	}
	else if (cpid_l == 0)
	{
		if (dup2(p[1], 1) == -1)
		{
			close(p[0]);
                	close(p[1]);
                	free_from_te(r, "dup", errno);
		}
		close(p[0]);
		close(p[1]);
		execute_node(node->left, r); // SHOULD NEVER RETURN
	}
	cpid_r = fork();
	if (cpid_r == -1)
	{
		close(p[0]);
                close(p[1]);
                free_from_te(r, "fork", errno);
	}
	else if (cpid_r == 0)
	{
		if (dup2(p[0], 0) == -1)
		{
			close(p[0]);
                        close(p[1]);
                        free_from_te(r, "dup", errno);
		}
		close(p[0]);
                close(p[1]);
		execute_node(node->right, r); // SHOULD NEVER RETURN
	}
	close(p[0]);
	close(p[1]);
	free_tree(r->tree);
	waitpid(cpid_l, &status, 0);
	waitpid(cpid_r, &status, 0);
	if (WIFEXITED(status))
		free_exit(r, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		free_exit(r, WTERMSIG(status));
}

void	execute_node(t_node *node, t_root *r)
{
	if (node->type == PIPE)
		execute_pipe((t_pipe *)node, r);
	else
		execute_exec((t_exec *)node, r);
}
