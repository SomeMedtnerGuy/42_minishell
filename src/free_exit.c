/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:22:59 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/24 17:08:16 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	close_temps(char *tempfiles_dir)
{
	DIR	*tempdir;
	struct dirent	*file;
	char	*filename;
	
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

void	free_exit(t_root *r, int exit_code)
{
	ft_matrix_free((void ***)&r->envp);
	exit(exit_code);
}
