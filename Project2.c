/*
Title: Financial driven project scheduling: A dynamic programming approach

Part A: Calculates all the possible paths
		and stores them in a linked list
		Input:A txt file with the names of the nodes and all the prerequicities
		Output: All the possible paths

		The names of the activities in the input file should be one letter A-Z or a-z
		This code supports maximum 52 activities.After that with sumbol : follows
		the period time of the activity and afterwords follows all the prerequicities
		of the given activity seperated by comma

		Example of the format given bellow:

			A:2/
			B:1/A
			C:1/A
			D:3/B
			a:2/A,B,D
			b:2/A,B,a
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define N 52 //maximum activities

typedef struct node//two way linked list
{
	struct node* next; //The next node of the list
	struct node* previousNode;//The previous node
	char graph[N][N];	//The possible paths for each node. Each node represents
						//the Gs = the set of all states g(s,t) at stage s
	int rows; //Indicates how many rows each graph has on each node
	float values[N];
}node;


node* head; // The start of the list
node* current; //Indicates the current possition in the linked list
node* before; // Indicates the previous possition in the linked list
char activ[N][N-1];//All the activities from the file stored in this array.
//Possible prerequicities all the others except its self(N-1)
char period[N];//Each element shows the time of each pi activity needed to complete
char TempPath[N];//Storing the path in order to search it into the next node
float npvp[N][N];
float array[N][N];

void CreateList();
void UnloadList();
void InsertNewNode();
void PrintList();
int CanBeMade(int,int); //and the first the activity and the second int indicates the row of the array in the node
void GoBack(); // Moves to the previous node
void GoForth();//Moves to the next node
int CalculatePeriod(int); //int indicates the row of the array in the node
void reversePrinting(); // ONLY FOR TESTING THE TWO WAY LINKED LIST
void MakeNewPath(int,int);
float NextNPV(); //Based on TempPath it will find in which place it is the path and will output the correct value of the path
//NPV to end
void readCashFlow(FILE*);
void PrintPath();

int main(int argc , char* argv[]) //Use of the program: ./Project_Scheduling <input_file.txt>
{
	char c,lst; //c will read each element from the filea and store it temporary
	char previous[N][N]; // Stores the  results of previous node
	int i=0,j=0,m=0,n=0,k=0,l=0,p=0,g=0; //indexes . index i will show us how many elements we have in activ
	int previousRows = 1,startTime=0; // Stores the rows of the previous node
	float npvp[9][16] = {{-34.8,-34.7,-34.5,-34.3,-34.1,-34.0,-33.8,-33.6,-33.5,-33.3,-33.1,-33.0,-32.8,-32.6,-32.5,-32.3},
						{1033.9,959.8,881.6,803.7,726.2,616.9,577.1,514.5,424.7,339.9,250.9,185.4,138.5,55.1,-37.0,-18.5},
						{-10,-9.9,-9.9,-9.8,-9.8,-9.7,-9.7,-9.6,-9.6,-9.5,-9.5,-9.4,-9.4,-9.3,-9.3,-9.2},
						{330.4,292,244.6,206.6,182.6,145,130.5,97.7,78.8,50.8,32.2,22.9,13.6,-4.9,-18.6,-32.3},
						{-44.8,-44.6,-44.3,-44.1,-43.9,-43.7,-43.5,-43.2,-43,-42.8,-42.6,-42.4,-42.2,-42,-41.8,-41.5},
						{212.9,193.4,174.1,154.9,140.3,130.4,106.8,92.5,69.1,32,22.6,4.2,-9.7,-18.8,-27.9,-36.9},
						{676.8,586.2,500.6,410.8,321.5,232.6,180.9,129.5,73.7,41.2,18.0,-0.4,-18.8,-23.3,-27.8,-18.5},
						{212.7,193.2,173.9,154.7,135.5,116.5,97.5,78.7,59.9,41.2,22.6,4.2,-14.2,-32.5,-27.8,-18.5},
						{95.2,90.2,85.1,80.1,75.1,65.6,56,46.6,37.2,27.8,18.5,9.2,4.5,-0.1,-4.7,-9.2}};




	if(argc!=3) //Checks if the arguments are correct
	{
		printf("Use of the program: ./Project_Scheduling <input_file.txt> <cashflow_file.txt> \n");
		return 1;
	}

	FILE* input = fopen(argv[1],"r"); //Opens the input file

	if(input == NULL) // Checks if input file opens correctly
	{
		printf("Something went wrong. Couldn't open input file\n");
		return 2;
	}

	FILE* cash = fopen(argv[2],"r");
	if(cash == NULL)
	{
		printf("Something went wrong. Couldn't open cashflow file(ERROR 3)\n");
		return 3;
	}

	//Begining of the transfer of inputs from file to array

	while(!feof(input)) // Read through the file
	{
		if(!feof(input)) //In order not to read the last element twice
		{
			c=fgetc(input);

			if(isalpha(c)) //Checks if it is a letter A-Z or a-z and stores it
			{
				activ[i][j++] = c;
			}

			if(isdigit(c)) // Checks if it is the period and stores it
			{
				period[i] = c - 48; //Convert number from ascii
			}

			if(c == '\n')//Indicates that the line has been read and moves to the next
			{
				i++;
				j=0;
			}
		}
	} //End of read

	CreateList(); // Creates the list
	m = 0;
	for(k=0; k<i; k++) // in this loop we put the first activities that have no prerequicities
	{
		if(activ[k][1] == '\0')
		{

			current->graph[m][0] = activ[k][0];//We store the first activities in the head
			current->graph[m][1] = '\0';
			previous[m][0] = current->graph[m][0];
			previous[m][1] = '\0';
			m++;
		}
	}

	current->rows = m; //Tells how many elements we inserted in node. We want the actuall number so we plus one
	previousRows = current->rows;

	 //False. Means that the activ can not be created yet
	int found;
	for(j=0; j<i; j++) // Passing through the activ array. We Already put the first activ so we begin from the last one we put
	{
		n=0;
		//PrintList();
		InsertNewNode(); //We create a new node and move in it

		for(m=0; m<previousRows; m++)
		{
			n=0;
			while(previous[m][n] !='\0') //We find the last element of each path
				n++;

			lst = previous[m][n-1]; //The last character of each path

			for(k=j; k<i; k++) //In this loop we check for possible activities that can be created
			{
				n=0;
				if(activ[k][n] > lst)
				{
					n++;
					while(activ[k][n] != '\0' && n < 100) //Ckecking all the prerequicities if they are already done
					{
						found = 0;
						l=0;

						while(previous[m][l] != '\0' && found == 0) //In this loop we check if a given prerequicity is done
						{																						//if it is done found will be true
							if(activ[k][n] == previous[m][l])
							{
								found = 1;
							}
							l++;
						}
						n++;

						if(found == 0) //If is even one activity which is not done then break
						{
							n = 100;
						}
					}

					if(n<100) //Activitie can be done , Passing the arguments into node
					{
						l=0;
						while(previous[m][l] != '\0' ) //First we pass all the previous activities
						{
							current->graph[current->rows][l] = previous[m][l];
							l++;
						}
						current->graph[current->rows][l] = activ[k][0]; //Passes the new Activitie that was created
						current->graph[current->rows][l+1] = '\0' ; //End of string
						current->rows++;
					}
				}
			}
		}

		//Updating previous array
		memset(previous, 0, sizeof(previous)); //Clearing the previous array.
		for(p=0; p<current->rows; p++)
		{
			g=0;
			while(current->graph[p][g]!='\0')
			{
				previous[p][g] = current->graph[p][g];
				g++;
			}
			previous[p][g] = '\0';//In order to pass the terminating char '\0'
		}
		previousRows = current->rows; //Updating Previous Rows
	}


	// ---------------- Part 2 ----------------------
	// Counting the most profitable path
	readCashFlow(cash);
	j=0;
	k=0;
	l=0;
	float nextN;
	GoBack(); //Moving back from the last node
	//printf("State In | Module | StartTime | NPV-Stage | StateOut | NPV nextStages | NPVtoend |\n\n");
	do{ //through all the nodes backwards. From the last to the first
		GoBack(); //Moving back from the last node
		for(j=0;j<current->rows;j++)//For each path in the node
		{

			for(k=0;k<i;k++)//For each activity in activ[k][0]
			{
				if(CanBeMade(j,k)==0) //activity K can be done
				{
					startTime = CalculatePeriod(j); //Counts the start time of the given path
					MakeNewPath(j,k); //Creating the state Out
					nextN = NextNPV();

					//printf("%2s + %4c,%8d,%12.1f,%14s,%12.1f,%12.1f\n",current->graph[j],activ[k][0],startTime,npvp[k][startTime-1],TempPath,nextN,npvp[k][startTime-1] + nextN);

					if(current->values[j] < ((npvp[k][startTime-1]) + nextN)) // Only the bigger
					{
						current->values[j] = ((npvp[k][startTime-1]) + nextN);
					}
				}
			}
		}
		printf("\n");

	}while(current->previousNode != NULL);


	for(j=0;j<current->rows;j++)
	{
		for(k=0;k<i;k++)
		{
			if(current->graph[j][0]==activ[k][0])
			{
				startTime = 1; //Because it's the first nodes
				//printf("   + %4s,%8d,%12.1f,%14s,%12.1f,%12.1f\n",current->graph[j],startTime,npvp[k][startTime-1],current->graph[j],current->values[j],current->values[j]+npvp[k][startTime-1]);
			}
		}

	}
	//PrintPath();
	//End of Program. Free any memory
	fclose(input);
	current = head;
	UnloadList();
	free(before);
	free(head);
	return 0;

}
//<================================= Begining of Functions ===============================>
//========================================================================================>
void PrintPath()
{
	//Prints the most profitable path

	while(current != NULL)
	{
		int i=0;
		for(i=0; i<current->rows; i++)
		{
			printf("%s  %f\n",current->graph[i],current->values[i]);
		}
		printf("\n\n");
		GoForth();
	}
}

void readCashFlow(FILE* cash)
{
	float r = 0.005;//The discount rate. Later it will be given by the user

	//finding the number of columns in cashflows.txt
	char c = fgetc(cash);
	int col = 1;
	while(c!='\n')
	{
		if(c==' ')
			col++;

		c=fgetc(cash);
	}
	if(col>N) //NPVP array right now is static and can handle 64x64 elemenets
		printf("NOT ENOUGH SPACE FOR NPVP!!!!!\n ERROR MAY OCCUR");
	rewind(cash);

	char temp[10];
	int negative = 0,j=0,k=0;//flag for negative numbers
	int i=0;
	c=fgetc(cash);
	while(!feof(cash))
	{
		if(!feof(cash))
		{

			if(c=='-')
				negative = 1; //number that follows is negative
			else if (c==' ' || c=='\n')
			{
				npvp[i][j] = (float)atoi(temp);
				memset(temp,0,sizeof(temp));
				k=0;

				if(negative == 1)
				{
					npvp[i][j] *= -1;
					negative = 0;
				}

				if(c == '\n')
				{
					j=0;
					i++;
			   	}
				else
					j++;
			}
			else if(isdigit(c))
				temp[k++]=c;
			c = fgetc(cash);
		}
	}

	 int m=0,t=0;
	 printf("%f",r);
	 for(k=0;k<i;k++)
	 {
		 printf("Activ %d\n",k);
		 for(m=0;m<col;m++)
		 {
		 	float sum = 0;

			for(t=0;t<col-m;t++)
			{
				//printf("t= %d ",t);
				sum += (npvp[k][t]/(powf((1+r),(t+1))));
			}
			sum*=(1/(powf((1+r),(float)(m))));
			array[k][m] = sum;

		 }

	 }

	 //Display npvp
	 for(k=0;k<i;k++)
	 {
		 for(m=0;m<col;m++)
		 {
		 	printf("%6.1f ",array[k][m]);
			npvp[k][m] = array[k][m];
		 }


		printf("\n");
	}

}

float NextNPV()
{
	GoForth(); //Moving to the next node
	int i=0;
	float n;
	for(i=0;i<current->rows;i++) //Check each path of the node
	{
		if(strncmp(current->graph[i],TempPath,N)==0)
		{
			n = current->values[i];
			break;
		}
	}
	GoBack();
	return n;
}

void MakeNewPath(int row,int activity)
{
	int i=0,j=0,done=0;
	while(current->graph[row][i]!='\0')
	{
		if(current->graph[row][i]<activ[activity][0] || done == 1)
		{
			TempPath[j] = current->graph[row][i];
			i++;
		}
		else
		{
			TempPath[j] = activ[activity][0];
			done = 1;
		}

		j++;
	}
	//That's only executed if the activity is the last one
	if(done == 0)
	{
		TempPath[j] = activ[activity][0];
	}
	TempPath[j+1]='\0';
}

int CalculatePeriod(int row) // row indicates which path in current node
{
	int sum=0,i=0,j=0;
	while(current->graph[row][i]!='\0') //For each activity in path
	{
		j = 0;
		while(j<N)
		{
			if(current->graph[row][i] == activ[j][0])
			{
				sum += period[j];
				j = N;
			}
			j++;
		}
		i++;
	}

	return sum+1; //Returns the sum of the total time
}


void reversePrinting()
{
	GoBack();
	do
	{
		GoBack();
		int q=0,r=0;
		for(q=0; q<current->rows; q++)
		{
			r=0;
			while(current->graph[q][r] !='\0')
			{
				printf("%c",current->graph[q][r]);
				r++;
			}
			printf("\n");
		}
		printf("\n");
	}
	while(current->previousNode !=NULL);
}



int CanBeMade(int row,int activity)
{
	//First we check if the given activity has not already done.
	//Then we check if all the prerequicities are done for the given activity
	int j=0,i=1,flag;

	while(current->graph[row][j]!='\0')//Check if it is already made
	{
		if(current->graph[row][j]== activ[activity][0])
			return 1; //False. Means that this activity is already made

		j++;
	}


	j=0;
	while(activ[activity][i]!='\0')//For every prerequicity
	{
		flag = 0;
		while(current->graph[row][j]!='\0' && flag !=1) //Check if it is already made
		{
			if(current->graph[row][j]==activ[activity][i])
			{
				flag = 1;
			}
			j++;
		}
		if(flag == 0) //If a prerequicity it is NOT made
		{
			return 1;//False
		}
		i++;
	}

return 0; // It can be made! Yosh !
}

void GoForth()
{
	current = current->next;
}

void GoBack()
{
	current = current->previousNode;
}


void PrintList()
{
	int q=0,r=0;
	for(q=0; q<current->rows; q++)
	{
		r=0;
		while(current->graph[q][r] !='\0')
		{
			printf("%c",current->graph[q][r]);
			r++;
		}
		printf("\n");
	}
	printf("\n");
}


void CreateList() // Creates the list
{
	head = malloc(sizeof(struct node));
	head->next = NULL; //Set the next node to NULL
	head->rows = 0;
	head->previousNode = NULL;

	current = malloc(sizeof(struct node));
	current = head ; // Current indicates the current location of our point in the list

	before = malloc(sizeof(struct node));
	before = head;


}


//Inserts a new node at the end of the list and moves in it
void InsertNewNode()
{
	current->next = malloc(sizeof(struct node));
	before = current;
	current = current->next; // The currnet location now goes to the next node

	current->previousNode = before; //Takes the pointer of the previous node
	current->next = NULL;
	current->rows = 0;
}

void UnloadList() //Free any memory that we used
{
	if(current->next != NULL)
	{
		current = current->next;
		UnloadList();
	}
	else
		free(current);
}
