/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anamella <anamella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 15:42:59 by anamella          #+#    #+#             */
/*   Updated: 2024/05/24 14:52:30 by anamella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(int n)
{
	int	c;

	c = (n <= 0);
	while ((long int)n * ((n > 0) - (n < 0)) > 0 && ++c)
		n = n / 10 * ((n > 0) - (n < 0));
	return (c);
}

static void	putinptr(unsigned int n, int count, char *ptr)
{
	if (n > 9)
		putinptr(n / 10, count - 1, ptr);
	ptr[count] = n % 10 + 48;
}

char	*ft_itoa(int n)
{
	char	*ptr;
	int		c;

	c = ft_count(n);
	ptr = malloc(c + 1);
	if (!ptr)
		return (NULL);
	if (n < 0)
		ptr[0] = '-';
	putinptr(n * ((n > 0) - (n < 0)), c - 1, ptr);
	ptr[c] = '\0';
	return (ptr);
}
