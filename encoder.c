#include "header.h"

int	smaller1(t_data *data, t_node *node[SIZE])
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
	for (i = 0; i < data->tree_size; i++)
	{
		if (i != small1 && node[i]->charfrequency <= min && node[i]
			->characterindex !=127)
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

char	*encode_file(char **dictionary, unsigned char *string)
{
	int		i;
	int		size;
	char	*str_code;

	i = -1;
	size = 0;
	while (string[++i] != '\0')
		size = size + strlen(dictionary[(int)string[i]]);
	str_code = calloc (size + 1, sizeof(char));
	i = -1;
	while (string[++i] != '\0')
		strcat(str_code, dictionary[(int)string[i]]);
	return (str_code);
}

unsigned int		checking_bit(char byte, int i)
{
	unsigned char	bit_checker;

	bit_checker = (1 << i);
	return (byte & bit_checker);
}

void	output_info(FILE *file, t_data *data, int i)
{
	double	*shared_output;
	int		shmid;
	double	timestamp;
	double	final;
	double	output_size;
	double	output_size_2;

	final = clock();
	timestamp = (double)(final- data->initial) / CLOCKS_PER_SEC;
	fseek(file, 0L, SEEK_END);
	output_size = ftell(file);
	shmid = shmget((key_t)2346, sizeof(double), 0666 | IPC_CREAT);
	shared_output = (double *)shmat(shmid, NULL, 0);
	shared_output[OUT_SIZE] = output_size;
	shared_output[IN_SIZE] = data->input_size;
	shared_output[OUT_TIME] = timestamp;
	output_size_2 = (double)i;
	shared_output[OUT_STR_LEN] = output_size_2;
	shared_output[OUT_RATE] = (100 * output_size) / data->input_size;
}

void	compress(char str[], char *output_name, t_data *data)
{
	FILE			*file = fopen(output_name, "wb");
	int				i;
	unsigned char	byte;
	unsigned char	bit_checker;
	int				size;

	i = 0;
	byte = 0;
	size = 7;
	if (file)
	{
		while (str[i])
		{
			bit_checker = 1;
			if (str[i] == '1')
			{
				bit_checker = bit_checker << size;
				byte = byte | bit_checker;
			}
			size --;
			if (size < 0)
			{
				fwrite(&byte, sizeof(unsigned char), 1, file);
				byte = 0;
				size = 7;
			}
			i++;
		}
		if (size != 7)
			fwrite(&byte, sizeof(unsigned char), 1, file);
		output_info(file, data, i);
		fclose(file);
	}
	else
		printf("Error compressing");
}

void	encode_input(unsigned char *file_content, long size, t_node	**tree,
			char *output_name, t_data *data)
{
	t_node			*node[SIZE];
	int				i;
	long			frequency_table[SIZE];
	char			**dictionary;
	int				height;
	char			*str_code;
	long			*shared_memory;
	int				shmid;

	for(i = 0; i < SIZE; i++)
		frequency_table[i] = 0;
	i = 0;
	while (file_content[i] && (long)i < size)
	{
		frequency_table[(int)file_content[i]]++;
		i++;
	}
	shmid = shmget((key_t)5678, sizeof(int), 0666 | IPC_CREAT);
	shared_memory = (long *)shmat(shmid, NULL, 0);
	for(i = 0; i < SIZE; i++)
		shared_memory[i] = frequency_table[i];
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
	str_code = encode_file(dictionary, file_content);
	compress(str_code, output_name, data);
	multiple_free(NULL, NULL, str_code, NULL, data);
	destroy_pointers_char(dictionary);
}

long	open_input(FILE *input, long size, t_data *data)
{
	fseek(input, 0L, SEEK_END);
	data->input_size = ftell(input);
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

int main(int argc, char **argv)
{
	char			*output_name;
	unsigned char	*file_content;
	unsigned char	*file_content2;
	long			size;
	long			size2;
	FILE			*input;
	FILE			*input2;
	t_node			*tree;
	t_data			*data;

	setlocale(LC_ALL, "utf8");
	data = (t_data *)calloc(1,sizeof(t_data));
	data->initial = clock();
	if (argc != 3 && argc != 2)
		error_msg("Please type 2 or 3 arguments");
	if (argc == 2 || argc == 3)
	{
		size = 0;
		input = fopen(argv[1], "r");
		if (input == NULL)
			error_msg("Error: Can't open file ");
		size = open_input(input, size, data);
		file_content = malloc(sizeof(unsigned char) * (size + 1));
		read_close_input(input, file_content, size);
	}
	if (argc == 3)
	{
		size2 = 0;
		input2 = fopen(argv[2], "r");
		if (input2 == NULL)
			error_msg("Error: Can't open file");
		size2 = open_input(input2, size2, data);
		file_content2 = malloc(sizeof(unsigned char) * (size2 + 1));
		read_close_input(input2, file_content2, size2);
		file_content = ft_strjoin(file_content, file_content2);
		free(file_content2);
	}

	output_name = ft_strjoin_2(argv[1], ".labs");
	encode_input(file_content, size, &tree, output_name, data);
	multiple_free(file_content, NULL,  output_name, tree, NULL);
	printf(GRN "<<<Successful compression>>>\n" RESET);
	return (0);
}
