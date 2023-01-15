#include "header.h"

void	ft_putstr_fd(char *s, int fd)
{
	int		i;
	char	c;

	i = 0;
	while (s[i])
	{
		c = s[i];
		write(fd, &c, 1);
		i++;
	}
}

void	free_tree(t_node *tree)
{
	if (tree == NULL)
		return ;
	free_tree(tree->right);
	free_tree(tree->left);
	free(tree);
}

void	multiple_free(unsigned char *var1, unsigned char *var2, char *var3, t_node
			*tree, t_data *data)
{
	if (var1)
		free(var1);
	if (var2)
		free(var2);
	if (var3)
		free(var3);
	if (tree)
		free_tree(tree);
	if (data)
		free(data);
}
void	destroy_pointers_char(char **p)
{
	int	i;

	i = 0;
	while (p != NULL && p[i] != NULL && i < SIZE)
		free(p[i++]);
	free(p);
}

int	error_msg(char *message)
{
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
	exit(ERROR);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

unsigned char	*ft_strjoin(unsigned char *s1, unsigned char *s2)
{
	size_t			i;
	size_t			j;
	size_t			len;
	unsigned char	*dest;

	i = 0;
	j = 0;
	len = 0;
	if (!s1)
	{
		s1 = (unsigned char *)malloc(1 * sizeof(unsigned char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	while (s1[len])
		len++;
	while (s2[j])
		j++;
	len = len + j;
	j = 0;
	dest = malloc((len + 1) * sizeof(unsigned char));
	if (dest == NULL)
		return (NULL);
	if (s1)
		while (s1[i] != '\0')
			dest[i++] = s1[j++];
	j = 0;
	while (s2[j])
		dest[i++] = s2[j++];
	dest[i] = '\0';
	free(s1);
	return (dest);
}

char	*ft_strjoin_2(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	char	*dest;

	i = 0;
	j = 0;
	if (!s1)
	{
		s1 = (char *)malloc(1 * sizeof(char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	dest = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	if (s1)
		while (s1[i] != '\0')
			dest[i++] = s1[j++];
	j = 0;
	while (s2[j])
		dest[i++] = s2[j++];
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*dest;
	size_t	lens;

	i = 0;
	if (s)
		lens = ft_strlen(s);
	else
		return (NULL);
	if (start >= lens)
		dest = (char *)malloc(1);
	else if (len >= lens)
		dest = (char *)malloc(lens - start + 2);
	else
		dest = (char *)malloc(lens + 1);
	if (!dest)
		return ((char *)s);
	while (start < lens && i < len)
	{
		dest[i] = s[start];
		i++;
		start++;
	}
	dest [i] = '\0';
	return (dest);
}
