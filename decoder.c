#include "header.h"

int	smaller1 (t_data *data, t_node *node[SIZE])
{
	int		i;
	long	min;
	int		minindex;

	min = LONG_MAX;
	for (i = 0; i < data->tree_size; i++)
	{
		if (node[i]->charfrequency < min && node[i]->characterindex != 127)
		{
			min = node[i]->charfrequency;
			minindex = i;
		}
	}
	return (minindex);
}

int	smaller2 (t_data *data, t_node *node[SIZE], int small1)
{
	int		i;
	long	min;
	int		minindex;

	min = LONG_MAX;
	minindex = data->i;
	for(i = 0; i < data->tree_size; i++)
	{
		if(i != small1 && node[i]->charfrequency <= min && node[i]
			->characterindex != 127)
		{
			min = node[i]->charfrequency;
			minindex = i;
		}
	}
	return (minindex);
}

void	create_subtrees(t_data *data, t_node *node[SIZE], t_node	**tree)
{
	t_node		*temp;
	int			small1;
	int			small2;

	small1 = 0;
	small2 = 0;
	data->i = data->tree_size;
	while (data->i > 1)
	{
		small1 = smaller1(data, node);
		small2 = smaller2(data, node, small1);
		temp = node[small1];
		node[small1] = malloc(sizeof(t_node));
		node[small1]->charfrequency = temp->charfrequency + node[small2]
			->charfrequency;
		node[small1]->character = 127;
		node[small1]->left = node[small2];
		node[small1]->right = temp;
		node[small2]->characterindex = 127;
		data->i--;
	}
	*tree = node[small1];
}

void	fill_dictionary(char **dictionary, int height, t_node *tree, t_data
			*data, char *path)
{
	char	left[height];
	char	right[height];

	if (tree->left == NULL && tree->right == NULL)
		strcpy(dictionary[(int)tree->character], path);
	else
	{
		strcpy(left, path);
		strcpy(right, path);
		strcat(left, "0");
		strcat(right, "1");
		fill_dictionary(dictionary, height, tree->left, data, left);
		fill_dictionary(dictionary, height, tree->right, data, right);
	}
}

int	tree_height(t_node *tree, t_data *data)
{

	int left;
	int right;

	if (tree == NULL)
		return (-1);
	else
	{
		left = tree_height(tree->left, data) + 1;
		right = tree_height(tree->right, data) + 1;
		if (left > right)
			return left;
		else
			return right;
	}
}

unsigned int		checking_bit(char byte, int i)
{
	unsigned char	bit_checker;

	bit_checker = (1 << i);
	return (byte & bit_checker);
}

void	decompress(char *output_name, t_node *tree, t_data *data)
{
	FILE			*file = fopen(output_name, "rb");
	unsigned char	byte;
	int				i;
	t_node			*aux;
	FILE			*decompressed_file;
	char			*decompressed_f_name;
	double			*shared_output;
	int				shmid;
	int				str_len;

	data->i = 0;
	shmid = shmget((key_t)2346, sizeof(double), 0666 | IPC_CREAT);
	shared_output = (double *)shmat(shmid, NULL, 0);
	str_len = (int)shared_output[OUT_STR_LEN];
	decompressed_f_name = ft_substr (output_name, 0, strlen(output_name) - 5);
	decompressed_file = fopen(decompressed_f_name, "w+");
	aux = tree;
	if (file)
	{
		while (fread(&byte, sizeof(unsigned char), 1, file) && data->i < str_len)
		{
			for(i = 7; i >= 0; i--)
			{
				if (data->i < str_len)
				{
					if (checking_bit(byte, i))
					{
						data->i++;
						aux = aux->right;
					}
					else
					{
						data->i++;
						aux = aux->left;
					}
					if (aux->left == NULL && aux->right == NULL)
					{
						fwrite(&aux->character, 1, 1, decompressed_file);
						aux = tree;
					}
				}
			}
		}
		fclose(decompressed_file);
		fclose(file);
	}
	else
		printf("Error decompressing");
}

void	decode_input(t_node	**tree, char *output_name, t_data *data)
{
	t_node			*node[SIZE];
	int				i;
	long			frequency_table[SIZE];
	char			**dictionary;
	int				height;
	long			*shared_memory;
	int				shmid;

	shmid = shmget((key_t)5678, sizeof(int), 0666);
	shared_memory = (long *)shmat(shmid, NULL, 0);
	for(i = 0; i < SIZE; i++)
		frequency_table[i] = shared_memory[i];
	data->tree_size = 0;
	i = 0;
	while (i < SIZE)
	{
		if (frequency_table[i] > 0)
		{
			node[data->tree_size] = malloc(sizeof(t_node));
			node[data->tree_size]->charfrequency = frequency_table[i];
			node[data->tree_size]->characterindex = i;
			node[data->tree_size]->character = i;
			node[data->tree_size]->left = NULL;
			node[data->tree_size]->right = NULL;
			data->tree_size++;
		}
		i++;
	}
	create_subtrees(data, node, tree);
	height = tree_height(*tree, data) + 1;
	dictionary = malloc(sizeof(char*) * SIZE);
	for (i = 0; i < SIZE; i++)
		dictionary[i] = calloc (height, sizeof(char));
	fill_dictionary(dictionary, height, *tree, data, "");
	decompress(output_name, *tree, data);
	multiple_free(NULL, NULL, NULL, NULL, data);
	destroy_pointers_char(dictionary);
}

long	open_input(FILE *input, long size)
{
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);
	return (size);
}

void	read_close_input(FILE *input, unsigned char *file_content, long size)
{
	file_content[size] = '\0';
	fread(file_content, sizeof(char), size, input);
	fclose(input);
}


void	output_info_print(void)
{
	double	*shared_output;
	int		shmid;

	shmid = shmget((key_t)2346, sizeof(double), 0666 | IPC_CREAT);
	shared_output = (double *)shmat(shmid, NULL, 0);
	printf("Out file size:    %.f bytes\n", shared_output[OUT_SIZE]);
	printf("In file size:     %.f bytes\n", shared_output[IN_SIZE]);
	printf("Compression time: %g s \n", shared_output[OUT_TIME]);
	printf("Compression rate:    %.f %% \n", shared_output[OUT_RATE]);
}

int main(int argc, char **argv)
{
	char			*output_name;
	char			*output_name_check;
	unsigned char	*file_content;
	long			size;
	FILE			*input;
	t_node			*tree;
	t_data			*data;

	setlocale(LC_ALL, "utf8");
	if (argc != 3 && argc != 2)
		error_msg("Please type 2 or 3 arguments");
	output_name_check = ft_substr(argv[1], (strlen(argv[1]) - 4), 5);
	if (strncmp(output_name_check, "labs", 5))
		error_msg("Error: Can't open file file must be .labs");
	if (argc == 2 || argc == 3)
	{
		size = 0;
		input = fopen(argv[1], "r");
		if (input == NULL)
			error_msg("Error: Can't open file");
		size = open_input(input, size);
		output_info_print();
		file_content = malloc(sizeof(char) * (size + 1));
		read_close_input(input, file_content, size);
	}
	data = (t_data *)calloc(1,sizeof(t_data));
	output_name = strdup(argv[1]);
	decode_input(&tree, output_name, data);
	multiple_free(file_content, NULL,  output_name, tree, NULL);
	free(output_name_check);
	printf(GRN "<<<Successful decompression>>>\n"    RESET);
	return (0);
}
