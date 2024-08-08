#include "../minishell.h"

void	syntax_simple_cmd(t_token **dsttoken, t_token *nowtoken);
void	syntax_cmd_redirects(t_token **dsttoken, t_token *nowtoken);
void	syntax_io_redirect(t_token **dsttoken, t_token *nowtoken);

void	syntax_pipeline(t_token **dsttoken, t_token *nowtoken, int firstopt)
{
	t_token *nexttoken;
	t_token *lasttoken;

	(void) firstopt;
	(void) lasttoken;

	if (!nowtoken)
		return ;
	//다음토큰 담기 
	nexttoken = nowtoken->right;
	//현재토큰
	if (nowtoken->type == T_PIPE)
	{	
		nowtoken->right = NULL;
		if(!*dsttoken)
			*dsttoken = nowtoken;
		else
			tlst_addright(dsttoken, nowtoken);
	}
	else
	{	
		// printf("dst: %p\n", *dsttoken);
		// printf("last: %p\n", tlst_lastright(*dsttoken));
		lasttoken = tlst_lastright(*dsttoken);
		if (lasttoken)
			syntax_simple_cmd(&lasttoken->left, nowtoken);
	}
	// printf("now: %p\n", nowtoken);
	// printf("next: %p\n", nexttoken);
	//다음토큰
	if (nexttoken)
		syntax_pipeline(dsttoken, nexttoken , 0);
}

//left
void	syntax_simple_cmd(t_token **dsttoken, t_token *nowtoken)
{
	t_token *newtoken;
	t_token *nexttoken;

	// (void) firstopt;
	(void) dsttoken;
	(void) nowtoken;
	(void) newtoken;
	(void) nexttoken;
	// printf("ok\n");

	if (!*dsttoken)
	{
		set_token(&newtoken); // NULL처리
		newtoken->type = T_SIMPLE_CMD;
		*dsttoken = newtoken;
	}
	nexttoken = nowtoken->right;
	if (nowtoken && nowtoken->type == T_CMD_WORD)
	{
		nowtoken->right = NULL;	
		tlst_addright(&(*dsttoken)->right, nowtoken);
	}
	else if (nowtoken)
		syntax_cmd_redirects(&(*dsttoken)->left, nowtoken);
	
	// if (nexttoken)
	// 	syntax_simple_cmd(&(*dsttoken)->right, nexttoken);
}	

void	syntax_cmd_redirects(t_token **dsttoken, t_token *nowtoken)
{
	t_token *newtoken;
	t_token *nexttoken;
	t_token *lasttoken;

	(void) nexttoken;
	(void) lasttoken;
	if (!nowtoken || nowtoken->type == T_PIPE)
		return ;
	// nexttoken = nowtoken->right;
	if (nowtoken && nowtoken->type >= T_DLESS && nowtoken->type <= T_GREAT)
	{	
		set_token(&newtoken); // NULL처리
		newtoken->type = T_IO_REDIRECTS;
		if (!*dsttoken)
			*dsttoken = newtoken;
		else 
			tlst_addright(dsttoken, newtoken);
		lasttoken = tlst_lastright(*dsttoken);
		syntax_io_redirect(&lasttoken, nowtoken);
	}
	// if (nexttoken)
	// 	syntax_cmd_redirects(dsttoken, nexttoken);
	// if (nexttoken)
	// {
		
	// 	printf("ok\n");
	// 	lasttoken = tlst_lastright(*dsttoken);
	// 	syntax_cmd_redirects(&lasttoken->right, nexttoken);
	// }
}

void	syntax_io_redirect(t_token **dsttoken, t_token *nowtoken)
{
	if (nowtoken && nowtoken->type >= T_DLESS && nowtoken->type <= T_GREAT)
	{	
		nowtoken->right = NULL;
		(*dsttoken)->left = nowtoken;
	}

	// if ()
	// 	syntax_io_here();
	// else 
	// 	syntax_io_file();
}

// void	syntax_io_file()
// {

// }

// void	syntax_io_here()
// {

// }

void print_tree(t_token *dsttoken, int leftright, int i)
{
	if(!dsttoken)
		return ;
	if (leftright == 0)
		printf("---- left %d----\n", i);
	else if (leftright == 1)
		printf("---- right %d----\n", i);
	else if (leftright == 2)
		printf("---- node %d----\n", i);

	printf("type: %d#\n", dsttoken->type);
	printf("word: %s#\n", dsttoken->word);
	int idx = 0;
	while (dsttoken->argv && dsttoken->argv[idx])
		printf("argv: %s#\n", dsttoken->argv[idx++]);

	printf("now: %p#\n", dsttoken);
	printf("left: %p#\n", dsttoken->left);
	printf("right: %p#\n\n", dsttoken->right);
	if (dsttoken->left)
		print_tree(dsttoken->left, 0, i + 1);
	if (dsttoken->right)
		print_tree(dsttoken->right, 1, i + 1);
}


t_token	*parser(t_token **token)
{
	t_token *nowtoken;
	t_token *dsttoken;

	dsttoken = NULL;
	nowtoken = *token;
	// while (nowtoken)
	// {
	// 	printf("%d\n", nowtoken->type);
	// 	nowtoken = nowtoken->right;
	// }
	syntax_pipeline(&dsttoken, nowtoken, 1);
	// print_tree(dsttoken, 2, 0);
	*token = dsttoken;
	return (dsttoken);
}