#include<stdio.h>
#include<string.h>
#include "timer.h"
#define infinity 1000000
#define MALLOC(t,n)    (t *) malloc( n*sizeof(t))
#define MALLOC2D(t,n)  (t **) malloc(n*sizeof(t))


int sourceNode,destinationNode;
int number_of_vertices;
int  *visited = (int*)   NULL;   //this is equivalent to the set S in our book; an array whose size we do not know
int *heap =(int *) NULL;      //this is our heap
int *heapPositions=(int*) NULL;  // wil be ised to be able to implement decreaseKey efficiently as described in page 65 of the book
int heapsize=0;                   //the size of the heap, initially 0
int  **graph_matrix;	       //our adjencecy matrix
int **perfectheuristics;
int *dist =(int *) NULL;      				// dist[i] is the distance from the source to node i
int *prev = (int *) NULL;					// prev[i] represents the preceding node of node i
double runtime;
int TotalComputation = 0;
char fileName_graph_matrix[30];
char fileName_heuristic_matrix[30];

void allocate(){
  visited=MALLOC(int, number_of_vertices);
  heap=MALLOC(int,number_of_vertices);
  heapPositions=MALLOC(int,number_of_vertices);
  dist=MALLOC(int,number_of_vertices);
  prev=MALLOC(int, number_of_vertices);

  int i;
  for (i = 0; i < number_of_vertices;i++)
  {
    visited[i] = 0;
    TotalComputation+=1;
  }

}

int **allocate_array(int row_dim, int col_dim)
{
  int **result;
  int i,j;

  /* Allocate an array of pointers to hold pointers to the rows of the
 * 	 array */
  result=(int **)malloc(row_dim*sizeof(float *));

  /* The first pointer is in fact a pointer to the entire array */
  result[0]=(int *)malloc(row_dim*col_dim*sizeof(float));

  /* The remaining pointers are just pointers into this array, offset
 * 	 in units of col_dim */
  for(i=1; i<row_dim; i++)
  {
	result[i]=result[i-1]+col_dim;
	//memset(result[i], 0, col_dim * sizeof(float));
  }

  for(i=0;i<row_dim;i++)
  {
	for(j=0;j<col_dim;j++)
		result[i][j] = 0 ;
  }
  return result;

}
// this function just builds the graph ( reads the matrix that we generated already)
void deallocate_array(int **array, int row_dim)
{
  int i;
  /* Make sure all the pointers into the array are not pointing to
	 random locations in memory */
  for(i=1; i<row_dim; i++)
	array[i]=NULL;
  /* De-allocate the array */
  free(array[0]);
  /* De-allocate the array of pointers */
  free(array);
}


void buildGraph(void){
  FILE *fp;
  //fp = fopen("testsparse.txt","r");
  fp = fopen(fileName_graph_matrix,"r");
  //fp = fopen("fullyconnected_dynamicedge_32000.txt","r");
     //fp = fopen("dense_dynamicedge_32000.txt","r");

  //fp = fopen("TestGraph.txt","r");
  int i=0,j=0;
  int counter=0;

fscanf(fp,"%d",&number_of_vertices);
//printf("%d\n",number_of_vertices);

  //allocate();
  //graph_matrix=MALLOC2D(int,number_of_vertices);
  graph_matrix = allocate_array(number_of_vertices,number_of_vertices);

/*
 for(i=0;i<number_of_vertices;i++)
 {
 	graph_matrix[i]=MALLOC(int,number_of_vertices);
 	//visited[i]=0;
	//TotalComputation+=1;
 }*/
 for(i=0;i<number_of_vertices;i++)
 {
	 	for(j=0;j<number_of_vertices;j++)
	 	   {
   	 		fscanf(fp,"%d",&graph_matrix[i][j]);
   	 		//printf("%d ",graph_matrix[i][j]);
   	 	}
	//printf("\n");

 }

  	fclose(fp);

}

