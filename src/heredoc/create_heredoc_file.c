/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_heredoc_file.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndo-vale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 18:27:53 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/07/31 09:19:51 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	get_heredoc_lines(char *eof_lit, int fd)
{
	char	*line;

	line = readline(">");
	while (line && ft_strncmp(line, eof_lit, ft_strlen(eof_lit) + 1) != 0)
	{
		line = find_and_expand(line);
		if (!line && errno)
		{
			perror("envp expansion");
			close(fd);
			exit(errno);
		}
		if (line)
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = readline(">");
	}
	if (!line)
		ft_putstr_fd("warning: here-document delimited by end-of-file\n", 2);
	free(line);
	close(fd);
	exit(0);
}

void	create_heredoc_file(char *filename, char *eof)
{
	char	eof_lit[1024];
	int		fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(filename);
		free(filename);
		exit(errno);
	}
	ft_strlcpy(eof_lit, eof, ft_strlen(eof) + 1);
	free(eof);
	free(filename);
	get_heredoc_lines(eof_lit, fd);
}