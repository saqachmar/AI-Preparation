
#include<stdio.h>
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

int *evaluation =(int *) NULL;      		// evaluation[i] is the evaluation from the node i to node goal evaluation[i]=gScore[i]+h(i)
int *gScore=(int *) NULL;
int *heuristic=(int *) NULL;
int **heuristicMatrix;

int *prev = (int *) NULL;					// prev[i] represents the preceding node of node i
double runtime;
int TotalComputation = 0;
int counterPerfect=0;         //this counter is just to avoid loading the heuristics when its matrix is already in memory.

void allocate(){
  visited=MALLOC(int, number_of_vertices);
  heap=MALLOC(int,number_of_vertices);
  heapPositions=MALLOC(int,number_of_vertices);
  evaluation=MALLOC(int,number_of_vertices);
  gScore=MALLOC(int,number_of_vertices);
  heuristic=MALLOC(int,number_of_vertices);
  prev=MALLOC(int, number_of_vertices);
  int i;
   for(i=0;i<number_of_vertices;i++)
 {
            visited[i]=0;
            heapPositions[i]=-1;
            prev[i]=-1;
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

void buildGraph(void){
  FILE *fp;
  fp = fopen("testsparse.txt","r");
  //fp = fopen("matrix4.txt","r");

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
 //	graph_matrix[i]=MALLOC(int,number_of_vertices);
// 	visited[i]=0;
// 	heapPositions[i]=-1;
// 	prev[i]=-1;
//	TotalComputation+=1;
 }
*/
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
   // printf("for L:SIZE=%d; heap[L]=%d; heap[i]=%d; evaluation[heap[l=%d]]=%d; evaluation[heap[i]]=%d\n",heapsize,heap[l],heap[i],l,evaluation[heap[l]],evaluation[heap[i]]);
   //printf("\n l=%d; heapSize=%d; evaluation[%d]=%d\n",l,heapsize,evaluation[heap[i]]);
     if(l<=heapsize && evaluation[heap[l]]<evaluation[heap[i]])
          smallest=l;
     else
            smallest=i;
    // printf("smallest 1 is %d\n",smallest);
     TotalComputation+=1;
    //printf("for R:SIZE=%d; heap[R]=%d; heap[i]=%d; evaluation[heap[r=%d]]=%d; evaluation[smaller]=%d\n",heapsize,heap[r],heap[i],r,evaluation[heap[r]],evaluation[heap[smallest]]);
     if(r<heapsize && evaluation[heap[r]]<evaluation[heap[smallest]])
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
             printf("\t%d(EV=%d,PAR=%d,POS=%d)",heap[i],evaluation[heap[i]],prev[heap[i]],heapPositions[heap[i]]);*/
               ReorderHeap(smallest);
     }

}
void DecreaseKey(int index, int key){
	if(key>evaluation[heap[index]])
		printf("error --> key=%d, and heap[%d]=%d\n",key,index,heap[index]);

		evaluation[heap[index]]=key;
		int i;
        while(index>1 && evaluation[heap[index/2]]>evaluation[heap[index]])
			{  //printf("did it enter here?");
				int temp=heap[index/2];
				heap[index/2]=heap[index];
				heap[index]=temp;
				heapPositions[heap[index]]=index;
				heapPositions[heap[index/2]]=index/2;
				TotalComputation+=4;
				index=index/2;
			}

}
void InsertToHeap(int e)
{       int i, tempBig=-5,temp;

        heapsize++;
        temp=evaluation[e];
        heap[heapsize]=e;
        heapPositions[e]=heapsize;

        evaluation[e] = infinity;  //insert at the end
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

       ReorderHeap(1);
       return min;
}


void compute_heuristic_values(int heuristic_choice){

   int i,j;
   FILE *f;
   if(heuristic_choice==0){      //No heuristic. //set all nodes to have heuristic 0
        for(i=0;i<number_of_vertices;i++)
        heuristic[i]=0;
        }

   else{ f=fopen("HeuristicValus_SPARSE.txt","r");
		if (f == NULL)
		{
			printf("Error loading Heuristic File. Programming exiting\n");
			exit(1);
		}

                if(counterPerfect++==0){ //if the heuristic matrix was never loded , load it; otherwise, it is already there.
                    heuristicMatrix=allocate_array(number_of_vertices,number_of_vertices);
  //                  for(i=0;i<number_of_vertices;i++)
    //                    heuristicMatrix[i]=MALLOC(int,number_of_vertices);

                    for(i=0;i<number_of_vertices;i++)           //read the whole heuristic matrix ( for potential uses later, with a different source/destination is given)
                        {
                            for(j=0;j<number_of_vertices;j++)
                                {fscanf(f,"%d",&heuristicMatrix[i][j]);
                               /* printf("%d ",heuristicMatrix[i][j]);*/}
                               // printf("\n");
                        }
                }
                    for(i=0;i<number_of_vertices;i++) {
                                heuristic[i]=heuristicMatrix[i][destinationNode];
                               // printf("heuristic[%d] to destination %d=%d\n",i,destinationNode,heuristic[i]);
                    }


        fclose(f);

   }

}


