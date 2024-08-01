/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 13:28:21 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/30 09:42:20 by ndo-vale         ###   ########.fr       */
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
		execute_node(r->tree, r);
		free_tree(r->tree);
		free_exit(r, 0);
	}
	free_tree(r->tree);
	wait(&cpstatus);
	if (WIFEXITED(cpstatus))
		r->exit_code = WEXITSTATUS(cpstatus);
	else
		r->exit_code = WTERMSIG(cpstatus);
}

static void	ft_readline_loop(t_root *r)
{
	get_line(r);
	if (handle_syntax(r->line) != 0)
	{
		r->exit_code = 2;
		free(r->line);
		return ;
	}
	tokenize_line(r); //TODO: ambiguous redirect is not working
	if (r->exit_code != 0 || !r->token_lst)
		return ;
	if (build_tree(r) != 0)
		return ;
	if (r->tree->type == EXEC && ((t_exec *)r->tree)->argv
		&& get_builtin(((t_exec *)r->tree)->argv->content))
	{
		r->exit_code = run_builtin(((t_exec *)r->tree)->argv, &r->envp);
		free_tree(r->tree);
	}
	else
		run_pipeline(r);
	if (close_temps() != 0)
		free_exit(r, errno);
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
	set_signals();
	while (1)
		ft_readline_loop(&r);
	return (0);
}
