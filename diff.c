#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define _ZERO_ CreateNode(NUMBER, NULL, NULL)
#define _ONE_ CreateNode(VAR, NULL, NULL)

enum Token {
	NUMBER,
	VAR,
	PLUS,
	MINUS,
	MULT,
	DIVIDE,
	POW,
	LOG,
	SIN,
	COS
};

struct Node
{
	char * val;
	int type;
	struct Node * left;
	struct Node * right;
};

void arr_fill(char * array, FILE * stream, int * number);													//Функции, которые остались от акинатора и нужны для заполнения исходного дерева.
void arr_slash(char * array, int * number);																		//Всего их 9 штук.
struct Node * tree_fill(struct Node * root, char * array, int * number);
struct Node * tree_add_preparation(struct Node * root, char * array, int * depth);
void tree_add(struct Node * root, int * depth, int * count, char * array, int * count_2);
int tab_count(const char * array);
char * spaces(char * array);
int point_finder(char * array);
int type_determinant(char * array);	

struct Node * Diff(const struct Node * root);
struct Node * CreateNode(int fella, struct Node * lf, struct Node * rg);
struct Node * Copy(const struct Node * root);

void tree_destroy(struct Node * root);
void tree_destroy_diff(struct Node * root);
void tree_png(struct Node * root, FILE * stream, int lab, int * lab_count);
void digraph(FILE * stream, int * label);

int main()
{
	FILE * f_in= NULL;
	FILE * f_out_1 = NULL;
	FILE * f_out_2 = NULL;
	char strings[1000] = {0};
	char * c = calloc(1, sizeof(char));
	int * n = calloc(1, sizeof(char));
	int * num = calloc(1, sizeof(int));
	struct Node * tree = NULL;
	struct Node * tree_diff = NULL;
	int lab = 0;
	int * lab_count = calloc(1,sizeof(int));
	f_in = fopen("strings", "r");
	f_out_1 = fopen("Tree1.dot", "w");
	f_out_2 = fopen("Tree2.dot", "w");
	assert(f_in);
	assert(f_out_1);
	assert(f_out_2);
	arr_fill(strings, f_in, num);
	arr_slash(strings, num);
	tree = tree_fill(tree, strings, num);
	assert(tree);
	tree_diff = Diff(tree);
	digraph(f_out_1, lab_count);
	tree_png(tree, f_out_1, lab, lab_count);
	digraph(f_out_1, lab_count);
	digraph(f_out_2, lab_count);
	tree_png(tree_diff, f_out_2, lab, lab_count);
	digraph(f_out_2, lab_count);
	free(c);
	free(n);
	free(num);
	tree_destroy_diff(tree_diff);
	tree_destroy(tree);
	free(lab_count);
	fclose(f_in);
	fclose(f_out_1);
	fclose(f_out_2);
	return 0;
}

int type_determinant(char * array) {
	int type = 0;
	if(array[1] == 'x') type = VAR;
	else if(array[1] == '+') type = PLUS;
	else if(array[1] == '-') type = MINUS;
	else if(array[1] == '*') type = MULT;
	else if(array[1] == '/') type = DIVIDE;
	else if(array[1] == '^') type = POW;
	else if(array[1] == 'l') type = LOG;
	else if (array[1] == 's') type = SIN;
	else type = NUMBER;
	return type;
}

void arr_fill(char * array, FILE * stream, int * number)
{
	assert(array);
	assert(stream);
	int counter = 0;
	int c = 0;
	for(; ; counter++)
	{
		c = fgetc(stream);
		if(c == EOF) break;
		array[counter] = c;
	}
	*number = counter;
}

void arr_slash(char * array, int * number)
{
	assert(array);
	assert(number);
	int counter = 0;
	for(; counter < *number; counter++)
	{
		if(array[counter] == '!')
		{
			array[counter] = '\0';
		}
	}
	array[counter + 1] = '\0';
}

struct Node * tree_fill(struct Node * root, char  * array, int * number)
{
	int counter = 0;
	int * depth = calloc(1, sizeof(int));
	for(; counter < *number; counter++)
	{
		if(array[counter] == '\0')
		{
			counter++;
			root = tree_add_preparation(root, array + counter, depth);
			assert(root);
		}
	}
	free(depth);
	return root;
}

