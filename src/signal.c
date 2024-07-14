#include "../minishell.h"

/* utils */
void	move_cursor(void)
{
	ft_putstr_fd("\033[1A", STDERR_FILENO);
	ft_putstr_fd("\033[12C", STDERR_FILENO);
}

/* signal*/
void	replace_line(int redisplayon)
{
	ft_putchar_fd('\n', STDERR_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (redisplayon == TRUE)
		rl_redisplay();
}

//child prcoess에서 받는 SIGTERM 시그널은 execve에서 처리해주는 듯 
// void	child_handler(int signo) 
// {
// 	status = signo + SIGEXIT;
// 	// printf("signal receive %d\n", status);
// 	// if (signo == SIGINT)
// 	// {
// 	// 	ft_putstr_fd("child_sigint\n", STDERR_FILENO);
// 	// 	ft_putstr_fd("^C\n", STDERR_FILENO);
// 	// }00
// 	if (signo == SIGQUIT)
// 	{
// 		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
// 		// exit(0);
// 		// ft_putstr_fd("child_sigterm\n", STDERR_FILENO);
// 	}
// 	// if (signo == SIGTERM)
// 	// {
// 		// ft_putstr_fd("child_sigterm\n", STDERR_FILENO);
// 	// }
// }

void	main_handler(int signo)
{
	status = signo + SIGEXIT;
	// printf("signal receive %d\n", status);
	if (signo == SIGINT)
	{
		ft_putstr_fd("main_sigint\n", STDERR_FILENO);
		replace_line(TRUE);
		// set_terminal_printoff();
	}
	if (signo == SIGTERM)
	{
		move_cursor();
		ft_putstr_fd("main_sigterm\n", STDERR_FILENO);
		ft_putstr_fd("exit\n", STDERR_FILENO);
	}
	// set_terminal_printoff();
}

void set_signal_default(t_shell *shell, void(*handler)(int))
{
	t_sigaction action;

	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(SIGINT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
	if (sigaction(SIGTERM, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
	if (sigaction(SIGQUIT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
}

void set_signal_parent(t_shell *shell, void(*handler)(int))
{
	t_sigaction action;

	(void)handler;
	action.sa_handler = SIG_IGN;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (sigaction(SIGINT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
}

void set_signal_child(t_shell *shell, void(*handler)(int))
{
	t_sigaction action;

	(void) handler;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	// set_terminal_printon(shell);
	// if (sigaction(SIGINT, &action, NULL) == (int)SIG_ERR)
	// 	exit_process(shell, NULL, EXTRA_ERROR);
	// if (sigaction(SIGTERM, &action, NULL) == (int)SIG_ERR)
		// exit_process(shell, NULL, EXTRA_ERROR);
	if (sigaction(SIGQUIT, &action, NULL) == (int)SIG_ERR)
		exit_process(shell, NULL, EXTRA_ERROR);
}

//메인 프로세스 - set signal(main_handler)
//자식 프로세스 - set signal(child handler)
//부모 프로세스 - set signal(parent handler)
//플래그 세울까 하다가 파라미터 개수, if문 개수 많아지면 안 좋으니가 함수 따로 짜기로!
//비트연산 속도 빠르고 연산부하가 낮아서 쓰는건데... 
//자식프로세스에서 execve를 실행한 후에 sigterm하면 child handler/main handler로 안들어감
//자식프로세스 내에서 sigaction 설정을 바꿔주면 자식과 프로세스