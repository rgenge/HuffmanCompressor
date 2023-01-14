#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#ifndef HEADER_H
# define HEADER_H

#define STDIN 0
#define STDOUT 1
#define IN 0
#define OUT 1
#define ERROR 1
#define SIZE 256
#define LONG_MAX 9223372036854775807


typedef struct	s_node
{
	int		characterindex;
	char	character;
	long	charfrequency;
	struct	s_node *left, *right;
}				t_node;

typedef struct	s_data
{
	int		file_size;
	int		tree_size;
	int		i;
}				t_data;

/*utils.c*/
int		openfile(char *filename, int mode);
int		error_msg(char *message);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strjoin_2(char *s1, char *s2);
void	ft_putstr_fd(char *s, int fd);
void	free_tree (t_node *tree);
void	destroy_pointers_char(char **p);
void	multiple_free(char *var1, char *var2, char *var3, t_node *tree, t_data *data);

/*
Copies bytes from a source memory area to a destination memory area,
where both areas may not overlap.
@param[out] s1 The memory area to join to.
@param[in]  s2  The memory area to copy from.
*/
char	*ft_strjoin_2(char *s1, char *s2);
#endif
