/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:44:59 by anamella          #+#    #+#             */
/*   Updated: 2024/05/14 01:05:23 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char *var)
{
	if (var)
		free(var);
	var = NULL;
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	len;
	size_t	len1;

	if (!s1 && !s2)
		return (NULL);
	if (s1)
		len = ft_strlen(s1);
	else
		len = 0;
	if (s2)
		len1 = ft_strlen(s2);
	else
		len1 = 0;
	ptr = malloc(len + len1 + 1);
	if (!ptr)
		return (NULL);
	if (s1)
		ft_memcpy(ptr, s1, len);
	if (s2)
		ft_memcpy((ptr + len), s2, len1);
	ptr[len + len1] = '\0';
	ft_free((char *)s1);
	return (ptr);
}
