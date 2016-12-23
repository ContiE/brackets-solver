#include <stdio.h>
#include <stdlib.h>
#define STEP_SIZE 16

char* read(int *);
char* analyze(char *, int);

int main(void)
{
	char *seq,
		 *seq_correct;
	int length = 0;
	
	do
		seq = read(&length);
	while(seq == NULL);
	
	seq_correct = analyze(seq, length);
	printf("Correct longest prefix: %s\n", seq_correct);

	free(seq);
	free(seq_correct);

	return 0;
}

/**
  read(): Funcion to read the sequence of brackets.
  */
char* read(int *size)
{
    int i = 0,
        valid = 1,
        num_alloc = 1;	/* counter of memory allocation */
    char *str = NULL,
         *tmp = NULL;
    char input;
    
	/* first attempt to allocate memory with default space, this amount is defined in STEP_SIZE */
    str = malloc((STEP_SIZE + 1) * sizeof(char));
    if (str != NULL) 
	{
		printf("Type the sequence of brackets to be analyzed: ");
    	do {
        	scanf("%c", &input);
			
			i++;
        	str[i-1] = input;
			
			/* check that the character is a brackets and avoid ENTER as input without other characters */
        	if((input != '(' &&
				input != ')' &&
   	 			input != '[' &&
            	input != ']' &&
            	input != '{' &&
            	input != '}' &&
            	input != '\n') ||
            	((i == 1) &&
            	(input == '\n')))
				valid = 0;
        	
			/* the counter (i) has reached the number of characters allowed by initial allocation, so more memory is needed */
        	if (i == ((num_alloc * STEP_SIZE))) 
			{
			 	/**
				 increase the number of allocations (num_alloc) and realloc memory by adding another STEP_SIZE,
				 to prevent problems is better reallocate memory in another pointer and "copy" it on the main one
            	 */
				num_alloc++;
				tmp = (char *)realloc(str, ((num_alloc * STEP_SIZE) + 1) * sizeof(char));
				if (tmp != NULL)
					str = tmp;
				else 
				{
					input = '\n';
					valid = 0;
					free(str);
				}
        	}	
		} while (input != '\n');
	}
	else
		valid = 0;
	
	str[i-1] = '\0';
	/* TODO: *size = num_alloc; */
	*size = i;	
	
	/* if valid is set to 1 return the main pointer else NULL */
    return (valid) ? str : NULL;
}

/**
 analyze(): Funcion to analyze the sequence and get the correct longest prefix
 */
char* analyze(char *str, int l) 
{
    char *tmp,
		 *out;
	int i = 0,
		j = 0,
		go = 1;
	char bracket;
	
	/**
	 the entire sequence can be entered correctly, so:
	 - is needed the same amount of allocated memory for reading the sequence
	 - are needed two pointers, one to run the algorithm and the other to build the prefix 
	 */
	l *= STEP_SIZE; 
	tmp = (char *)malloc(l * sizeof(char));
	out = (char *)malloc(l * sizeof(char));
	
	/**
	 stepping conditions:
	 - str[i] must contains a character 
	 - go = 1 (flag) 
	 */
	while(str[i] && 
		 (go == 1))
	{	
		switch(str[i])
		{
			case '(':
			case '[':
			case '{':
				/* each instance of open brackets is copied in both arrays */
				tmp[j++] = str[i];
				out[i] = str[i];
				break;
			default:
				/* close brackets, build its open parenthesis using ASCII encoding */
				bracket = (str[i] == ')') ? str[i] - 1 : str[i] - 2;
				
				/** 
				 verify if the last brackets in $(tmp) is equal to $(bracket) , $(j) is decreased because
				 after each copy of open brackets it was increased, if the check fails $(go) is set to 0
				 */
				if(bracket == tmp[--j]) 
					out[i] = str[i];
				else
					go = 0;
				break;
		}
		i++;
	}
	
	out[i] = '\0';
	free(tmp);

	return out;
}
