/* COP 3502C Programming Assignment 3
This program is written by: Ryan Ramdihal*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct CoordStruct // cartesian plane
{
    int x;
    int y;
};
// global 
struct CoordStruct location;

void ReadData(FILE* inputfile, struct CoordStruct coordinates[], int n);
int compareTo(struct CoordStruct *ptrP1, struct CoordStruct *ptrP2);
void sort(struct CoordStruct coordinates[], int n, int threshold);
void mergeSort(struct CoordStruct coordinates[], int low, int high, int threshold);
void merge(struct CoordStruct coordinates[], int low, int middle, int high);
void insertSort(struct CoordStruct coordinates[], int low, int high);
int binSearch(struct CoordStruct coordinates[], int n, struct CoordStruct querypoint);


int main(void)
{
    FILE* inputfile = fopen("assignment3input.txt", "r"); //open inputfile// has to have name "assignment3input.txt" to open
    FILE* outputfile = fopen("out.txt", "w"); // opens a writeable outputfile

    if(inputfile == NULL)
    {
        printf("No assignment3input.txt file\n"); // if there is no inputfile in correct format // error messege
    }
    else
    {
        int i;
        int n; //size
        int searchPoints; // number of points //s
        int threshold; // t

        struct CoordStruct coordinates[1000000]; // max number of infected = 10^6
        struct CoordStruct querypoint;

        fscanf(inputfile, "%d %d %d %d %d",&location.x, &location.y, &n, &searchPoints, &threshold); //scans in values from inputfile
  
        ReadData(inputfile, coordinates, n); //read in coordinates
        sort(coordinates, n, threshold); 

        // output the sorted coordinates to output file
        for(i = 0;i < n; i++)
        {
            fprintf(outputfile, "%d %d\n",coordinates[i].x, coordinates[i].y);
        }

        // loop to search for s coordinates and output the result to output file
        for(i = 0; i < searchPoints; i++)
        {
            fscanf(inputfile, "%d %d",&querypoint.x, &querypoint.y);
            int index = binSearch(coordinates, n, querypoint);
            if(index == -1) 
            {
                fprintf(outputfile,"%d %d not found\n",querypoint.x,querypoint.y); // if querypoint values not found
            }
            else 
            {
                fprintf(outputfile,"%d %d found at rank %d\n",querypoint.x,querypoint.y, index+1); // if querypoint values found
            }
        }

        fclose(inputfile);
        fclose(outputfile);
    }

    return 0;
}

void ReadData(FILE* file, struct CoordStruct coordinates[], int n) //reads coordinates from file into an array
{
    int i = 0;
    while(i < n)
    {
        fscanf(file, "%d %d",&coordinates[i].x, &coordinates[i].y); //scan in coordinates to array
        i++; // index counter
    }
}


int compareTo(struct CoordStruct *ptrP1, struct CoordStruct *ptrP2)// comparing p1 and p2 
{
    // distance from location
    float P1distance = sqrt((float)pow(location.x-ptrP1->x,2) + (float)pow(location.y-ptrP1->y, 2)); 
    float P2distance = sqrt((float)pow(location.x-ptrP2->x,2) + (float)pow(location.y-ptrP2->y, 2));


    if(P1distance < P2distance)
    {
        return -1;
    }
    else if(P1distance > P2distance)
    {
        return 1;
    }
    else //distances equal, compare x and y 
    {
        if(ptrP1->x < ptrP2->x)
        {
            return -1;
        }
        else if(ptrP1->x > ptrP2->x)
        {
            return 1;
        }
        else
        {
            if(ptrP1->y < ptrP2->y)
            {
                return -1;
            }
            else if(ptrP1->y > ptrP2->y)
            {
                return 1;
            }
            else // both same point
                return 0;
        }
    }
}

void merge(struct CoordStruct coordinates[], int low, int middle, int high) // merge subarrays
{
    struct CoordStruct buffer[high-low+1]; //temp array
    int i = low; //first left index
    int j = middle + 1; //first right index
    int k = 0;

    while(i <= middle && j <= high) // loop until end of both subarrays
    {
        if(compareTo(&coordinates[i], &coordinates[j]) <= 0) 
        {
            buffer[k] = coordinates[i]; // element of left array to buffer
            i++;
        }
        else
        {
            buffer[k] = coordinates[j]; //element of right array to buffer
            j++;
        }
        k++;
    }

    while(i <= middle) // rest of left array to buffer
    {
        buffer[k] = coordinates[i];
        k++;
        i++;
    }


    while(j <= high)// rest of right array to buffer
    {
        buffer[k] = coordinates[j];
        k++;
        j++;
    }

    for(i=low, k=0;i<=high;i++, k++) // copy fully sorted elements into coordinate struct array
    {
        coordinates[i] = buffer[k];
    }
}

void mergeSort(struct CoordStruct coordinates[], int low, int high, int threshold)// recursive
{
    int n = (high-low)+1; // size of the subarray

    if(n <= threshold)
    {
        insertSort(coordinates, low, high);
    }
    else //greater than threshold, use merge
    {
        int middle = (low+high)/2;
        mergeSort(coordinates, low, middle, threshold); //left subarray sort
        mergeSort(coordinates, middle+1, high, threshold); //right subarray sort
        merge(coordinates, low, middle, high); //merge after both subarrays are sorted
    }
}

void insertSort(struct CoordStruct coordinates[], int low, int high)// sort input from low to high
{
    int i;
    int j;
    struct CoordStruct querypoint;
    for(i = low + 1; i <= high; i++)
    {
        j = i - 1; //right array
        querypoint = coordinates[i];

        while( j >= low && compareTo(&querypoint, &coordinates[j]) < 0)
        {
            coordinates[j+1] = coordinates[j];
            j--;
        }

        coordinates[j+1] = querypoint;
    }
}
void sort(struct CoordStruct coordinates[], int n, int threshold) // sort array // size n
{
    if(n <= threshold)
    {
        insertSort(coordinates, 0, n-1); // sort using insert sort
    }
    else 
        mergeSort(coordinates, 0, n-1, threshold); //sort using merge, size greater than threshold
}

int binSearch(struct CoordStruct coordinates[], int n, struct CoordStruct querypoint)// search and returns index of querypoint
{
    int low = 0; //first index
    int high = n-1; //last index
    while(low <= high) //search through entire array
    {
        int middle = (low+high)/ 2; // middle index
        float distance = compareTo(&querypoint, &coordinates[middle]);

        if(distance == 0) 
        {
            return middle; // middle = querypoint// return value;
        }
        else if(distance < 0)
        { 
            high = middle - 1; // search left subarray
        }
        else 
            low = middle + 1;//search right subarray
    }

    return -1; // no querypoint found
}
