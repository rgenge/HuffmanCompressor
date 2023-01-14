#include "header.h"

/*Function that find the least frequent character*/
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
	printf("small1 %d \t %ld  %c\n",minindex, node[minindex]->charfrequency, 					node[minindex]->character);//delt//delt//delt//delt
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
			->characterindex !=127)
		{
			min = node[i]->charfrequency;
			minindex = i;
		}
	}
	printf("small2 %d \t %ld  %c\n",minindex, node[minindex]->charfrequency, 					node[minindex]->character);//delt//delt//delt//delt//delt
	return (minindex);
}

/*This is the process that will in fact create the tree, by reading the nodes and always adding the smaller1 and smaller2 frequency to make a new node untill you have only one node*/

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

/*This function will run the dictionary and find every character found in the file content and atributte a code for it */
void	fill_dictionary(char **dictionary, int height, t_node *tree, t_data *data, char *path)
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

/*Function to get the tree height so we can use it in the dictionary*/
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
/*This process generates the code for the string using it's dictionary*/
char	*encode_file(char **dictionary, char *string)
{
	int		i;
	int		size;
	char	*str_code;

	i = -1;
	size = 0;
	while (string[++i] != '\0')
		size = size + strlen(dictionary[(int)string[i]]);
	printf("size: %d\n", size); //delt//delt//delt//delt
	str_code = calloc (size + 1, sizeof(char));
	i = -1;
	while(string[++i] != '\0')
	{
		strcat(str_code, dictionary[(int)string[i]]);
		printf("%c %s\n",(int)string[i],   dictionary[(int)string[i]]); //delt
	}
	return (str_code);
}

/*The inverse process will get a code and generate a string*/
char	*decode_file(t_node *tree, char *string)
{
	int		i;
	char	*str_decode;
	char	temp[2];
	t_node	*aux;

	i = -1;
	aux = tree;
	str_decode = calloc (strlen(string), sizeof(char));
	while (string[++i] != '\0')
	{
		if (string[i] == '0')
			aux = aux->left;
		else
			aux = aux->right;
		if (aux->left == NULL && aux->right == NULL)
		{
			temp[0] = aux->character;
			temp[1] = '\0';
			strcat (str_decode, temp);
			aux = tree;
		}
	}
	return (str_decode);
}

unsigned int		checking_bit(char byte, int i)
{
	unsigned char	bit_checker;

	bit_checker = (1 << i);
	return (byte & bit_checker);
}
/*This process will do the inverse using bit operations running the tree and finding which letter belongs to the code found*/
void	decompress(char *output_name, t_node *tree)
{
	FILE	*file = fopen(output_name, "rb");
	unsigned char	byte;
	int		i;
	t_node	*aux;

	aux = tree;
	if (file)
	{
		while (fread(&byte, sizeof(unsigned char), 1, file))
		{
			for(i = 7; i >= 0; i--)
			{
				if (checking_bit(byte, i))
					aux = aux->right;
				else
					aux = aux->left;
				if (aux->left == NULL && aux->right == NULL)
				{
					printf("%c", aux->character);
					aux = tree;
				}
			}
		}
		fclose(file);
	}
	else
		printf("Error decompressing");

}
/*This process will compress the file reading the genrated string and then using bit operations to tranform the code in binary to reduce the file size*/
void	compress(char str[], char *output_name)
{
	FILE	*file = fopen(output_name, "wb");
	int		i;
	unsigned char	byte;
	unsigned char	bit_checker;
	int	size;

	i = 0;
	byte = 0;
	size = 7;
	if (file)
	{
		while (str[i])
		{
//			printf("comp: %c\t", str[i]);
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
		fclose(file);
	}
	else
		printf("Error compressing");
}

/*First of all we have to create a frequency table and fill it with the chacaracters frequency in the file content, after that we create the nodes to use to create the tree later on, then we create a dictionary which you atributte a code to each character then we finalize the process*/
void	encode_input(char *file_content, long size, t_node	**tree, char *output_name)
{
	t_node			*node[SIZE];
	int				i;
	t_data			*data;
	long			frequency_table[SIZE];
	char			**dictionary;
	int				height;
	char			*str_code;
	char			*str_decode;

	data = (t_data *)calloc(1,sizeof(t_data));
	for(i = 0; i < SIZE; i++)
		frequency_table[i] = 0;
	i = 0;
	while (file_content[i] && (long)i < size)
	{
		frequency_table[(int)file_content[i]]++;
		i++;
	}
	int	*shared_memory;
	int	shmid;
	shmid = shmget((key_t)2345, sizeof(int), 0666|IPC_CREAT);
	shared_memory = (int *)shmat(shmid, NULL, 0);
	for(int x = 0; x < SIZE; x++)
		shared_memory[x] = frequency_table[x];
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
	str_decode = decode_file(*tree, str_code);
	compress(str_code, output_name);
//	decompress(output_name, *tree);

	// for(int j =0; j <data->tree_size; j++)
	// printf("%d   %ld\t %c \n",j,  node[j]->charfrequency, node[j]
	// 	->characterindex);
	// printf("%s  \t \n", str_code);
	// printf("%s  \t \n", str_decode);
	// for(int j =0; j < SIZE; j++)
	// 	printf("%d %s  \t \n",j, dictionary[j]);

	multiple_free(str_code, str_decode,NULL, NULL, data);
	destroy_pointers_char(dictionary);
}

/*Opening input, getting beggining and end of a string and it's size*/
long	open_input(FILE *input, long size)
{
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	fseek(input, 0, SEEK_SET);
	return (size);
}

/*Reading the file and saving it into the file_content string*/
void	read_close_input(FILE *input, char *file_content, long size)
{
	file_content[size] = '\0';
	fread(file_content, sizeof(char), size, input);
	fclose(input);
}

/*The main function will read the input which will be a file, after reading it it will store it and we will read the file and start the encoding process*/
int main(int argc, char **argv)
{
	char	*output_name;
	char	*file_content;
	char	*file_content2;
	long	size;
	long	size2;
	FILE	*input;
	FILE	*input2;
	t_node	*tree;

	if (argc != 3 && argc != 2)
		error_msg("Please type 2 or 3 arguments");
	if (argc == 2 || argc == 3)
	{
		size = 0;
		input = fopen(argv[1], "r");
		if (input == NULL)
			error_msg("Error: Can't open file");
		size = open_input(input, size);
		file_content = malloc(sizeof(char) * (size + 1));
		read_close_input(input, file_content, size);
	}
	if (argc == 3)
	{
		size2 = 0;
		input2 = fopen(argv[2], "r");
		if (input2 == NULL)
			error_msg("Error: Can't open file");
		size2 = open_input(input2, size2);
		file_content2 = malloc(sizeof(char) * (size2 + 1));
		read_close_input(input2, file_content2, size2);
		file_content = ft_strjoin(file_content, file_content2);
		free(file_content2);
	}
	output_name = ft_strjoin_2(argv[1], ".labs");
	encode_input(file_content, size, &tree, output_name);
	multiple_free(file_content, NULL,  output_name, tree, NULL);
	return (0);
}
