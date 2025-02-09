#include <stdio.h>
#include <stdlib.h>

//define struct named node that includes number and next ( pointer )
typedef struct node
{
    int number;
    struct node *next;
}
node;

int main(void)
{
    // iniatalize variable named list which is of structure node (defined above) which is empty to start off NULL
    node *list = NULL;
 
    //add a number to list
    node *n = malloc(sizeof(node));

    //safty step if f.e computer out of memory stop here
    if(n == NULL)
    {
    free(list);
    return 1;
    }
    //add number 2 to list with pointer as a node (number + next = node as def. above)
    n->number = 2;
    n->next = NULL;
    list->next = n;

    //free list if something goes wrong
    n = malloc(sizeof(node));
    if (n == NULL)
    {
        free(list->next);
        free(list);
        return 1;
    }
    //add number 3 to list
    n-> number = 3;
    n->next = NULL;
    // add the 3rd position in list (next next) (pointing through the nodes with next)
    list->next->next = n;

    //print numbers
    //node (type def above) *tmp (temporary pointer) = list (same as i = 0); the temporary node isnt NULL, go on and set the temporaray node as the next one in my list with a pointer
    for(node *tmp = list; tmp != NULL; tmp = tmp->next)
    {
        //print whatever in my tmp node is as number (def above)
        printf("%i\n", tmp->number);
    }
    //free linked list
    while(list != NULL)
    {
        //create tmp node to store the first address of the list's pointer to 2nd address
        node *tmp = list->next;
        //delete first address here
        free(list);
        //set new list to my temporary variable which stored the pointer to the 2nd address and repeat
        list = tmp;
    }
}
