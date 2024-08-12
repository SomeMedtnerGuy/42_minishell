/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:02 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/12 16:00:02 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	save_cwd(char *cwd, size_t size)
{
	if (getcwd(cwd, size) == NULL)
	{
		perror("getcwd");
		return (errno);
	}
	return (0);
}

static int	verify_change_dir(char *dir)
{
	if (dir == NULL)
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	else if (chdir(dir) != 0)
	{
		perror("cd");
		return (errno);
	}
	return (0);
}

int	ft_cd(char **argv, char ***envp)
{
	char	cwd[1024];
	char	*new_dir;

	if (argv[1] && argv[2])
		return (ft_print_error("cd: too many arguments"), 1);
	if (save_cwd(cwd, sizeof(cwd)) != 0)
		return (-1);
	if (!argv[1])
	{
		new_dir = get_env_value("HOME", *envp);
		if (new_dir == NULL)
			return (ft_print_error("cd: HOME not set"), 1);
		verify_change_dir(new_dir);
		free(new_dir);
	}
	else if (argv[1][0] == '\0')
		return (0);
	else
	{
		new_dir = argv[1];
		verify_change_dir(new_dir);
	}
	update_env("OLDPWD", cwd, envp);
	save_cwd(cwd, sizeof(cwd));
	update_env("PWD", cwd, envp);
	return (0);
}