void Astar(int source, int destination){
    StartTimer();
	int v,j,i;
	//step1 : Add the source node to open List ( which is heap)
	gScore[source]=0;
	evaluation[source]=heuristic[source]; // f(source)=g(source)+h(source), and g(source)=0
	TotalComputation+=2;
	InsertToHeap(source);

	//step2: keep selectiong nodes with minimal evaluation f in the heap untill it is empty ( or untill we reach a solution)

	while(heapsize>=1) {


		int currentNode=DeleteMin();

		visited[currentNode]=1;  //add this node to visited ( to set S in the book page 658)

         if(currentNode==destinationNode)
            break;


		for(i=0;i<number_of_vertices;i++)    //get the neighbors of current node
		     {
     			if(graph_matrix[currentNode][i]>0 && visited[i]!=1) // i is a neighbor
     				{
                       int g_score_try=gScore[currentNode]+  graph_matrix[currentNode][i];
                        if(heapPositions[i]==-1||(g_score_try<gScore[i])){
                            prev[i]=currentNode;
                            gScore[i]=g_score_try;
                            evaluation[i]=gScore[i]+heuristic[i];
                            if(heapPositions[i]==-1)
                                   InsertToHeap(i);

                            else

                                   DecreaseKey(heapPositions[i],evaluation[i]);
                            TotalComputation+=4;
				     	}


				     }

     		}




	}
    runtime = GetTimer();
      if(evaluation[destination]!=infinity)
        {printf("Distance between source node  %d and node  %d is %d\n",sourceNode,destinationNode,evaluation[destination]);
        printf("the path is: \n");
        }


}
void printShortestPath(int destination){
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


}
main(){
	int repeat,count;



	buildGraph();   // first step is to create the graph from the text file
	do {
	allocate();
	heap[0]=-1;     // we start indexing at 1 (not 0) so that to be able to use the array implementation of heap; other wise, 2n might be 0..
	evaluation[heap[0]]=-infinity;
	int heuristic_choice=0;
	do{ repeat=0;
	printf("\t\tAStar Algorithm\n\t\t-----------------\n\n");
        printf("Make your choice:\n\t0) No Heuristic at all\n\t1)Perfect Heuristic\n");
        scanf("%d",&heuristic_choice);
		printf("Type a source node and a goal node seperated by space ( between 0 and %d) : ",number_of_vertices-1);
		scanf("%d %d",&sourceNode,&destinationNode);
		compute_heuristic_values(heuristic_choice);
		if(sourceNode<0 || sourceNode>=number_of_vertices|| destinationNode<0 || destinationNode>=number_of_vertices)
		{
			printf("the nodes entered should be between %d and %d\n",0,number_of_vertices-1);
			repeat=1;
		}
	}while(repeat ==1);


	Astar(sourceNode,destinationNode);   //step 2: run AStar's algorithm
     if(evaluation[destinationNode]!=infinity)
           {
             printShortestPath(destinationNode);      //step3: print the path
             printf("\n\nrunning time in seconds: %0.6f\nrunning time in milliseconds:%0.6f\n",(double)(runtime/1000.f),runtime);
	     printf("Total Number of Comparison (Initialize,Decrease Key,ExtractMin,Deletemin,InsertToHeap,ReorderHeap,MakeQueue):%d\n\n",TotalComputation);
           }
     else{
          printf("problem with the graph!! there is no path from source to destination ( graph not connected)");
     }
    memset(visited,0,number_of_vertices);
    memset(heap,0,number_of_vertices);
    memset(heapPositions,-1,number_of_vertices);
    memset(evaluation,0,number_of_vertices);
    memset(prev,0,number_of_vertices);
    memset(gScore,0,number_of_vertices);
    memset(heuristic,0,number_of_vertices);
    heapsize = 0;
    TotalComputation = 0;
    printf("\nEnter 0 to stop; 1 to continue : ");
    scanf("%d",&count);} while(count==1);
}
