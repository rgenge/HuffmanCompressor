# Labs 4º Edição

## Desafio

O desafio consiste no desenvolvimento de uma aplicação de análise e compressão de dados. Essa aplicação utilizará o algoritmo de compressão Huffman e poderá receber múltiplos dados onde, uma vez que comprimido deve ser descomprimido e coletar informações pertinentes para a análise do processo e dado.

Para isso, você criará dois programas: `encoder` e `decoder`. O encoder receberá o dado a ser comprimido e exibirá suas informações vindas do decoder. Já, o decoder descomprimirá o dado e irá enviá-lo ao encoder com suas informações, onde será exibido. Os programas devem se comunicar utilizando *shared memory operations*.

A linguagem C será utilizada para o desenvolvimento e não há bibliotecas externas permitidas.

## É necessário

- Que existam dois programas `encoder` e `decoder` que se comuniquem via memória compartilhada.
- Que o `encoder` possa receber, no mínimo, múltiplos textos como dado e comprimi-lo. Caso mais de um texto seja enviado, devem ser comprimidos juntos, resultando em apenas um único dado.
- Que o algoritmo de Huffman seja implementando em sua totalidade e que o processo de compressão e descompressão ocorram utilizando-o.
-  Que o `decoder` possa descomprimir e enviar as seguintes informações ao `encoder`: dado descomprimido, quantidade de bits ou bytes totais, quantidade de bits ou bytes comprimidos e o tempo da operação de descompressão.
- O `encoder` exibir as informações recebidas pelo `decoder`.

### O que será avaliado

- Código bem escrito e limpo.
- A documentação do seu código.
- Ferramentas que foram utilizadas e por quê.
- Sua criatividade e capacidade de lidar com problemas diferentes.
- Alinhamento do seu projeto com a proposta.

### O mínimo necessário

- README.md com a documentação contendo informações do projeto.

### Bônus

Os itens a seguir não são obrigatórios, mas são funcionalidades que darão mais valor ao seu desafio.

- Compressão de múltiplos arquivos binários.
- Criptografia do dado comprimido, utilizando senha fornecida pelo usuário.
- Verificação de integridade do arquivo comprimido.
- Opção para escolher mais outro algoritmo de compressão a ser utilizado.
- Tempo de descompressão (`decoder`) abaixo da média dos candidatos.
- Cuidados especiais com otimização e padrões de código.
- Uso de ferramentas externas para planejamento nas etapas de desenvolvimento.

















# Data compression program using Huffman coding

#Introduction

- The program is divided in 2 parts, Encoding and Decoding. 
- The Enconding part consists on :
- 1 -Reading the input and getting the characters and how many times they are repeated in the input.
- 2 -Creating a Huffman Tree with the chacaracters found in the file.
- 3 -Creatinga table using the tree, the table you be used to know where are the characters location in the tree.
- 4 -Using the table to generate a code.
- 5 -Using bit operations to generate a binary code since the code generated before was a string, and every string has 8 bits we have to transform it into binary in order to reduce the file size.
- 6 - Generanting a binary file.
- The Deconding part consists on :
- 1 -Doing the inverse operation to descompress the files using the tree.

#Encoder

This part of the program consists on a program to compress a file.
File compression consists on reducing the size of a file, in this case we will use the Huffman coding which keeps the data quality.
This algorithm is very popular and have many variants, but in this case we will keep the original one. It consists on making a binary tree to reduce the size of the characters you find in the file.

<img src="https://user-images.githubusercontent.com/82456124/211985017-ef93cdb6-002a-4156-89d9-40859396567b.jpg" width=20% height=20%>
This image shows a basic Huffman Binary Tree with aaaabbbcc .
As you can see the Huffmann coding will result in that tree that is used to encode the files reducing it's size.
Instead of having the whole ASCII table with 8 bits each character, you will create your own coding table with less memory used due to the Huffman algorithm.
You can find a brief explanation about it there: https://en.wikipedia.org/wiki/Huffman_coding

#Shared Memory

In this program we use the sys/shm.h library and shared memory functions to share the information from the encoder to the decoder. Shared memory is used to improve the data management of the programs, because you can use the same memory address and it is even faster to access the memory.
You can find more about it here : https://man7.org/linux/man-pages/man0/sys_shm.h.0p.html


## How to use it?
1. Compile with `make` using your, it will generate 2 programs the encoder and the decoder:
```
make
```
2. After running make, you can use `./encoder` command below to compress the file, it will generate a file with .labs extension.
* To compress one file use:
```
./encoder {filename}
```
* To compress 2 files use:

```
./encoder {filename1} {filename2} ...
```
3.  And to decompress a compressed file, use the `./decoder` command below without .labs exntension:
```
./decoder {filename}
