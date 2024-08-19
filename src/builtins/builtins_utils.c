/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 20:17:33 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/19 14:44:39 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_option(char *arg)
{
	if (!arg)
		return (0);
	if (arg[0])
		return (arg[0] == '-' && arg[1]);
	return (0);
}

char	*get_key_from_var(char *var)
{
	int	i;

	i = -1;
	if (!var)
		return (NULL);
	while (var[++i] && var[i] != '='
			&& !(var[i] == '+' && var[i + 1] == '='))
		continue ;
	return (ft_strldup(var, i));
}

int	is_key_valid(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	while (key[++i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
	}
	return (i);
}

int	get_envp_i_from_key(char *key, char **envp)
{
	int	i;
	int	key_len;

	key_len = ft_strlen(key);
	if (!is_key_valid(key))
		return (-1);
	i = -1;
	if (!envp)
		return (-1);
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0)
			return (i);
	}
	return (-1);
}

void	delete_var(char *key, char **envp)
{
	int	var_i;

	var_i = get_envp_i_from_key(key, envp);
	if (var_i >= 0)
	{
		free(envp[var_i]);
		while (envp[++var_i])
			envp[var_i - 1] = envp[var_i];
		envp[var_i - 1] = NULL;
	}
}

t_builtin	get_builtin(char *cmd)
{
	static char	*builtins[BUILTINS_AM + 1] = {
		"echo", "cd", "pwd", "export", "unset",
		"env", "exit", NULL};
	static int	(*builtin_funcs[BUILTINS_AM + 1])(char **, char ***) = {
		&ft_echo, &ft_cd, &ft_pwd, &ft_export, &ft_unset,
		&ft_env, &ft_exit, NULL};
	int			i;

	i = -1;
	if (!cmd)
		return (NULL);
	while (builtins[++i])
	{
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (builtin_funcs[i]);
	}
	return (NULL);
}

int	run_builtin(t_list *argv_lst, char ***envp)
{
	char	**args;
	int		status;

	args = create_args(argv_lst);
	if (!args)
		return (errno);
	status = get_builtin(args[0])(args, envp);
	ft_matrix_free((void ***)&args);
	return (status);
}