struct Node * tree_add_preparation(struct Node * root, char * array, int * depth)
{
	assert(array);
	if(root == NULL)
	{
		root = calloc(1, sizeof(struct Node));
		root->val = (array + 1);
		root->type = type_determinant(array);
		return root;
	}
	int * count = calloc(1, sizeof(int));
	int * count_2 = calloc(1, sizeof(int));
	*depth = tab_count(array);
	if(*depth) tree_add(root, depth, count, array, count_2);
	free(count);
	free(count_2);
	return root;
}

void tree_add(struct Node * root, int * depth, int * count, char * array, int * count_2)
{
	assert(depth && count);
	if(*count < (*depth - 1) && root)
	{
		(*count)++;
		tree_add(root->right, depth, count, array, count_2);
		tree_add(root->left, depth, count, array, count_2);
		(*count)--;
	}	
	if(root)
	{
		if((*count == (*depth - 1)) && (!root->right || !root->left) && (root) && !(*count_2))
		{
			(*count_2)++;
			array = spaces(array);
			assert(array[0] == '(' || array[0] == ')' || array[0] == '\0');
			if(!point_finder(root->val))
			{
				if(array[0] == '(') 
				{
					assert(!root->right);
					root->right = calloc(1, sizeof(struct Node));
					root->right->val = (array + 1);
					root->right->type = type_determinant(array);
				}
				else if(array[0] == ')')
				{
					assert(!root->left);
					root->left = calloc(1, sizeof(struct Node));
					root->left->val = (array + 1);
					root->left->type = type_determinant(array);
				}
			}
			else
				(*count_2)--;
		}
	}
}

int tab_count(const char * array)
{
	assert(array);
	int counter = 0;
	int depth = 0;
	for(; array[counter] != '\0'; counter++)
	{
		if(array[counter] == '\t') 
		{	
			depth++;
		}
	}
	return depth;
}

char * spaces(char * array)
{
	assert(array);
	int counter = 0;
	int space = 0;
	for(; array[counter] != '\0'; counter++)
	{
		if(array[counter] == '\t' || array[counter] == '\n')
		{
			space++;
		}
	}
	return (array + space);
}

int point_finder(char * array)
{
	assert(array);
	int counter = 0;
	int i = 0;
	for(i = 0; array[i] != '\0'; i++)
	{
		if(array[i] == ' ') counter++;
	}
	return counter;
}

void tree_png(struct Node * root, FILE * stream, int lab, int * lab_count)
{
	assert(stream);
	if(root) 
	{
		fprintf(stream, "n%03d ", lab);
		fprintf(stream, "[label=\"%s\"] ;\n", root->val);
		assert(lab < 990 && (*lab_count) < 990);
		if(root->left || root->right) fprintf(stream, "n%03d -> n%03d;\n", lab, (lab + 1));
		(*lab_count)++;
		tree_png(root->left, stream, (lab + 1), lab_count);
		assert(lab < 990 && (*lab_count) < 990);
		if(root->left || root->right) fprintf(stream, "n%03d -> n%03d;\n", lab, (*lab_count));
		tree_png(root->right, stream, *lab_count, lab_count);
	}
}

