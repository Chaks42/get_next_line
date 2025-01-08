/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:51:07 by vpoelman          #+#    #+#             */
/*   Updated: 2025/01/05 23:28:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_line(char *str_storage)
{
	size_t	i;
	char	*line;

	i = 0;
	if (!str_storage[i])
		return (NULL);
	while (str_storage[i] && str_storage[i] != '\n')
		i++;
	if (str_storage[i] == '\n')
		i++;
	line = (char *)malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (str_storage[i] && str_storage[i] != '\n')
	{
		line[i] = str_storage[i];
		i++;
	}
	if (str_storage[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

static char	*update_storage(char *str_storage)
{
	size_t	i;
	size_t	j;
	char	*new_storage;

	i = 0;
	while (str_storage[i] && str_storage[i] != '\n')
		i++;
	if (!str_storage[i] || !str_storage[i + 1])
	{
		free(str_storage);
		return (NULL);
	}
	new_storage = malloc(sizeof(char) * (ft_strlen(str_storage) - i));
	if (!new_storage)
	{
		free(str_storage);
		return (NULL);
	}
	j = 0;
	while (str_storage[++i])
		new_storage[j++] = str_storage[i];
	new_storage[j] = '\0';
	free(str_storage);
	return (new_storage);
}

static char	*handle_read(int fd, char *buffer, char *str_storage)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read < 0)
	{
		if (str_storage)
			free(str_storage);
		return (NULL);
	}
	buffer[bytes_read] = '\0';
	if (bytes_read == 0)
		return (str_storage);
	if (!str_storage)
	{
		str_storage = ft_strdup(buffer);
		if (!str_storage)
			return (NULL);
		return (str_storage);
	}
	temp = ft_strjoin(str_storage, buffer);
	if (!temp && str_storage)
		free(str_storage);
	return (temp);
}

static char	*read_and_store(int fd, char *str_storage)
{
	char	buffer[BUFFER_SIZE + 1];
	char	*temp;
	ssize_t	check;

	check = read(fd, buffer, 0);
	if (check < 0)
	{
		free(str_storage);
		return (NULL);
	}
	while (!str_storage || !ft_strchr(str_storage, '\n'))
	{
		temp = handle_read(fd, buffer, str_storage);
		if (!temp)
			return (NULL);
		if (temp == str_storage && !ft_strchr(temp, '\n'))
			return (temp);
		str_storage = temp;
	}
	return (str_storage);
}

char	*get_next_line(int fd)
{
	static char	*str_storage = NULL;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		if (str_storage)
		{
			free(str_storage);
			str_storage = NULL;
		}
		return (NULL);
	}
	str_storage = read_and_store(fd, str_storage);
	if (!str_storage)
		return (NULL);
	line = get_line(str_storage);
	if (!line)
	{
		free(str_storage);
		str_storage = NULL;
		return (NULL);
	}
	str_storage = update_storage(str_storage);
	return (line);
}
