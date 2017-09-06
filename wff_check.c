#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_OPERATOR -1
#define FALSE 0
#define TRUE 1

typedef int boolean;
typedef struct Statement Statement;


// Represents either a compound statement or an atomic statement
struct Statement{
	int operator_index;
	Statement *left;
	Statement *right;
	char string[25];
};

char operators[5] = { '~', '.', 'v', '-', '<' };
/* negation, conjunction, disjunction, conditional, biconditional
 * actually entered into command line as ~, ., v, ->, <-> */


/* find_operators returns the index of a statement->string that contains the main
logical opperator of that string. The index is needed for reference instead of the
char representing the operator in the event that the operator shows up in the string
more than once. If there is no operator, then the statement is atomic and returns
NO_OPERATOR. */
int find_operators(Statement *statement)
{

	/* level denotes how many nested parenthesis we have entered. It will be used
	to track the scope of each operation within the entire formula. If level == 1,
	then we are within the scope of the two outer parenthesis and it's possible
	that any char here could be an operator. So, when level == 1, find_operators
	will check the current char against the operator array, and if a match is
	found, it will return the index of statement->string where the match was found. */
	int str_index, op_index;
	int level = 0;

	/* If a tilde resides in the begginning of the statement->string, then the
	relevant operator in this string has to be the tilde operator. So, return the
	index of the tilde, aka 0. */
	if (statement->string[0] == '~') {
		return 0;
	}

	for (str_index = 0; str_index < strlen(statement->string); str_index++) {
		if (statement->string[str_index] == '(' || statement->string[str_index] == '[')
			level++;

		if (statement->string[str_index] == ')' || statement->string[str_index] == ']')
			level--;

		if (level == 1) {

			for (op_index = 1; op_index < 5; op_index++) {

				if (statement->string[str_index] == operators[op_index])
					return str_index;
			}
		}

	}

	return NO_OPERATOR;
}


/* A simple function to create a statement struct using a pointer to a pointer to
a statement. */
void create_statement(Statement **new)
{
	*new = (Statement *)malloc(sizeof(struct Statement));
	(*new)->left = NULL;
	(*new)->right = NULL;
	(*new)->operator_index = NO_OPERATOR;
}

/* create_left will save all chars left of a statement's operator into a new
statement->string. It will not save outside parenthesis. Then it will return
the new statement. We will have it return to a statement->left. */
Statement *create_left(Statement *statement)
{
	Statement *new;
	create_statement(&new);

	int i;
	for (i = 1; i < strlen(statement->string); i++) {
		if (i == statement->operator_index) {
			new->string[i] = '\0';
			break;
		}
		new->string[i-1] = statement->string[i];
	}

	return new;
}

/* create_right will save all chars right of a statement's operator into a new
statement->string. It will not save outside parenthesis, unless statement->string[0]
is a '~'. Then it will return the new statement. We will use it to return to a
statement->right. */
Statement *create_right(Statement *statement)
{
	Statement *new;
	create_statement(&new);

	int current, start = statement->operator_index;

	/* If the operator is written as "->" or "<->", then we must offset
	our index to skip the rest of the string that represents an operator. */
	if (statement->string[start] == '-')
		start++;
	if (statement->string[start] == '<')
		start += 2;
	start++;
	
	current = start;

	if (statement->string[statement->operator_index] != '~') {
		for (current; current < strlen(statement->string) - 1; current++)
			new->string[current - start] = statement->string[current];

		new->string[current - start + 1] = '\0';
	} else {
		for (current = 0; current <= strlen(statement->string) - 1; current++)
			new->string[current] = statement->string[current + 1];
		
		new->string[current + 1] = '\0';
	}

	return new;
}

/* free_statements will free each statement node in the binary tree. */
void free_statements(Statement *statement)
{
	if (statement == NULL)
		return;

	free_statements(statement->left);
	free_statements(statement->right);
	free(statement);
}

/* create_tree will create a binary tree of statements that represent a
parsed SL tree. It will do this by finding the main operator of the current
statement, saving the left and right sides of it into new statements, and
then repeating the process for each new statement until no more operators
can be found in statement->string. */
void create_tree(Statement *statement)
{
	statement->operator_index = find_operators(statement);
	if (statement->operator_index != 0 && statement->operator_index != NO_OPERATOR) {
		statement->left = create_left(statement);
		create_tree(statement->left);
	}
	if (statement->operator_index != NO_OPERATOR) {
		statement->right = create_right(statement);
		create_tree(statement->right);
	}
}

/* print_tree will find the right-most statement of the binary tree and work
its way to the left side of the binary tree, printing each statement in such
a fashion that all prints should represent the binary tree's structure.
When printed this way, we should get a representation of a parsed SL tree. */
void print_tree(Statement *statement, int spaces)
{
	if (statement == NULL)
		return;

	print_tree(statement->right, spaces + 1);
	
	int i;
	for (i = 0; i < spaces; i++)
		printf("\t\t");

	printf("%s", statement->string);
	if (statement->operator_index != -1) {
		printf(" %c", statement->string[statement->operator_index]);
		if (statement->string[statement->operator_index] == '-')
			printf(">");
		if (statement->string[statement->operator_index] == '<')
			printf("->");
	}
	printf("\n");

	print_tree(statement->left, spaces + 1);
	
}


/* is_wff is the function that makes sure that all leaves of the SL tree are
indeed atomic statement letters. Atomic statement letters are considered capital
letters that represent a statement. An atomic statement letter is trivially
well-formed, so if our functions that build the tree are correct, then the rest
of the tree must be well formed combinations of atomic statement letters and the
entire formula is well formed. */
boolean is_wff(Statement *statement)
{
	// possibly at an atomic statement letter or a negation
	// either way it has to be handled differently than the normal case
	if (statement->left == NULL) {

		if (statement->operator_index == 0)
			return is_wff(statement->right);

		if (statement->right == NULL) {

			if (strlen(statement->string) == 1
			    && statement->string[0] >= 'A'
			    && statement->string[0] <= 'Z')
				return TRUE;
			else
				return FALSE;
		}
	}

	if (is_wff(statement->left) && is_wff(statement->right))
		return TRUE;
	else
		return FALSE;
}

int main()
{
	printf("\nEnter a formula in SL form: ");

	Statement *statement;
	create_statement(&statement);

	scanf("%[^\n]s", statement->string);

	create_tree(statement);
	
	printf("\n");
	print_tree(statement, 0);

	if (is_wff(statement))
		printf("\nThis is a well formed formula.\n");
	else
		printf("\nThis is not a well formed formula.\n");

	free_statements(statement);
	return 0;
}