void ReorderHeap(int i){
     int l=2*i;
     int r=(2*i) +1;
     int j;
     int smallest;
   //  printf("checking left or right\n");
   // printf("for L:SIZE=%d; heap[L]=%d; heap[i]=%d; dist[heap[l=%d]]=%d; dist[heap[i]]=%d\n",heapsize,heap[l],heap[i],l,dist[heap[l]],dist[heap[i]]);
   //printf("\n l=%d; heapSize=%d; dist[%d]=%d\n",l,heapsize,dist[heap[i]]);
     if(l<=heapsize && dist[heap[l]]<dist[heap[i]])
          smallest=l;
     else
            smallest=i;
    // printf("smallest 1 is %d\n",smallest);
     TotalComputation+=1;
    //printf("for R:SIZE=%d; heap[R]=%d; heap[i]=%d; dist[heap[r=%d]]=%d; dist[smaller]=%d\n",heapsize,heap[r],heap[i],r,dist[heap[r]],dist[heap[smallest]]);
     if(r<heapsize && dist[heap[r]]<dist[heap[smallest]])
           smallest=r;

    // printf("smallest is %d\n",smallest);
     if(smallest!=i){
                int temp;
                temp=heap[i];
                heap[i]=heap[smallest];
                heap[smallest]=temp;
                heapPositions[heap[i]]=i;
                heapPositions[heap[smallest]]=smallest;
                TotalComputation += 5;

            /*printf("\nwhile OREDERING (heap size=%d): heap before ordering: \n",heapsize);
               for(j=0;j<heapsize+1;j++)
             printf("\t%d(EV=%d,PAR=%d,POS=%d)",heap[i],dist[heap[i]],prev[heap[i]],heapPositions[heap[i]]);*/
               ReorderHeap(smallest);
     }

}
void DecreaseKey(int index, int key){
	if(key>dist[heap[index]])
		printf("error --> key=%d, and heap[%d]=%d\n",key,index,heap[index]);


		//heap[index]=key;
		//heapPositions[key]=index;
	//	printf("khra,key=%d, heap[index=%d]=%d(p=%d,ev=%d)",key,index, heap[index],prev[heap[index]],dist[heap[index]]);
		dist[heap[index]]=key;
		//printf("key is: %d. dist of key is : %d. index is :%d\n",key,dist[heapkey],index);
		int i;
		/* printf("\n Open List right before decrease key:\n");
        for(i=0;i<heapsize+1;i++)
            printf("heap[%d]=%d(EV=%d,PAR=%d)\n",i,heap[i],dist[heap[i]],prev[heap[i]]);*/

		while(index>1 && dist[heap[index/2]]>dist[heap[index]])
			{  //printf("did it enter here?");
				int temp=heap[index/2];
				heap[index/2]=heap[index];
				heap[index]=temp;
				heapPositions[heap[index]]=index;
				heapPositions[heap[index/2]]=index/2;
				TotalComputation+=4;
				index=index/2;
			}
       /* printf("\n Open List after decrease key:\n");
        for(i=0;i<heapsize+1;i++)
            printf("heap[%d]=%d(EV=%d,PAR=%d)\n",i,heap[i],dist[heap[i]],prev[heap[i]]);*/
}
void InsertToHeap(int e)
{       int i, tempBig=-5,temp;

        heapsize++;
        temp=dist[e];
       // printf("temp =%d\n",temp);
      //  printf("new heapSize after inserting %d is %d\n",e,heapsize);
        heap[heapsize]=e;
        heapPositions[e]=heapsize;

        dist[e] = infinity;  //insert at the end
     //   printf("heap[heapsize After insert=%d]=%d (p=%d,ev=%d)\n",heapsize,e,prev[e],dist[e]);
        TotalComputation+=1;
        DecreaseKey(heapsize,temp);

}

int DeleteMin()
{      int min,i;
       if(heapsize<1)
            printf("Error, heap is already empty");
       min=heap[1];
       heap[1]=heap[heapsize];
       heapPositions[heap[1]]=1;
       heapsize=heapsize-1;
        TotalComputation+=4;
       // printf("\nwhile deleting (heap size=%d): heap before ordering: \n",heapsize);
        /*for(i=0;i<heapsize+1;i++)
             printf("\t%d(EV=%d,PAR=%d,POS=%d)",heap[i],dist[heap[i]],prev[heap[i]],heapPositions[heap[i]]);*/
      //  printf("\n\n");
       ReorderHeap(1);
      // printf("\nwhile deleting: heap AFTER ordering: \n");
     /*   for(i=0;i<heapsize+1;i++)
             printf("\t%d(EV=%d,PAR=%d,POS=%d)",heap[i],dist[heap[i]],prev[heap[i]],heapPositions[heap[i]]);*/
       return min;
}


