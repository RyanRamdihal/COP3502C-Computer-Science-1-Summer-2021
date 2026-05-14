/* COP 3502C Assignment 2
This program is written by: Ryan Ramdihal */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // INT_MAX

typedef struct customer {
    char name[9];                  
    int lineNumber;  // line number for customer
    int time;    // when at line
    int numberItems;  // amount of items customers have
} customer;

typedef customer *queueitem;
typedef struct node   // struct of nodes in queue
{
    queueitem data;       // pointer to items
    struct node *next;    //pointer for next item in queue
} node;

typedef struct queue // struct of queue
{
    node *front;     //points to first in list
    node *back;    // points to last in list
    int size;     // size of current queue
} queue;

#define lines 12  // amount of lines
queue queue_list[lines];

int current_time;                         

void initialize(queue *queue)  // initialize queue
{
    queue->front = NULL;
    queue->size = 0;
    queue->back = NULL;
}

int empty(queue *queue)  // returns 1 if empty , else 0
{
if (0)
    return (queue->size == 0);
else
    return (queue->front == NULL);
}

void enqueue(queue *queue, queueitem data)
{
    node *new;
    node *prev;

    new = malloc(sizeof(node));
    new->next = NULL;
    new->data = data;  // to end of queue
    prev = queue->back;

    if (prev != NULL)
        prev->next = new;
    else
        queue->front = new;

    queue->back = new;

if (queue->size)
    queue->size += 1;
}

queueitem dequeue(queue *queue)
{
    node *currentnode;
    node *nextnode;
    queueitem data;

    do {
        currentnode = queue->front; // first node

        if (currentnode == NULL) // if no node, return null
        {
            data = NULL;
            break;
        }

        data = currentnode->data; //data from currentnode
        nextnode = currentnode->next; // point to nextnode
        node *back = queue->back;

        if (currentnode == back)
            queue->back = nextnode;

        queue->front = nextnode;

if (queue->size)
        queue->size -= 1;
        free(currentnode);
    } while (0);

    return data;
}

queueitem front(queue *queue) // peek to front 
{
    node *currentnode;
    queueitem data;

    currentnode = queue->front;
    if (currentnode != NULL)
        data = currentnode->data;
    else
        data = NULL;

    return data;
}

customer *nextcustomer(void) // getting new customer
{
    int i;
    queue *queue;
    int mintime;
    customer *c;
    customer *next;
    int p;
    
    for (p = 1;  p <= 2;  ++p) // find nextcustomer by comparing times
    {
        next = NULL;
        mintime = INT_MAX;
       int pcounter = 0;

        for (i = 0;  i < lines;  ++i) //reading all lines
         {
            queue = &queue_list[i];

            c = front(queue); //front customer in line
            if (c == NULL)
                continue;

            pcounter = 1;  // customer waiting

            if (c->time < mintime)
                mintime = c->time;

            if (c->time > current_time) // skip if customer not in line yet, comparing their time and current
                continue;

            if (next == NULL) // no more customers
            {
                next = c;
                continue;
            }

            if (c->numberItems < next->numberItems) // least amount of items to checkout
            {
                next = c;
                continue;
            }
        }

        if (! pcounter) // no more customers
            break;
    
        if (next != NULL)  //dequeue and free
        {
            queue = &queue_list[next->lineNumber - 1];
            dequeue(queue);
            break;
        }

        
        current_time = mintime; // waiting on future customers
    }
    return next;
}

void checkout(customer *customer)
{
    int checkouttime;

    if (current_time < customer->time) //arrival
        current_time = customer->time;

    checkouttime = 0;
    checkouttime += 30;
    checkouttime += customer->numberItems * 5; // total time it takes to check the customer out

    current_time += checkouttime; // amount of time it took to check customer out, present

    printf("%s from line %d checks out at time %d.\n",
        customer->name,customer->lineNumber,current_time);

    free(customer);
}

void testcase(FILE *file)
{
    queue *queue;
    customer *customer;

    int totalcustomers;
    fscanf(file," %d", &totalcustomers); // # of customers

    for (int currentcustomer = 0;  currentcustomer < totalcustomers;  ++currentcustomer) //scans in customers info
     {
        customer = malloc(sizeof(*customer));

        fscanf(file," %d", &customer->time);
        fscanf(file," %d", &customer->lineNumber);
        fscanf(file," %s", customer->name);
        fscanf(file," %d", &customer->numberItems);

        queue = &queue_list[customer->lineNumber - 1]; // customer added to next available line
        enqueue(queue,customer);
    }
    current_time = -1; 

    while (1) 
    {
        customer = nextcustomer();

        if (customer == NULL)// no more customers
            break;

        checkout(customer); 
    }
}

int main(int argc, char **argv)
{
    char *inputfile;
    FILE *file;

    --argc; //argument count
    ++argv; //argument value

    if (argc > 0)
        inputfile = *argv; // open input file from command line while running program
    else
        inputfile = "assignment2input.txt";  // if not, automatically uses "assignment2input.txt" file
    file = fopen(inputfile,"r");
    if (file == NULL)  // if no file , end
    {
        exit(1);
    }

    int testCases;
    fscanf(file," %d", &testCases); // scan in number of testcases

    for ( ;  testCases > 0;  --testCases) // all test cases
        testcase(file);

    fclose(file);
    return 0;
}
