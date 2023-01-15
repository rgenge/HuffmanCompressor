# Data compression program using Huffman coding

#Introduction

- This is a data compression program, it consists on reducing the size using bit operations where we will get the file and use the Huffman Tree to build a whole new
dictionary that will be smaller than the ASCII table (which ASCII character has 8 bits), we will also use shared memory to send the compression information to the
decompressor.


- The program is divided in 2 parts, Encoding and Decoding.
- The Enconding part consists on :
- 1 -Reading the input and getting the characters and how many times they are repeated in the input.
- 2 -Creating a Huffman Tree with the chacaracters found in the file.
- 3 -Creatinga table using the tree and saving it to the shared memory, the table you be used to know where are the characters location in the tree.
- 4 -Using the table to generate a code.
- 5 -Using bit operations to generate a binary code since the code generated before was a string, and every string has 8 bits we have to transform it into binary in order to reduce the file size.
- 6 - Generanting a binary file.
- The Deconding part consists on :
- 1 -Doing the inverse operation to descompress the files using the tree and the frenquency table that is stored in the shared memory.
<img src="https://user-images.githubusercontent.com/82456124/212557288-dd04a680-64fa-49b5-a327-5f252fd6924e.png" width=80% height=80%>

#Encoder

This part of the program consists on a program to compress a file.
File compression consists on reducing the size of a file, in this case we will use the Huffman coding which keeps the data quality.
This algorithm is very popular and have many variants, but in this case we will keep the original one. It consists on making a binary tree to reduce the size of the
characters you find in the file.

<img src="https://user-images.githubusercontent.com/82456124/211985017-ef93cdb6-002a-4156-89d9-40859396567b.jpg" width=20% height=20%>
This image shows a basic Huffman Binary Tree with aaaabbbcc .
As you can see the Huffmann coding will result in that tree that is used to encode the files reducing it's size.
Instead of having the whole ASCII table with 8 bits each character, you will create your own coding table with less memory used due to the Huffman algorithm.
You can find a brief explanation about it there: https://en.wikipedia.org/wiki/Huffman_coding



#Decoder

This part of the program consists on a program to decompress a file.
File decompression consists on getting the original file, in this case we won't lose any data quality.
The programa will get the frequency table stored in the shared memory by the encoder and to the inverse bit operation.

#Shared Memory

In this program we use the sys/shm.h library and shared memory functions to share the information used for compress the file from the encoder to the decoder. Shared
memory is used to improve the data management of the programs, because you can use the same memory address and it is even faster to access the memory. In this program
we used the shared memory to save the compression rate, files size, and time. Those information are stored when compressing and then printed by the descompressor.
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
./encoder {filename1} {filename2}
```
3.  And to decompress a compressed file, use the `./decoder` command  and do not forget the generated file has .labs extension:
```
./decoder {filename}.labs
