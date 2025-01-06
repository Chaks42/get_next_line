/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 21:45:06 by vpoelman          #+#    #+#             */
/*   Updated: 2025/01/06 22:58:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_and_store(int fd, char *str_storage)
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes_read;
	char	*temp_text;

	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(str_storage);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		temp_text = ft_strjoin(str_storage, buffer);
		free(str_storage);
		str_storage = temp_text;
		if (str_storage && ft_strchr(str_storage, '\n'))
			break ;
	}
	return (str_storage);
}

static int	find_line_length(char *text)
{
	int	i;

	i = 0;
	while (text[i] && text[i] != '\n')
		i++;
	if (text[i] == '\n')
		return (i + 2);
	return (i + 1);
}

static char	*extract_line(char *text)
{
	char	*current_line;
	int		i;
	int		line_length;

	if (!text || !*text)
		return (NULL);
	line_length = find_line_length(text);
	current_line = malloc(sizeof(char) * (line_length));
	if (!current_line)
		return (NULL);
	i = 0;
	while (text[i] != '\n' && text[i] != '\0')
	{
		current_line[i] = text[i];
		i++;
	}
	if (text[i] == '\n')
	{
		current_line[i] = '\n';
		current_line[i + 1] = '\0';
	}
	else
		current_line[i] = '\0';
	return (current_line);
}

static char	*update_remaining_text(char *text)
{
	char	*remaining_text;
	int		i;

	i = 0;
	while (text[i] && text[i] != '\n')
		i++;
	if (!text[i])
	{
		free(text);
		return (NULL);
	}
	remaining_text = ft_strdup(text + i + 1);
	free(text);
	return (remaining_text);
}

char	*get_next_line(int fd)
{
	static char	*str_storage = NULL;
	char		*current_line;

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
	current_line = extract_line(str_storage);
	if (!current_line)
	{
		free(str_storage);
		str_storage = NULL;
		return (NULL);
	}
	str_storage = update_remaining_text(str_storage);
	return (current_line);
}
/*
int main()
{
	int	fd;

	fd = open("txt.txt", O_RDONLY);
	char *str;
	for (int i = 0; i < 55555; i++)
	{			
		str = get_next_line(fd);
		printf("%s", str);
		free(str);
	}
	//system("leaks a.out");
	return 0;
}
*/
