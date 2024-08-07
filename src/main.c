/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:28:21 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/02 14:37:38 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	run_pipeline(t_root *r)
{
	pid_t	cpid;
	int		cpstatus;

	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		free_tree(r->tree);
		free_exit(r, errno);
	}
	if (cpid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, signal_handler_pipeline_childs);
		execute_node(r->tree, r);
	}
	free_tree(r->tree);
	wait(&cpstatus);
	if (WIFEXITED(cpstatus))
		r->exit_code = WEXITSTATUS(cpstatus);
	else
	{
		if (WCOREDUMP(cpstatus))
			ft_putstr_fd(CORE_DUMP_MSG, STDERR_FILENO);
		r->exit_code = 128 + WTERMSIG(cpstatus);
	}
}

static int	get_exit_code_from_arg(char *arg)
{
	int	i;
	
	i = 0;
	if (!ft_isdigit(arg[i]) && arg[i] != '-')
		return (-1);
	while (arg[++i])
	{
		if (!ft_isdigit(arg[i]))
			return (-1);
	}
	return (ft_atoi(arg) % 255);
}

void	ft_exit_parent(t_root *r, t_exec *node)
{
	int	exit_code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (node->argv->next)
	{
		exit_code = get_exit_code_from_arg(node->argv->next->content);
		if (exit_code == -1)
		{
			ft_putstr_fd("exit: numeric arguments required\n", 2);
			free_tree(r->tree);
			free_exit(r, 2);
		}
		if (node->argv->next->next)
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			r->exit_code = 1;
			return ;
		}
		else
		{
			free_tree(r->tree);
			free_exit(r, exit_code);
		}
	}
	free_tree(r->tree);
	free_exit(r, r->prev_exit_code); //TODO: Implement same in exit in pipeline
}

static void	execute_builtin_in_parent(t_root *r)
{
	t_exec	*exec_node;
	
	exec_node = (t_exec *)r->tree;
	execute_redirs(exec_node->redirs, r);
	if (ft_strncmp(exec_node->argv->content, "exit", 5) == 0)
		ft_exit_parent(r, exec_node);
	else
		r->exit_code = run_builtin(((t_exec *)r->tree)->argv, &r->envp);
	free_tree(r->tree);
}

static void	ft_readline_loop(t_root *r)
{
	if (get_line(r) != 0)
		return ;
	add_history(r->line);
	if (handle_syntax(r->line) != 0)
	{
		r->exit_code = SYNTAX_ERROR_CODE;
		return ;
	}	
	tokenize_line(r);
	if (r->exit_code != 0 || !r->token_lst)
		return ;
	if (build_tree(r) != 0)
		return ;
	if (r->tree->type == EXEC && ((t_exec *)r->tree)->argv
		&& get_builtin(((t_exec *)r->tree)->argv->content))
		execute_builtin_in_parent(r);
	else
	{
		set_signal_pipeline();
		run_pipeline(r);
		set_signal_default();
	}
}

static void	init_root(t_root *r, char **envp)
{
	r->line = NULL;
	r->envp = (char **)ft_matrix_dup((void **)envp);
	if (!r->envp)
	{
		perror("malloc");
		exit(errno);
	}
	getcwd(r->tempfiles_dir, BUFFER_MAX_SIZE);
	if (errno)
	{
		perror("getcwd");
		ft_matrix_free((void ***)envp);
		exit(errno);
	}
	ft_strlcat(r->tempfiles_dir, TEMPFILES_DIR, BUFFER_MAX_SIZE);
	r->exit_code = 0;
	r->prev_exit_code = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_root	r;

	if (argc != 1)
		return (ft_putstr_fd(LAUNCH_ERROR, STDERR_FILENO), 0);
	(void)argv;
	init_root(&r, envp);
	set_signal_default();
	while (1)
	{
		ft_readline_loop(&r);
		if (close_temps(r.tempfiles_dir) != 0)
		{
			perror("close temps");
			free_exit(&r, errno);
		}
	}
	return (0);
}
