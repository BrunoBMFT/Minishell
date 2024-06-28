/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ycantin <ycantin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/28 19:05:11 by yohan             #+#    #+#             */
/*   Updated: 2024/04/24 18:23:39 by ycantin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	node_amount;

	node_amount = 0;
	while (lst)
	{
		node_amount++;
		lst = lst->next;
	}
	return (node_amount);
}
/*
int	main(void)
{
	t_list *node1 = ft_lstnew("one");
	t_list *node2 = ft_lstnew("two");
	t_list *node3 = ft_lstnew("three");
	t_list *node4 = ft_lstnew("four");
	t_list *node5 = ft_lstnew("Last node");
	t_list *lst = NULL;
	ft_lstadd_front(&lst, node1);
	ft_lstadd_front(&lst, node2);
	ft_lstadd_front(&lst, node3);
	ft_lstadd_front(&lst, node4);
	ft_lstadd_front(&lst, node5);

	int size = ft_lstsize(lst);
	printf ("number of nodes: %d\n", size);
	while (lst)
	{
		t_list *temp = lst;
		lst = lst -> next;
		free (temp);
	}
	return (0);
} */