struct Node * Diff(const struct Node * root)
{
	switch(root->type)
	{
		case NUMBER: return _ZERO_ ;
		case VAR: return _ONE_ ;
		case PLUS: return CreateNode(PLUS, Diff(root->left), Diff(root->right));
		case MINUS: return CreateNode(MINUS, Diff(root->left), Diff(root->right));
		case MULT: 
		{
			struct Node * f_factor = CreateNode(MULT, Diff(root->left), Copy(root->right));
			struct Node * s_factor = CreateNode(MULT, Copy(root->left), Diff(root->right));
			return CreateNode(PLUS, f_factor, s_factor);
		}
		case DIVIDE:
		{
			struct Node * du = Diff(root->left);
			struct Node * dv = Diff(root->right);
			struct Node * u = Copy(root->left);
			struct Node * v = Copy(root->right);
			struct Node * nom = CreateNode(MINUS, CreateNode(MULT, du, v), CreateNode(MULT, u, dv));
			struct Node * denom = CreateNode(MULT, Copy(v), Copy(v));
			return CreateNode(DIVIDE, nom, denom);
		}
		case POW:
		{
			struct Node * u = Copy(root->left); 
			struct Node * n = Copy(root->right);			//u^n
			struct Node * one = _ONE_ ;
			struct Node * multi_1 = CreateNode(POW, u, CreateNode(MINUS, n, one));
			struct Node * multi_2 = CreateNode(MULT, Copy(root->right), multi_1);
			return CreateNode(MULT, multi_2, Diff(root->left));
		}
		case LOG:
		{
			struct Node * u = Copy(root->left);
			struct Node * fd = Copy(root->right);			//log[u, fd]
			struct Node * exp = _ZERO_ ;
			exp->val[0] = 'e';
			struct Node * den = CreateNode(MULT, u, CreateNode(LOG, fd, exp));
			return CreateNode(DIVIDE, Diff(root->left), den);
		}
		case SIN:
		{
			struct Node * a = Copy(root->left);		//sin(a)
			struct Node * cos = CreateNode(SIN, a, _ZERO_);
			return CreateNode(MULT, cos, Diff(root->left));
		}
		default: printf("Wrong type");
	}
	return NULL;
}

struct Node * CreateNode(int fella, struct Node * lf, struct Node * rg)
{
	switch(fella)
	{
		struct Node * tree = NULL;
		case NUMBER:
			tree = calloc(1,sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '0';
			tree->val[1] = '\0';
			tree->type = NUMBER;
			return tree;
		case VAR:
			tree = calloc(1,sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '1';
			tree->val[1] = '\0';
			tree->type = VAR;
			return tree;
		case PLUS:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '+';
			tree->val[1] = '\0';
			tree->type = PLUS;
			tree->left = lf;
			tree->right = rg;
			return tree;
		case MINUS:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '-';
			tree->val[1] = '\0';
			tree->type = MINUS;
			tree->left = lf;
			tree->right = rg;
			return tree;
		case MULT:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '*';
			tree->val[1] = '\0';
			tree->type = MULT;
			tree->left = lf;
			tree->right = rg;
			return tree;
		case DIVIDE:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '/';
			tree->val[1] = '\0';
			tree->type = DIVIDE;
			tree->left = lf;
			tree->right = rg;
			return tree;
		case POW:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(2, sizeof(char));
			tree->val[0] = '^';
			tree->val[1] = '\0';
			tree->type = POW;
			tree->left = lf;
			tree->right = rg;
			return tree;
		case LOG:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(4, sizeof(char));
			tree->val[0] = 'l';
			tree->val[1] = 'o';
			tree->val[2] = 'g';
			tree->val[3] = '\0';
			tree->type = LOG;
			tree->left = lf;
			tree->right = rg;
			return tree;
		case SIN:
			tree = calloc(1, sizeof(struct Node));
			tree->val = calloc(4, sizeof(char));
			tree->val[0] = 'c';
			tree->val[1] = 'o';
			tree->val[2] = 's';
			tree->val[3] = '\0';
			tree->type = COS;
			tree->left = lf;
			tree->right = rg;
			return tree;
	}
	return NULL;
}

struct Node * Copy(const struct Node * root)
{
	if(root)
	{
		struct Node * tree = calloc(1, sizeof(struct Node));
		tree->val = calloc(10, sizeof(char));
		tree->val = strncpy(tree->val, root->val, 10);
		tree->type = root->type;
		tree->left = Copy(root->left);
		tree->right = Copy(root->right);
		return tree;
	}
	return NULL;
}

void digraph(FILE * stream, int * label)
{
	assert(label);
	if(!(*label)) {
		fprintf(stream, "digraph G{\n");
		fprintf(stream, "node [style=filled, color=azure3];\n");
	}
	else {
		fprintf(stream,"}\n");
		(*label) = 0;
	}
}

void tree_destroy(struct Node * root)
{
	if(root)
    {
		tree_destroy(root->left);
		tree_destroy(root->right);
		free(root);
    }
}

void tree_destroy_diff(struct Node * root)
{
	if(root)
	{
		tree_destroy_diff(root->left);
		tree_destroy_diff(root->right);
		free(root->val);
		free(root);
	}
}
