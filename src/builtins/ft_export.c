/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <ndo-vale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2024/08/19 20:16:12 by ndo-vale         ###   ########.fr       */
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
void place_var_in_envp(char *var, char ***envp)
{
	char **new_envp;

	new_envp = (char **)ft_calloc(count_envs(*envp) + 2, sizeof(char *));
	fill_new_envp(new_envp, envp);
	new_envp[count_envs(*envp)] = ft_strdup(var);
	free(*envp);
	*envp = new_envp;
}

int place_variables_in_envp(char **argv, char **new_envp, int count)
{
	int i;
	int error_code;
	char *key;

	i = 0;
	error_code = 0;
	while (argv[++i])
	{
		key = get_key_from_var(argv[i]);
		if (is_key_valid(key) && (ft_strchr(argv[i], '=') || !get_env_value(argv[i], new_envp)))
		{
			new_envp[count] = ft_strdup(argv[i]);
			if (!new_envp[count])
				return (-1);
			count += 1;
		}
		else
			error_code = 1;
		free(key);
	}
	return (error_code);
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

int count_exports(char **argv, char ***envp)
{
	char *key;
	int count;
	int i;

	count = 0;
	i = 0;
	while (argv[++i])
	{
		key = get_key_from_var(argv[i]);
		if (!is_key_valid(key))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else if (ft_strnstr(argv[i], "+=", ft_strlen(argv[i])))
		{
			append_env_var(&(argv[i]), *envp);
			count++;
		}
		else if (ft_strchr(argv[i], '='))
		{

			delete_var(key, *envp);
			count++;
		}
		else if (!get_env_value(key, *envp))
			count++;
		free(key);
	}
	return (count);
}






void	place_vars_in_envp(char **argv, char ***envp)
{
	char	*key;
	int	i;

	i = 0;
	while (argv[++i])
	{
		key = get_key_from_var(argv[i]);
		if (!is_key_valid(key))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
		}
		else if (ft_strnstr(argv[i], "+=", ft_strlen(argv[i])))
		{
			append_env_var(&(argv[i]), *envp);
			place_var_in_envp(argv[i], envp);
		}
		else if (ft_strchr(argv[i], '='))
		{
			delete_var(key, *envp);
			place_var_in_envp(argv[i], envp);
		}
		else if (!get_env_value(key, *envp))
			place_var_in_envp(argv[i], envp);
		free(key);
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
