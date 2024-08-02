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

void	get_line(t_root *r)
{
	setget_signo(SET, 0);
	r->line = readline(PROMPT);
	if (setget_signo(GET, 0) == 2)
		r->exit_code = 130;
	if (!r->line)
	{
		ft_putstr_fd(CTRD_EXIT_MSG, 1);
		free_exit(r, r->exit_code);
	}
}

void	run_pipeline(t_root *r)
{
	pid_t	cpid;
	int	cpstatus;

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
		execute_node(r->tree, r);
		free_tree(r->tree);
		free_exit(r, 0);
	}
	free_tree(r->tree);
	wait(&cpstatus);
	if (WIFEXITED(cpstatus))
		r->exit_code = WEXITSTATUS(cpstatus);
	else
		//r->exit_code = WTERMSIG(cpstatus);
	{
		if (WCOREDUMP(cpstatus))
			ft_printf("Quit (core dumped)\n");
		r->exit_code = 128 + WTERMSIG(cpstatus);
	}
}

static void	ft_readline_loop(t_root *r)
{
	get_line(r);
	add_history(r->line);
	if (handle_syntax(r->line) != 0)
	{
		r->exit_code = 2;
		free(r->line);
		return ;
	}
	tokenize_line(r);
	if (r->exit_code != 0 || !r->token_lst)
		return ;
	if (build_tree(r) != 0)
		return ;
	if (r->tree->type == EXEC && ((t_exec *)r->tree)->argv
		&& get_builtin(((t_exec *)r->tree)->argv->content))
	{
		execute_redirs(((t_exec *)r->tree)->redirs, r);
		if (ft_strncmp(((t_exec *)r->tree)->argv->content, "exit", 5) == 0)
		{
			free_tree(r->tree);
			ft_printf("exit\n");
			free_exit(r, errno);
		}
		r->exit_code = run_builtin(((t_exec *)r->tree)->argv, &r->envp);
		free_tree(r->tree);
	}
	else
	{
		set_signal_pipeline();
		run_pipeline(r);
		set_signal_default();
	}
	if (close_temps(r->tempfiles_dir) != 0)
	{
		perror("close temps");
	 	free_exit(r, errno);
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
	getcwd(r->tempfiles_dir, 1024);
	if (errno)
	{
		perror("getcwd");
		ft_matrix_free((void ***)envp);
		exit(errno);
	}
	ft_strlcat(r->tempfiles_dir, "/.tempfiles/", 1024);
	r->exit_code = 0;
	r->prev_exit_code = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_root	r;

	if (argc != 1)
		return (ft_putstr_fd(LAUNCH_ERROR, 2), 0);
	(void)argv;
	init_root(&r, envp);
	set_signal_default();
	while (1)
		ft_readline_loop(&r);
	return (0);
}
