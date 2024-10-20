/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termsync_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafernan <vafernan@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 09:29:43 by vafernan          #+#    #+#             */
/*   Updated: 2024/10/20 09:29:45 by vafernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termsync.h"

/// @brief Sets the terminal to raw mode.
/// Raw mode is a mode that disables line buffering and echo.
/// Allows for reading one character at a time.
void	set_raw_mode(void)
{
	struct termios	t;

	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

/// @brief Restores the terminal to its original mode.
/// Enabling line buffering and echo.
/// Must be called at the exit so the terminal is not left in raw mode.
void	restore_terminal_mode(void)
{
	struct termios	t;

	tcgetattr(STDIN_FILENO, &t);
	t.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

/// @brief Prints the opuput from the terminal with a label.
/// The label is used to indicate the costum terminal.
/// @param label Label to indicate the terminal.
/// @param buf Contains the output from the terminal.
/// @param n_read bytes read into the buffer.
void	print_terminal_output(const char *label, const char *buf,
		ssize_t n_read)
{
	static int	new_line = 1;

	if (new_line)
	{
		printf("[%s]: ", label);
		new_line = 0;
	}
	fwrite(buf, 1, n_read, stdout);
	if (buf[n_read - 1] == '\n')
		new_line = 1;
	fflush(stdout);
}

/// @brief Sends the keystrokes to the custom terminal and the fifo.
/// @param custom_terminal_fd Fd of the custom terminal.
/// @param fifo_fd Ff of the fifo file.
void	handle_keystrokes(int custom_terminal_fd, int fifo_fd)
{
	char	buf[BUF_SIZE];
	ssize_t	n_read;

	n_read = read(STDIN_FILENO, buf, BUF_SIZE);
	if (n_read <= 0)
		return ;
	if (write(custom_terminal_fd, buf, n_read) == -1)
		perror("write to minishell failed");
	if (write(fifo_fd, buf, n_read) == -1)
		perror("write to fifo failed");
}

/// @brief Prints the output from the minishell in the std output.
/// @param custom_terminal_fd Fd of the custom terminal.
void	handle_minishell_output(int custom_terminal_fd)
{
	char	buf[BUF_SIZE];
	ssize_t	n_read;

	n_read = read(custom_terminal_fd, buf, BUF_SIZE);
	if (n_read > 0)
		print_terminal_output("Minishell", buf, n_read);
	else if (n_read == 0)
	{
		printf("\nMinishell has exited.\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		perror("read from minishell failed");
		exit(EXIT_FAILURE);
	}
}
