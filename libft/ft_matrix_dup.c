/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix_dup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 14:17:36 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/16 15:36:29 by fivieira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

void	**ft_matrix_dup(void **matrix)
{
	char	**matrix_str;
	char	**dup;
	int		i;
	int		size;

	matrix_str = (char **)matrix;
	i = 0;
	while (matrix_str[i])
		i++;
	size = i;
	dup = (char **)ft_calloc(size + 1, sizeof(char *));
	if (!dup)
		return (NULL);
	i = -1;
	while (++i < size)
	{
		dup[i] = ft_strldup(matrix_str[i], ft_strlen(matrix_str[i]));
		if (!(dup[i]))
			return (ft_matrix_free((void ***)(&dup)), NULL);
	}
	return ((void **)dup);
}
