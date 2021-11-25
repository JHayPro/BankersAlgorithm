#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int* resourceVector, * availableVector,
** maxClaimsArray, ** allocationArray, ** needArray,
processes, resources;

bool* safelySequenced;

void buildGraph() {//builds graphs based on user input

	//Prints and reads in data
	printf("Enter Number Of Processes: ");
	scanf("%d", &processes);
	printf("Enter Number Of Resources: ");
	scanf("%d", &resources);

	//Allocates memory to vectors
	resourceVector = (int*)malloc(resources * sizeof(int));
	availableVector = (int*)malloc(resources * sizeof(int));
	safelySequenced = (bool*)malloc(processes * sizeof(bool));
	maxClaimsArray = (int**)malloc(processes * sizeof(int*));
	allocationArray = (int**)malloc(processes * sizeof(int*));
	needArray = (int**)malloc(processes * sizeof(int*));

	//constructs vectors and assigns values
	for (int i = 0; i < resources; i++) {
		resourceVector[i] = 0;
		availableVector[i] = 0;
	}
	for (int i = 0; i < processes; i++)
	{
		maxClaimsArray[i] = (int*)malloc((resources) * sizeof(int));
		allocationArray[i] = (int*)malloc((resources) * sizeof(int));
		needArray[i] = (int*)malloc((resources) * sizeof(int));
		safelySequenced[i] = false;

		for (int j = 0; j < resources; j++) {
			maxClaimsArray[i][j] = 0;
			allocationArray[i][j] = 0;
			needArray[i][j] = 0;
		}
	}

	//asks for and assigns resources
	printf("Enter Number Of Units For Resources (R0 to R%d%s", resources - 1, "): ");
	for (int i = 0; i < resources; i++)
		scanf("%d", &resourceVector[i]);

	//asks for and assigns max processes
	for (int i = 0; i < processes; i++) {
		printf("Enter Maximum Number Of Units Process P%d%s%d%s", i, " Will Request From Each Resource (R0 to R", resources - 1, "): ");
		for (int j = 0; j < resources; j++)
			scanf("%d", &maxClaimsArray[i][j]);
	}

	//asks for and assigns allocated resources
	for (int i = 0; i < processes; i++) {
		printf("Enter Number Of Units Of Each Resource (R0 to R%d%s%d%s", resources - 1, ") Allocated To Process P", i, "): ");
		for (int j = 0; j < resources; j++)
			scanf("%d", &allocationArray[i][j]);
	}

	//calculates values for available vector
	for (int i = 0; i < resources; i++) {
		for (int j = 0; j < processes; j++)
			availableVector[i] += allocationArray[j][i];

		availableVector[i] = resourceVector[i] - availableVector[i];
	}
	//calculates values fro needArray
	for (int i = 0; i < processes; i++)
		for (int j = 0; j < resources; j++)
			needArray[i][j] = maxClaimsArray[i][j] - allocationArray[i][j];

	//Displays resources
	printf("\nResources:\n");
	for (int i = 0; i < resources; i++) {
		printf("\tR%d", i);
	}
	printf("\n");
	for (int i = 0; i < resources; i++) {
		printf("\t%d", resourceVector[i]);
	}

	//Displays available vector
	printf("\n\nAvailable:\n");
	for (int i = 0; i < resources; i++) {
		printf("\tR%d", i);
	}
	printf("\n");
	for (int i = 0; i < resources; i++) {
		printf("\t%d", availableVector[i]);
	}

	//Displays max claim array
	printf("\n\nMax Claim:\n");
	for (int i = 0; i < resources; i++) {
		printf("\tR%d", i);
	}
	for (int i = 0; i < processes; i++) {
		printf("\nP%d", i);

		for (int j = 0; j < resources; j++) {
			printf("\t%d", maxClaimsArray[i][j]);
		}
	}

	//Displays allocated array
	printf("\n\nAllocated:\n");
	for (int i = 0; i < resources; i++) {
		printf("\tR%d", i);
	}
	for (int i = 0; i < processes; i++) {
		printf("\nP%d", i);

		for (int j = 0; j < resources; j++) {
			printf("\t%d", allocationArray[i][j]);
		}
	}

	//Displays need array
	printf("\n\nNeed:\n");
	for (int i = 0; i < resources; i++) {
		printf("\tR%d", i);
	}
	for (int i = 0; i < processes; i++) {
		printf("\nP%d", i);

		for (int j = 0; j < resources; j++) {
			printf("\t%d", needArray[i][j]);
		}
	}
};

void findSequence() {//attempts to find safe sequence

	for (int failCount = 0; failCount < processes; failCount++) {//repeats calculation until the failcount = the processes count
		for (int i = 0; i < processes; i++) {//checks each process
			if (safelySequenced[i] == false) {//if it has not been safely sequenced, check if it can be sequenced

				//Displays checking message
				printf("\nChecking: < ");
				for (int j = 0; j < resources; j++)
					printf("%d%s", needArray[i][j], " ");
				printf("> <= < ");
				for (int j = 0; j < resources; j++)
					printf("%d%s", availableVector[j], " ");
				printf("> ");

				//checks if each value in the current process in the needarray is less than the avaliablevector
				for (int j = 0; j < resources; j++)
					if (needArray[i][j] > availableVector[j]) {
						safelySequenced[i] = false;//if the process cannot be safely sequenced, set safelysquenced to false and break the loop
						break;
					}
					else
						safelySequenced[i] = true;//marks the the process has been safely sequenced for the current resource

				if (safelySequenced[i] == true) {//if all resources are safely sequenced
					printf(":P%d%s", i, " Safely Sequenced");

					for (int j = 0; j < resources; j++)//adds all resources of the availablevector to the allocationarray for the current process
						availableVector[j] += allocationArray[i][j];
				}
			}
		}
	}

	for (int i = 0; i < processes; i++)//fail condition if a safe squence for all processes cannot be found
		if (!safelySequenced[i]) {
			printf("\n\nNo Safe Squence Found");
			break;
		}

};

void freeMemory() {//frees memory

	for (int i = 0; i < processes; i++) {//frees 2d vector memory
		free(maxClaimsArray[i]);
		free(allocationArray[i]);
		free(needArray[i]);
	}
	//frees remaining pointer memory
	free(maxClaimsArray);
	free(allocationArray);
	free(needArray);
	free(resourceVector);
	free(availableVector);
	free(safelySequenced);
};

int main() {

	int userIn = 0;
	bool dataEntered = false;

	do {//loops until user enters 3, prints message and reads in value after each loop
		if (userIn == 3)
			break;

		else if (userIn == 2)
			if (dataEntered)//checks that data is entered, before loooking for a squence
				findSequence();//attempts to find safe sequence
			else
				printf("\nNo Data Entered");

		else if (userIn == 1) {
			if (dataEntered)//if data is already entered, clear the memory first
				freeMemory();//frees memory

			buildGraph();//builds graphs based on user input
			dataEntered = true;
		}

	} while (printf("\n\nBanker's Algorithm\n-------------------\n1) Enter Parameters\n2) Determine Safe Squence\n3) Quit Prgoram\n\nEnter Selection: ") && scanf("%d", &userIn));

	printf("\nQuitting Program...\n\n");
	freeMemory();//frees memory
	return 0;
};
