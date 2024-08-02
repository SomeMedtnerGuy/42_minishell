/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:02 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/02 13:54:53 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(char **argv, char ***envp)
{
	char	cwd[1024];
	char	*new_dir;

	if (argv[1] && argv[2])
		return (printf("cd: too many arguments\n"), 1);
	verify_getcwd(cwd, sizeof(cwd));
	if (argv[1] == NULL)
	{
		new_dir = get_env_value("HOME", *envp);
		if (new_dir == NULL)
			return (printf("cd: HOME not set\n"), 1);
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
	verify_getcwd(cwd, sizeof(cwd));
	update_env("PWD", cwd, envp);
	return (0);
}

int	verify_getcwd(char *cwd, size_t size)
{
	if (getcwd(cwd, size) == NULL)
	{
		perror("getcwd");
		return (errno);
	}
	return (0);
}

int	verify_change_dir(char *dir)
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
