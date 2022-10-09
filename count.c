#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 30

int main (int argc, char *argv[]) {
	//retry if user gives invalid format
	if (argc != 2) {
		printf("Please insert correct number or arguments.\n");
		return 1;
	}

	//establish variables
	FILE *book;
	char word[MAX_WORD_LENGTH];
	int wordNum = 0;

	//open file for reading
	book = fopen(argv[1], "r");

	//while you can read strings from the file, increment the counter for each word
	while (fscanf(book, "%s", word) == 1)
	{
		wordNum++;
	} 
	
	//print total words from book
	printf("%i total words.\n", wordNum);

	//close file and program
	fclose(book);
	return 1;
}



