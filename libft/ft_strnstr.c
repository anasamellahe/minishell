/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:45:40 by anamella          #+#    #+#             */
/*   Updated: 2023/11/08 18:50:54 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	t;

	i = 0;
	t = 0;
	if (*needle == '\0')
		return ((char *)haystack);
	while (i < len && i < ft_strlen(haystack))
	{
		t = 0;
		while (needle[t] == haystack[i] && i < ft_strlen(haystack) && i < len)
		{
			i++;
			t++;
		}
		if (!needle[t])
			return (&((char *)haystack)[i - t]);
		i -= t;
		i++;
	}
	return (NULL);
}
