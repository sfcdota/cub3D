/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cbach <cbach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/25 23:00:48 by cbach             #+#    #+#             */
/*   Updated: 2020/05/29 23:52:45 by cbach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *),
void (*del)(void *))
{
	t_list *n;
	t_list *first;

	if (lst)
	{
		first = ft_lstnew(f(lst->content));
		if (first)
		{
			n = first;
			while (lst->next)
			{
				lst = lst->next;
				n->next = ft_lstnew(f(lst->content));
				if (!n->next)
				{
					ft_lstclear(&first, del);
					return (NULL);
				}
				n = n->next;
			}
		}
		return (first);
	}
	return (NULL);
}
