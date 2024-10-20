/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termsync.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafernan <vafernan@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 09:29:54 by vafernan          #+#    #+#             */
/*   Updated: 2024/10/20 09:29:55 by vafernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termsync.h"

extern char	**environ;

/// @brief Starts the costum terminal.
/// @param custom_terminal_fd Ptr to the fd of the custom terminal.
/// @param custom_pid Ptr to the pid of the custom terminal.
void	start_minishell(int *custom_terminal_fd, pid_t *custom_pid)
{
	char	*custom_argv[2];

	custom_argv[0] = "minishell";
	custom_argv[1] = NULL;
	*custom_pid = forkpty(custom_terminal_fd, NULL, NULL, NULL);
	if (*custom_pid == -1)
	{
		perror("forkpty for minishell");
		exit(EXIT_FAILURE);
	}
	if (*custom_pid == 0)
	{
		printf("Starting minishell...\n");
		fflush(stdout);
		if (access(custom_argv[0], F_OK) == -1)
		{
			perror("access failed");
			exit(EXIT_FAILURE);
		}
		execve(custom_argv[0], custom_argv, environ);
		perror("execve minishell failed");
		exit(EXIT_FAILURE);
	}
}

/// @brief Starts the xterm process with bash.
/// @param bash_pid Ptr to the pid of the bash process.
void	start_xterm(pid_t *bash_pid)
{
	char	*xterm_argv[8];

	xterm_argv[0] = "xterm";
	xterm_argv[1] = "-fa";
	xterm_argv[2] = "Monospace";
	xterm_argv[3] = "-fs";
	xterm_argv[4] = "10";
	xterm_argv[5] = "-e";
	xterm_argv[6] = "bash -c 'cat < /tmp/termsync_fifo | /bin/bash'";
	xterm_argv[7] = NULL;
	*bash_pid = fork();
	if (*bash_pid == -1)
	{
		perror("fork for xterm");
		exit(EXIT_FAILURE);
	}
	if (*bash_pid == 0)
	{
		printf("Starting xterm with bash...\n");
		fflush(stdout);
		execve("/usr/bin/xterm", xterm_argv, environ);
		perror("execve xterm failed");
		exit(EXIT_FAILURE);
	}
}

/// @brief Handles input and output between the custom terminals.
/// @param custom_terminal_fd Fd of the custom terminal.
/// @param fifo_fd Fd of the fifo file for inter-process comunication,
/// this named pipe allows data to be read and written in a sequential manner.
void	main_loop(int custom_terminal_fd, int fifo_fd)
{
	fd_set	read_fds;
	int		max_fd;
	int		ready;

	set_raw_mode();
	printf("TermSync started. Type to send keystrokes to both terminals...\n");
	fflush(stdout);
	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(STDIN_FILENO, &read_fds);
		FD_SET(custom_terminal_fd, &read_fds);
		max_fd = custom_terminal_fd + 1;
		ready = select(max_fd, &read_fds, NULL, NULL, NULL);
		if (ready == -1)
		{
			perror("select");
			break ;
		}
		if (FD_ISSET(STDIN_FILENO, &read_fds))
			handle_keystrokes(custom_terminal_fd, fifo_fd);
		if (FD_ISSET(custom_terminal_fd, &read_fds))
			handle_minishell_output(custom_terminal_fd);
	}
	restore_terminal_mode();
}

/// @brief Stets up and starts the TermSync program.
/// @return Exit status.
int	main(void)
{
	int			custom_terminal_fd;
	int			fifo_fd;
	pid_t		custom_pid;
	pid_t		bash_pid;
	const char	*fifo_path;

	fifo_path = "/tmp/termsync_fifo";
	setup_fifo(fifo_path);
	start_minishell(&custom_terminal_fd, &custom_pid);
	start_xterm(&bash_pid);
	fifo_fd = open_fifo(fifo_path);
	main_loop(custom_terminal_fd, fifo_fd);
	close(fifo_fd);
	unlink(fifo_path);
	waitpid(custom_pid, NULL, 0);
	waitpid(bash_pid, NULL, 0);
	return (0);
}
