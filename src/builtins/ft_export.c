/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/20 12:49:08 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int print_export_envs(char **envp)
{
	int i;

	char **sorted_envp = copy_envs(envp);
	if (!sorted_envp)
		return (errno);
	bubble_sort_envs(sorted_envp);
	print_sorted_envs(sorted_envp);
	i = -1;
	while (sorted_envp[++i])
		free(sorted_envp[i]);
	free(sorted_envp);
	return (0);
}
int	place_var_in_envp(char *var, char ***envp)
{
	char	**new_envp;
	char	*new_var;
	int		envs_am;

	new_var = ft_strdup(var);
	if (!new_var)
		return (-1);
	envs_am = count_envs(*envp);
	new_envp = (char **)ft_calloc(envs_am + 2, sizeof(char *));
	if (!new_envp)
		return (free(new_var), -1);
	fill_new_envp(new_envp, envp);
	new_envp[envs_am] = new_var;
	free(*envp);
	*envp = new_envp;
	return (0);
}


void append_env_var(char **arg_ptr, char **envp)
{
	int i;
	char *arg_key;
	char *old_value;
	char *arg_value;

	arg_key = get_env_key(*arg_ptr);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], arg_key, ft_strlen(arg_key)) == 0 && envp[i][ft_strlen(arg_key)] == '=')
		{
			old_value = get_env_value(arg_key, envp);
			arg_value = ft_strdup(ft_strnstr(*arg_ptr, "+=", ft_strlen(*arg_ptr)) + 2);
			free(*arg_ptr);
			*arg_ptr = ft_strjoin_free(ft_strjoin(arg_key, "="),
									   ft_strjoin_free(old_value, arg_value));
			delete_var(arg_key, envp);
			free(arg_key);
			return;
		}
		else if (ft_strncmp(envp[i], arg_key, ft_strlen(arg_key)) == 0)
		{
			arg_value = ft_strdup(ft_strnstr(*arg_ptr, "+=", ft_strlen(*arg_ptr)) + 2);
			free(*arg_ptr);
			*arg_ptr = ft_strjoin_free(ft_strjoin(arg_key, "="), arg_value);
			delete_var(arg_key, envp);
			free(arg_key);
			return;
		}
	}
	arg_value = ft_strdup(ft_strnstr(*arg_ptr, "+=", ft_strlen(*arg_ptr)) + 1);
	free(*arg_ptr);
	*arg_ptr = ft_strjoin_free(arg_key, arg_value);
}

void handle_invalid_key(char *arg) 
{
    ft_putstr_fd("export: `", 2);
    ft_putstr_fd(arg, 2);
    ft_putstr_fd("': not a valid identifier\n", 2);
}

void	process_var(char **arg_ptr, char ***envp, char *key, char *value)
{
	if (ft_strnstr(*arg_ptr, "+=", ft_strlen(*arg_ptr)))
	{
		append_env_var(&(*arg_ptr), *envp);
		place_var_in_envp(*arg_ptr, envp);
	}
	else if (ft_strchr(*arg_ptr, '='))
	{
		delete_var(key, *envp);
		place_var_in_envp(*arg_ptr, envp);
	}
	else if (!value)
	{
		delete_var(key, *envp);
		place_var_in_envp(*arg_ptr, envp);
	}
}

void	place_vars_in_envp(char **argv, char ***envp)
{
	char	*key;
	int		i;
	char	*value;

	i = 0;
	while (argv[++i])
	{
		key = get_key_from_var(argv[i]);
		value = get_env_value(key, *envp);
		if (!is_key_valid(key))
			handle_invalid_key(argv[i]);
		else
			process_var(&argv[i], envp, key, value);	

		free(key);
		free(value);
	}
}

int ft_export(char **argv, char ***envp)
{
	char *print_error;

	if (!argv[1])
		return (print_export_envs(*envp));
	if (argv[1][0] == '-' && argv[1][1] != '\0')
	{
		print_error = ft_strjoin_free(ft_strjoin("export: ", argv[1]), ft_strdup(": invalid option"));
		if (!print_error)
			return (errno);
		ft_print_error(print_error);
		free(print_error);
		return (INVALID_OPTION_CODE);
	}

	place_vars_in_envp(argv, envp);
	return (0); //TODO: check which error code should be used
}
