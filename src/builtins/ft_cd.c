/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:20:02 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/01 15:55:48 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(char **argv, char ***envp)
{
	char	cwd[1024];
	char	*new_dir;

	if (argv[1] && argv[2])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return 1;
	}
	if (argv[1] == NULL || argv[1][0] == '\0')
	{
		new_dir = get_env_value("HOME", *envp);
		if (new_dir == NULL)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
	}else
	{
		new_dir = argv[1];
	}
	if(getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return errno;
	}
	if (chdir(new_dir) != 0)
	{
		perror("cd");
		return errno;
	}
	update_env("OLDPWD", cwd, envp);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return errno;
	}
	update_env("PWD", cwd, envp);
	
    return (0);
}
