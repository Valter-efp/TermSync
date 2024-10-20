/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termsync.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafernan <vafernan@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 09:29:59 by vafernan          #+#    #+#             */
/*   Updated: 2024/10/20 09:30:01 by vafernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMSYNC_H
# define TERMSYNC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <pty.h>
# include <errno.h>
# include <sys/select.h>
# include <sys/time.h>
# include <sys/stat.h>

# define BUF_SIZE 1024

//termsync_utils.c
void	set_raw_mode(void);
void	restore_terminal_mode(void);
void	print_terminal_output(const char *label, const char *buf,
			ssize_t n_read);
void	handle_keystrokes(int custom_terminal_fd, int fifo_fd);
void	handle_minishell_output(int custom_terminal_fd);

//termsync_utils2.c
void	setup_fifo(const char *fifo_path);
int		open_fifo(const char *fifo_path);

#endif