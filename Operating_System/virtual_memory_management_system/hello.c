#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node *next;
	};

struct node *head;
struct node *tail;

init(void) {
	head=(struct node *) malloc(sizeof *head);
	tail=(struct node *) malloc(sizeof *head);
	head->next = tail; tail->next = tail;
}

struct node *append(int v) {
	struct node *ptr;
	struct node *t;
	ptr=head;
	while (ptr->next != tail) ptr = ptr->next;
	t=(struct node *) malloc(sizeof *t);
	t->value=v;
	t->next = tail;
	ptr->next = t;
	return ptr;
}

struct node *insert(int v, struct node *ptr) {
	struct node *t;
	t=(struct node *) malloc(sizeof *t);
	t->value=v;
	t->next = tail;
	ptr->next = t;
	return ptr;
	}	
	
void delete(struct node *ptr) {
	struct node *t;
	t = ptr->next;
	ptr->next = ptr->next->next;
	free(t);
}
	


	struct node * next;
} ;
typedef struct node new_node;
int main () {
struct node *ptr;

init();

ptr=append(10);
ptr=append(20);
ptr=append(30);
ptr=append(40);
ptr=append(50);

//delete(ptr);

ptr=head->next;
while (ptr != tail) {
	printf("%d\n",ptr->value);
	ptr = ptr->next;
	}
}	 
/*#include<stdio.h>
#include<stdlib.h>
struct node{
	int num;
new_node * root = NULL;

main()
{
	/*
	FILE *test_file;
	test_file = fopen("test.log", "a");
	fputs("This is testing for fputs...\n", test_file);
	fclose(test_file);
	test_file = fopen("test.log", "a");
	
	fputs("This is second testing for fputs... \n", test_file);

	

	printf("hello world\n");
	new_node * current = root;
	new_node * head = current;
	current = (new_node *) malloc(sizeof(new_node));
	current->num = 1;
	current = current->next;
	current = (new_node *) malloc(sizeof(new_node));
	current->num = 2;

//	new_node * current2 = root;
	printf("linklist first digit: %d\n", current->num);
//	current2 = current2->next;
	printf("linklist second digit: %d\n", head->num);
	

/*	new_node * current2 = root;
	while(current2 != NULL){
		printf("%d\n",current2->num);
		current2 = current2->next;
	}

	return 0;
}*/