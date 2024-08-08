/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:22:59 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/02 16:01:29 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	close_temps(char *tempfiles_dir)
{
	DIR				*tempdir;
	struct dirent	*file;
	char			*filename;

	tempdir = opendir(tempfiles_dir);
	if (!tempdir)
		return (errno);
	file = readdir(tempdir);
	if (errno)
		return (errno);
	while (file)
	{
		filename = ft_strjoin(tempfiles_dir, file->d_name);
		if (!filename)
			return (errno);
		if ((file->d_name)[0] != '.')
			unlink(filename);
		free(filename);
		errno = 0;
		file = readdir(tempdir);
		if (errno)
			return (errno);
	}
	closedir(tempdir);
	return (errno);
}

void	exit_from_te(t_root *r, char *msg, int exit_code)
{
	if (msg)
		perror(msg);
	free_tree(r->tree);
	free_exit(r, exit_code);
}

void	free_exit(t_root *r, int exit_code)
{
	ft_matrix_free((void ***)&r->envp);
	rl_clear_history();
	exit(exit_code);
}

void	clean_exit_minishell(t_root *r, int exit_code)
{
	ft_putstr_fd("exit", 2);
	free_exit(r, exit_code);
}