/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 03:50:57 by anamella          #+#    #+#             */
/*   Updated: 2023/11/10 23:51:04 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	src_len;
	size_t	dst_len;
	size_t	len;

	i = 0;
	src_len = ft_strlen(src);
	dst_len = ft_strlen(dst);
	len = 0;
	while (len < dstsize && dst[len])
		len++;
	if (len > dstsize)
		return (len + src_len);
	if (dst_len < dstsize)
	{
		while (i < src_len && i < dstsize - dst_len - 1)
		{
			dst[dst_len + i] = src[i];
			i++;
		}
		dst[dst_len + i] = '\0';
	}
	return (len + src_len);
}