void Dijsktra(int source, int destination){
    StartTimer();
	int v,j,i;
	//step1 : initialize Single_Source (look book 648)
	for(v=0;v<number_of_vertices;v++)
		{
		 dist[v]=infinity;
		 prev[v]=NULL;
		 TotalComputation+=2;
		}
	dist[source]=0;
	TotalComputation+=1;
	//step2: Visited should be empty set, and heap should contain all nodes of our graph ( see book 658).
	     //visited is already empty.
	     //now we insert all nodes to the heap
	     for(i=0;i<number_of_vertices;i++)    //insert all nodes to the heap --this step could be done earlier.
		InsertToHeap(i);

    //step 3:   do the while loop part ( look page 658)
	while(heapsize>=1) {

        int currentNode=DeleteMin();
		if(visited[currentNode]==1)
			continue;
		visited[currentNode]=1;  //add this node to visited ( to set S in the book page 658)
		if(currentNode==destinationNode) //if we reach the goal, no need to continue
			break;
		// now we should relax the edges
		for(i=0;i<number_of_vertices;i++)    //get the edges of currentNode, and relax them
		     {
     			if(graph_matrix[currentNode][i]>0)
     				{  if(dist[currentNode] + graph_matrix[currentNode][i] < dist[i] && visited[i]==0)
				     		{
		     					dist[i]=dist[currentNode]+graph_matrix[currentNode][i];
		     					prev[i]=currentNode;
							TotalComputation+=4;
		     					DecreaseKey(heapPositions[i],dist[i]);

		     				}
				     }

     		}

	}
    runtime = GetTimer();
      if(dist[destination]!=infinity)
        {//printf("distance between source node  %d and node  %d is %d\n",sourceNode,destinationNode,dist[destinationNode]);
       // printf("the path is: \n");
        }


}
/*void printShortestPath(int destination){
   	 if(destination==sourceNode)
   	 	{
	 	   	printf("%d -->",destination);
	 	   }
	 else if(destination==destinationNode)
	 	{
	 		printShortestPath(prev[destination]);
	 		printf("%d\n",destination);
	 	}
	 	else{
	 		printShortestPath(prev[destination]);
	 		printf("%d -->",destination);
	 	}


}*/

void savePerfectHeuristics(int number_of_vertices) {
    FILE *heurFile;
    int i,j,temp;

    if((heurFile=fopen(fileName_heuristic_matrix,"w"))==NULL)
        printf("File of heuristics could not be created");
    else {
          perfectheuristics=allocate_array(number_of_vertices,number_of_vertices);
          //printf("pass\n");
          for(i=0;i<number_of_vertices;i++)
            {for(j=0;j<number_of_vertices;j++)
                {   sourceNode=i;destinationNode=j;
                    allocate();
                    heap[0]=-1;     // we start indexing at 1 (not 0) so that to be able to use the array implementation of heap; other wise, 2n might be 0..
                    dist[heap[0]]=-infinity;
                    Dijsktra(i,j);

                   // printShortestPath(j);
                    temp=dist[j];//printf("temp=%d\n",temp);
                    perfectheuristics[i][j]=temp;
                   // printf("perfect heuristic[%d][%d]=%d",i,j,perfectheuristics[i][j]);
                    memset(visited,0,number_of_vertices);
                    memset(heap,0,number_of_vertices);
                    memset(heapPositions,0,number_of_vertices);
                    memset(dist,0,number_of_vertices);
                    memset(prev,0,number_of_vertices);
                    heapsize = 0;
                }


            }
            printf("Heuristic values are:\n\n");
         for(i=0;i<number_of_vertices;i++)
            {for(j=0;j<number_of_vertices;j++)
                {   printf("%d ",perfectheuristics[i][j]);
                    fprintf(heurFile,"%d ",perfectheuristics[i][j]);
                }
              fprintf(heurFile,"\n");
              printf("\n");
            }
     fclose(heurFile);
     deallocate_array(perfectheuristics,number_of_vertices);
    }


}
main(){
	int repeat,count=0,maxiter=150;
	double sumruntime=0.0,sumcomputation=0.0,maxruntime=0.0,maxcomputation=0.0,avgruntime,avgcomputation;
	printf("\t\tGenerating a perfect heuristic for a given graph matrix:\n\t---------------------------------------------------------------\n\n");
	printf("Please enter the name of matrix file located withing this folder\n\t(example: matrix2.txt):" );
	gets(fileName_graph_matrix);
	strcpy(fileName_heuristic_matrix,"HeuristicValus_SPARSE.txt");

	buildGraph();   // first step is to create the graph from the text file
    savePerfectHeuristics(number_of_vertices);
    printf("The Heuristic values in the file %s\n",fileName_heuristic_matrix);
}
