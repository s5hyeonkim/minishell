#include "../minishell.h"

int set_token(t_token **t)
{
	*t = ft_calloc(1, sizeof(t_token));
	if (*t == NULL)
		return (EXTRA_ERROR);
	return (EXIT_SUCCESS);
}

t_token *tlst_lastright(t_token *t)
{
	if (t == NULL)
		return (t);
	while (t->right)
		t = t->right;
	return (t);
}

t_token *tlst_lastleft(t_token *t)
{
	if (t == NULL)
		return (NULL);
	while (t->left != NULL)
		t = t->left;
	return (t);
}

void tlst_addright(t_token **t, t_token *newtoken)
{
	t_token *lasttoken;

	// printf("%s\n", newtoken->word);
	if (*t == NULL)
		*t = newtoken;
	else
	{
		lasttoken= tlst_lastright(*t);
		lasttoken->right = newtoken; 
	}

}

void tlst_addleft(t_token **t, t_token *newtoken)
{
	t_token *lasttoken;

	if (*t == NULL)
		*t = newtoken;
	else if (newtoken != NULL)
	{
		lasttoken= tlst_lastleft(*t);
		lasttoken->left = newtoken; 
	}
}
