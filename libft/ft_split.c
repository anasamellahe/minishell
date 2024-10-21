/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 02:47:31 by anamella          #+#    #+#             */
/*   Updated: 2023/11/14 23:21:57 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**ft_count(char const *s, char c)
{
	size_t	i;
	size_t	words;
	size_t	t;

	i = 0;
	words = 0;
	t = 0;
	while (s[i])
	{
		if (!(s[i] == c) && t == 0)
		{
			words++;
			t = 1;
		}
		if (s[i] == c && t == 1)
			t = 0;
		i++;
	}
	return ((char **)malloc((words + 1) * sizeof(char *)));
}

static char	*ft_strndup(char const *s, size_t n)
{
	size_t	i;
	char	*ptr;

	i = 0;
	ptr = malloc(n + 1);
	if (!ptr)
		return (NULL);
	while (i < n)
	{
		ptr[i] = s[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

static char	**ft_free(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
	return (NULL);
}

static char	**ft_count2(char const *s, char c, char **ptr)
{
	size_t	i;
	size_t	words;
	size_t	n;

	i = 0;
	n = 0;
	while (i < (size_t)ft_strlen(s))
	{
		if (!(s[i] == c))
		{
			words = 0;
			while (s[i] != c && s[i])
			{
				words++;
				i++;
			}
			ptr[n] = ft_strndup(&s[i - words], words);
			if (!ptr[n])
				return (ft_free(ptr));
			n++;
		}
		i++;
	}
	ptr[n] = 0;
	return (ptr);
}

char	**ft_split(char const *s, char c)
{
	char	**ptr;

	if (!s)
		return (NULL);
	ptr = ft_count(s, c);
	if (!ptr)
		return (NULL);
	if (!ft_count2(s, c, ptr))
		return (NULL);
	return (ptr);
}
