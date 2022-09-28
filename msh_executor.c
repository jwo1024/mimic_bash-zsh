
#include "minishell.h"
#include "msh_tree.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

enum	e_pipe_fd {
	PIPE_OUT = 0,
	PIPE_IN = 1,
	STD_IN = 0,
	STD_OUT = 1
};

char	**msh_executor_get_path(char **envp_list);

/* utils */
char	**msh_executor_get_path(char **envp_list);
pid_t	*msh_executor_malloc_pids(t_tree *tree);


/* fork wait*/
pid_t	*msh_executor_fork(t_node *pipe_nd, char **env_path, pid_t *pids);
int		msh_executor_wait_child(int *pids);


/* run */
int		msh_run_cmd(t_node *cmd_nd, int fd[2], char **envp_list);
int		msh_run_simp_cmd(t_node *cmd_nd, char **env_path);
char	*msh_get_cmd_path(char *cmd, char **env_path);


/* redirection */
int	msh_redirection(t_node *redirct_nd, int fd[2]);
int	msh_set_redirection(t_node *redirct_nd, int	fd[2]);


int	msh_executor(t_tree *tree, char **envp_list) // env.. 
{
	pid_t	*pids;
	int		rtn;
	char	**env_path;

	// 환경변수 PATH 가공하여 이차원배열로 받아오기
	env_path = msh_executor_get_path(envp_list);

	// pipe 존재만큼 pid 배열 생성
	pids = msh_executor_malloc_pids(tree); // process가 1개일 경우에는 ?
	if (pids == NULL)
		return (-1);
	// 자식 프로세스 fork 및 파이프 할당
		// export시 단일 명령어로 실행될때만 적용되는 사안은 자식 프로세스가 아닌 메인 프로세스에서 실행된다고 보아도 무방한가?
	msh_executor_fork(tree->top, env_path, pids);
	// ;



	rtn = msh_executor_wait_child(pids);
	// free(tree);
	return (1);
}



/* utils */
char	**msh_executor_get_path(char **envp_list)
{
	int		i;
	char	**path;
	char	*tmp;

	i = 0;
	while (envp_list[i])
	{
		if (ft_strncmp(envp_list[i], "PATH=", 5) == 0)
		{
			path = ft_split(envp_list[i], ':');
			tmp = path[0];
			path[0] = ft_substr(path[0], 5, ft_strlen(path[0]) - 5);
			free(tmp);
			return (path);
		}
		i++;
	}
	return (NULL);
}

pid_t	*msh_executor_malloc_pids(t_tree *tree)
{
	t_node *pipe_nd;
	int		cnt;
	pid_t	*pids;

	pipe_nd = tree->top;
	cnt = 0;
	while (pipe_nd)
	{
//		if (pipe_nd->type != T_PIPE)
//			return (NULL);
		pipe_nd = pipe_nd->right;
		cnt++;
	}
//	pids = (pid_t *)malloc(sizeof(pid_t) * cnt + 1);
	pids = ft_calloc(cnt + 1, sizeof(pid_t));
	return (pids);
}




/* fork wait*/
pid_t	*msh_executor_fork(t_node *pipe_nd, char **env_path, pid_t *pids)
{
	int		i;
	int		pipe_fd[2];
	int		fd[2];

	i = 0;
	fd[STD_IN] = STD_IN;
	fd[STD_OUT] = STD_OUT;
	while (pipe_nd)
	{
		if (pipe_nd->right)
		{
			pipe(pipe_fd);
			fd[STD_OUT] = pipe_fd[PIPE_IN];
			if (fd[STD_IN] > 2)
				close(fd[STD_IN]);
		}
		else
			fd[STD_OUT] = STD_OUT;
		pids[i] = fork();
		if (pids[i++] == 0)
		{
			close(pipe_fd[PIPE_OUT]);
		//	exit(msh_run_cmd(pipe_nd->left, fd[], env)); // command_not_found error
			printf("fd[STD_IN] %d, fd[STD_OUT] %d\n", fd[STD_IN], fd[STD_OUT]);
			msh_run_cmd(pipe_nd->left, fd, env_path);
			exit(0);
		}
		pipe_nd = pipe_nd->right;
		fd[STD_IN] = pipe_fd[PIPE_OUT];
		close(pipe_fd[PIPE_IN]);
	}

	return (pids);
}

int	msh_executor_wait_child(int *pids)
{
	int	i;
	int	statloc;

	i = 0;
	while (pids[i])
	{
		waitpid(pids[i], &statloc, 0); // NULL options. .
		i++;
	}
	// 마지막 statloc 해석
	// rtn (마지막 종료상태 ) ;
//	printf("bbbb\n");
	return (1);
}



/* run */
int	msh_run_cmd(t_node *cmd_nd, int fd[2], char **env_path)
{
	msh_redirection(cmd_nd->left, fd);
	// msh_set_wordsplit(); //// 더블쿼터 싱글쿼터
	// msh_run_builtin(); // built in 함수 있는지 검사 (있다면 해당함수명령 실행 exit) (없다면 return)
	// 만약 built in 함수라면 -> 여기서 종료하기.. 

	// $? 이거 어디다 저장해두나요..? ? ? ? ?  ?

	msh_run_simp_cmd(cmd_nd->right, env_path);
	// msh_run_simp_cmd(); // exeve(); envp_path
	return (1);
}

int	msh_run_simp_cmd(t_node *cmd_nd, char **env_path)
{
	char	*file_path;
	char	**cmd_argv;

	file_path = msh_get_cmd_path(cmd_nd->str1, env_path);
	cmd_argv = ft_split(cmd_nd->str2, ' ');
	execve(file_path, cmd_argv, env_path);
	return (-1);
}

char	*msh_get_cmd_path(char *cmd, char **env_path)
{
	char		*cmd_path;
	char		*tmp;
	int			i;
	struct stat	buf;

	i = 0;
	while (env_path[i])
	{
		tmp = ft_strjoin(env_path[i], "/");
		cmd_path = ft_strjoin(tmp, cmd);
		free(tmp);
	//	printf("cmd_path %s\n", cmd_path);
		if (stat(cmd_path, &buf) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}



	return (NULL);
}


//dup2

/* redirection */
int	msh_redirection(t_node *redirct_nd, int fd[2])
{
	while (redirct_nd)
	{
		redirct_nd = redirct_nd->left;
		if (msh_set_redirection(redirct_nd, fd) == -1)
			return (-1);
	}
	// 왜 안도ㅑ?
	if (dup2(STD_IN, fd[STD_IN]) == -1)
		return (-1);
	if (dup2(STD_OUT, fd[STD_OUT]) == -1)
		return (-1);
	return (1);
}

int	msh_set_redirection(t_node *redirct_nd, int	fd[2])
{
	// 만약 open에 실패 하였을때 오류 처리... ?
	// error "bash: a.txt: Permission denied" // a.txt = redirct_nd->str2

	if (ft_strncmp(redirct_nd->str1, "<", 2) == 0) // 입력 리다이렉션
		fd[STD_IN] = open(redirct_nd->str2, O_RDONLY);
	else if (ft_strncmp(redirct_nd->str1, ">", 2) == 0) // 출력
		fd[STD_OUT]= open(redirct_nd->str2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (ft_strncmp(redirct_nd->str1, ">>", 2) == 0) // 출력 붙여쓰기
		fd[STD_OUT] = open(redirct_nd->str2, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (ft_strncmp(redirct_nd->str1, "<<", 2) == 0)
		; // here doc.. 이거는 여기가 아니라 저 저 저어기 tokenzier에서 처리하는게 더 낫지 않나? 
	else
		return (-1);
	return (1);
}