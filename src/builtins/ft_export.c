/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 10:21:16 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/01 10:21:00 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	print_export_envs(char **envp)
{
	char	*key_equals;
	char	*value;
	int		i;

	i = -1;
	while (envp[++i])
	{
		key_equals = ft_strldup(envp[i], get_env_key_len(envp[i]) + 1);
		if (!key_equals)
			return (errno);
		value = ft_strdup(envp[i] + ft_strlen(key_equals));
		if (!value)
			return (free(key_equals), errno);
		ft_printf("declare -x %s", key_equals);
		if (key_equals[get_env_key_len(envp[i])] == '=')
			ft_printf("\"%s\"", value);
		write(1, "\n", 1);
		free(key_equals);
		free(value);
	}
	return (0);
}

int	validate_argv(char **argv, char **envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (argv[++i])
	{
		if (is_key_valid(argv[i]))
		{
			delete_var(argv[i], envp);
			count++;
		}
		else
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("\': not a valid identifier\n", 2);
		}
	}
	return (count);
}

int	place_variables_in_envp(char **argv, char **new_envp, int count)
{
	int	i;
	int	error_code;

	i = 0;
	error_code = 0;
	while (argv[++i])
	{
		if (is_key_valid(argv[i]))
		{
			new_envp[count] = ft_strdup(argv[i]);
			if (!new_envp[count])
				return (-1);
			count += 1;
		}
		else
			error_code = 1;
	}
	return (error_code);
}

int	ft_export(char **argv, char ***envp)
{
	char	**new_envp;
	int		count;
	int		error_code;

	if (!argv[1])
		return (print_export_envs(*envp));
	count = validate_argv(argv, *envp);
	new_envp = (char **)ft_calloc(count_envs(*envp) + count + 1,
			sizeof(char *));
	if (!new_envp)
		return (errno);
	count = fill_new_envp(new_envp, envp);
	error_code = place_variables_in_envp(argv, new_envp, count);
	if (error_code < 0)
		return (errno);
	free(*envp);
	*envp = new_envp;
	return (error_code);
}
