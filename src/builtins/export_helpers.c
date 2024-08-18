/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 20:09:41 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/18 23:23:12 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_envs(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

int	fill_new_envp(char **new, char ***old_ref)
{
	char	**old;
	int		i;

	old = *old_ref;
	i = -1;
	while (old[++i])
		new[i] = old[i];
	return (i);
}

int	compare_env(const void *a, const void *b)
{
    return ft_strncmp(*(const char **)a, *(const char **)b, ft_strlen(*(const char **)a));
}

char	**copy_and_sort_envs(char **envp)
{
    int count; 
    
    count = count_envs(envp);
    char **sorted_envp = (char **)malloc(sizeof(char *) * (count + 1));
    if (!sorted_envp)
        return NULL;
    int i = 0;
    while (i < count)
    {
        sorted_envp[i] = ft_strdup(envp[i]);
        i++;
    }
    sorted_envp[count] = NULL;
    qsort(sorted_envp, count, sizeof(char *), compare_env);
    return sorted_envp;
}

void	print_sorted_envs(char **sorted_envp)
{
    int i = 0;
    while (sorted_envp[i])
    {
        char *key_equals = ft_strldup(sorted_envp[i], get_env_key_len(sorted_envp[i]) + 1);
        char *value = ft_strdup(sorted_envp[i] + ft_strlen(key_equals));
        
        ft_printf("declare -x %s", key_equals);
        if (key_equals[get_env_key_len(sorted_envp[i])] == '=')
            ft_printf("\"%s\"", value);
        write(1, "\n", 1);
        
        free(key_equals);
        free(value);
        i++;
    }
}
