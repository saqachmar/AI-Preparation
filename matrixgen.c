#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

int **allocate_array(int row_dim, int col_dim) 
{
  int **result;
  int i,j;

  /* Allocate an array of pointers to hold pointers to the rows of the
	 array */
  result=(int **)malloc(row_dim*sizeof(float *));

  /* The first pointer is in fact a pointer to the entire array */
  result[0]=(int *)malloc(row_dim*col_dim*sizeof(float));

  /* The remaining pointers are just pointers into this array, offset
	 in units of col_dim */
  for(i=1; i<row_dim; i++)
  {
	result[i]=result[i-1]+col_dim;
	//memset(result[i], 0, col_dim * sizeof(float));
  }

for(i=0;i<row_dim;i++)
	for(j=0;j<col_dim;j++)
		result[i][j] = 0 ;

  return result;
}

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

#define N 32000
int main(int argc, char **argv)
{

  int **array;
  int i,j, maxedge_cost = 384,offset=128,edge_per_vertex;
  int edgecount = 0,nlogn = N * (log10((float)N)/log10((float)(2)));
  edge_per_vertex = nlogn/N;
  array = allocate_array(N,N);	
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			array[i][j] = -1;
		}
	}

	srand(time(NULL));

	int count = 0;
  	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{			
			// dense graph 
			//if (j > i && (rand() % 2 == 1))
			//fullyconnected graph
			//if (j > i)
			//sparse graph
  			if (j > i && (rand() % 2 == 1))
			{
				array[i][j] = rand() % N + 1;									
			 	count++;
				edgecount++;
			}
		}
		count = 0;
	}

  for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (i > j)
				array[i][j] = array[j][i];
			if(i==j)
				array[i][j]=0;	
		}
	}


  FILE *fp;
  fp = fopen("dense_dynamicedge_32000.txt","w");
//    fp = fopen("TestGraph.txt","w");
  printf("vertex = %d\t edge = %d\n",N,edgecount);
  fprintf(fp, "%d\n", N);
  for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			fprintf(fp, "%d ", array[i][j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
  deallocate_array(array,N);

  return 0;
}
