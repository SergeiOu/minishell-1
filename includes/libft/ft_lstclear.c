/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yabodaya <yabodaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 11:39:01 by yabodaya          #+#    #+#             */
/*   Updated: 2025/06/10 12:22:29 by yabodaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;
	t_list	*nnode;

	if (!lst || !*lst || !del)
		return ;
	temp = *lst;
	while (temp)
	{
		nnode = temp->next;
		del(temp->content);
		free(temp);
		temp = nnode;
	}
	*lst = NULL;
}
