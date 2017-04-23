/**************************************************************
DESCRIPTION:generate random words and write these words to 
			word.txt
**************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int word_len = 0;
int upper_low = 65;
int lowwer_low = 97;

// if letter_size = 5
// it will generate a-e or A-E letter.
int letter_size = 0;

int random_letter()
{
	return rand() % letter_size;
}
int random_word(
	char *word,
	int opt )
{	
	// minimum word'length is 3.
	int true_word_len = rand() % word_len + 3;
	int true_word_len1 = true_word_len;

	while ( true_word_len-- ) {
		char letter = 0;
	
		if ( opt == 0 ) {
			letter = random_letter() + lowwer_low;
		} else if ( opt == 1 ) {
			letter = random_letter() + upper_low;
		} else {
			int opt_case = rand() % 2;
			if ( opt_case == 0 )
				letter = random_letter() + lowwer_low;
			else 
				letter = random_letter() + upper_low;
		}
		word[true_word_len] = letter;
	}

	return true_word_len1;
}
void gen_word(
	int fd,
	int word_num,
	int opt )
{
	char word[20] = {0};
	int true_word_len = 0;

	while ( word_num-- ) {
		memset( word, 0, 20);
		true_word_len = random_word( word, opt );
		word[true_word_len] = '\n';
		write( fd, word, true_word_len + 1 );
	}
}
int main(
	int argc,
	char **argv )
{
	srand((int)time(NULL));
	if ( argc != 5 ) {
		printf("please input "
				"word's length & "
				"words' number & "
				"word's range & "
				"gen_case(0:lowwer case,1:upper case,other:both\n");
		exit( 0 );
	}

	word_len = atoi( argv[1] );
	int word_num = atoi( argv[2] );
	letter_size = atoi( argv[3] );
	int opt = atoi( argv[4] );

	int fd = open("word.txt", O_RDWR | O_TRUNC, 0777);

	gen_word( fd, word_num, opt );

	close( fd );

	return 0;
}
