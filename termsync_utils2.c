/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termsync_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vafernan <vafernan@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 09:29:49 by vafernan          #+#    #+#             */
/*   Updated: 2024/10/20 09:29:50 by vafernan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "termsync.h"

/// @brief Sets a Fifo file at the specified path.
//  If already there, it is removed and a new one is created.
/// @param fifo_path Path where the fifo file is to be created.
void	setup_fifo(const char *fifo_path)
{
	unlink(fifo_path);
	if (mkfifo(fifo_path, 0666) == -1)
	{
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}
}

/// @brief Opens the fifo for writing.
/// @param fifo_path Path of the Fifo filo to be opened.
/// @return  Fd of the fifo file.
int	open_fifo(const char *fifo_path)
{
	int	fifo_fd;

	fifo_fd = open(fifo_path, O_WRONLY);
	if (fifo_fd == -1)
	{
		perror("open fifo");
		exit(EXIT_FAILURE);
	}
	return (fifo_fd);
}
