/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fivieira <fivieira@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/27 10:54:09 by ndo-vale          #+#    #+#             */
/*   Updated: 2024/08/01 21:25:34 by ndo-vale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//void	set_sig_hd(void)
void	set_signal_heredoc(void)
{
	signal(SIGINT, SIG_DFL);
/*	signal(SIGQUIT, SIG_DFL);
} //TODO: Organize this plzz

int	setget_signo(int action, int ntoset)
{
	static int	g_signo = 0;

	if (action == SET)
		g_signo = ntoset;
	else if (action == GET)
		return (g_signo);
	return (0);
}

void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		rl_replace_line("", 0);
		write(1, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
		setget_signo(SET, SIGINT);
	}*/
	signal(SIGQUIT, SIG_IGN);
}

void	psignal_handler_hd(int signo)
{
	(void)signo;
}//Necessary?

//void	set_signals(void)
void	set_signal_default(void)
{
	//signal(SIGINT, signal_handler);
	signal(SIGINT, signal_handler_default);
	signal(SIGQUIT, SIG_IGN);
}
void	set_signal_pipeline(void)
{
	signal(SIGINT, signal_handler_pipeline);
	signal(SIGQUIT, SIG_IGN);
}
