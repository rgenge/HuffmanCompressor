#ifndef HEADER_H
# define HEADER_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <fcntl.h>
# include <string.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <time.h>
# include <locale.h>
# define OUT_SIZE 0
# define OUT_TIME 1
# define OUT_RATE 2
# define IN_SIZE 3
# define OUT_STR_LEN 4
# define STDIN 0
# define STDOUT 1
# define IN 0
# define OUT 1
# define ERROR 1
# define SIZE 256
# define LONG_MAX 9223372036854775807
# define GRN   "\x1B[32m"
# define RESET "\x1B[0m"

typedef struct s_node
{
	int				characterindex;
	unsigned char	character;
	long			charfrequency;
	struct s_node	*left, *right;
}				t_node;

typedef struct s_data
{
	int		file_size;
	int		tree_size;
	int		i;
	double	input_size;
	double	initial;
}				t_data;

/*
Function that find the least frequent character in the frequency table.
@param[in] *data The struct data.
@param[in] *node[SIZE] The struct node.
*/
int				smaller1(t_data *data, t_node *node[SIZE]);
int				smaller1(t_data *data, t_node *node[SIZE]);

/*
This functions that create the tree, by reading the nodes and always adding the
smaller1 and smaller2 frequency to make a new node untill you have only one
node with all chars and frequencies.
@param[in] *data The struct data.
@param[in] *node[SIZE] The whole nodes array.
@param[in] *tree Huffman tre.
*/
void			create_subtrees(t_data *data, t_node *node[SIZE], t_node
					**tree);

/*
This function will generate the dictionary by finding every character found in
the tree and atributte a code for it.
@param[in] **dictionary The array with all codes.
@param[in] height The height of the tree.
@param[in] *tree Huffman tre.
@param[in] *data Generated tree.
@param[in] path The concatenated path of the code.
*/
void			fill_dictionary(char **dictionary, int height, t_node *tree,
					t_data *data, char *path);

/*
Function to get the tree height so we can use it to generate the dictionary
@param[in] *tree Huffman tre.
@param[in] *data Generated tree.
*/
int				tree_height(t_node *tree, t_data *data);

/*
Generates the code for the string using it's dictionary
@param[in] **dictionary The array with all codes.
@param[in] *string Generated code string.
*/
char			*encode_file(char **dictionary, unsigned char *string);

/*
It will shift you binary to the left i times and them check if it is a 0 or 1
@param[in] byte The byte you will check.
@param[in] i The number of times you will shift.
*/
unsigned int	checking_bit(char byte, int i);

/*
Decompress the file using bit operations running the tree and finding which letter belongs to the code found.
@param[in] output_name Filename
@param[in] *tree Huffman tre.
*/
void	decompress(char *output_name, t_node *tree, t_data *data);

/*
Compress the file reading the genrated string and then using bit operations to
tranform the code in binary to reduce the file size.
@param[in] output_name Filename
@param[in] str The string that will be compressed.
@param[in] data Data used to get clock.
*/
void			compress(char str[], char *output_name, t_data *data);

/*
Create a frequency table and fill it with the chacaracters frequency in the
file content, after that we create the nodes to use to create the tree later
on, then we create a dictionary which you atributte a code to each character
then we finalize the process by compressing it using bit operations.
@param[in] *file_content The file content.
@param[in] size The file size.
@param[in] *tree Huffman tre.
@param[in] output_name The name the file will have when generated.
@param[in] data data containing useful shared variables.
*/
void			encode_input(unsigned char *file_content, long size, t_node	**tree, char *output_name, t_data *data);

/*
Create a frequency table from the shared memory and fill it with the
chacaracters frequency in the file content, after that we create the nodes
used to create the tree later on, then we create a dictionary which you
atributte a code to each character then we finalize the process by
decompressing using bit operation.
@param[in] *file_content The file content.
@param[in] size The file size.
@param[in] *tree Huffman tre.
@param[in] output_name The name the file will have when generated.
@param[in] data data containing useful shared variables.
*/
void	decode_input(t_node	**tree, char *output_name, t_data *data);

/*
Opening input, getting beggining and end of a string and it's size
@param[in] size Size of the file content.
@param[in] *data Struct wit some variables used in multiple functions.
*/
void			fill_dictionary(char **dictionary, int height, t_node *tree,
					t_data *data, char *path);

					/*
Reading the file and saving it into the file_content string.
@param[in] *input The file.
@param[in] *file_content The file content.
@param[in] size The file size.
*/
void			read_close_input(FILE *input, unsigned char *file_content, long
					size);

/*Those funcitions name are self explanatory*/
char			*ft_substr(char const *s, unsigned int start, size_t len);
int				error_msg(char *message);
void			ft_putstr_fd(char *s, int fd);
void			free_tree(t_node *tree);
void			destroy_pointers_char(char **p);
void			multiple_free(unsigned char *var1, unsigned char *var2,
					char *var3, t_node *tree, t_data *data);
unsigned char	*ft_strjoin(unsigned char *s1, unsigned char *s2);
/*
Copies bytes from a source memory area to a destination memory area,
where both areas may not overlap.
@param[out] s1 The memory area to join to.
@param[in]  s2  The memory area to copy from.
*/
char			*ft_strjoin_2(char *s1, char *s2);
#endif
