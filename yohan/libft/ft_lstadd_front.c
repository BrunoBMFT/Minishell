/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 14:03:18 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:20 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (new)
	{
		new->next = *lst;
		*lst = new;
	}
}

/*
int	main(void)
{
	t_list	*lst;
	t_list	*temp;
	t_list	*newvalue;
	char	*str1;
	char	*str2;
	char	*fullcontent;

	str1 = "first";
	str2 = "second";
	lst = ft_lstnew(str1);
	newvalue = ft_lstnew(str2);
	temp = lst;
	ft_lstadd_front(&lst, newvalue);
	fullcontent = temp -> content;
	while (temp)
	{
		printf("%s\n", fullcontent);
		temp = temp -> next;
	}
	while (lst)
	{
		free (lst -> content);
		free (lst);
	}
	return (0);
}
*/