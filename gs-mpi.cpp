#include <mpi.h>
#include <iostream>

int main (int argc, char** argv){
int rank, size;

MPI_Init (&argc, &argv); /* starts MPI */

int master = 0;	 // rank of master processor

MPI_Comm_rank (MPI_COMM_WORLD, &rank); /* get current process id */
MPI_Comm_size (MPI_COMM_WORLD, &size); /* get number of processes */
int number = 0;
if(rank == master){
	int women = (size-1)/2; // begining of the woman processors'rank
	int count = size-1;
	int flag = 0;
	/* Read input file */
	ifstream file ; 
	file.open(argv[1]);
	int n;
	file >> n ;
	for(int i=1 ; i<= 2*n ; i++)
		for(int j = 0 ; j<n; j++){
			int e;
			file>> e ;
			MPI_Send(&e, 1, MPI_INT, i, 0, MPI_COMM_WORLD);	// Sends preference lists
		}
	MPI_Barrier(MPI_COMM_WORLD); 	// Waits until all processors recieves the data

	while(!flag){
		MPI_Iprobe( MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
		if(flag){
			MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// If woman processes confirms that they're engaged
			if(number == terminate){
				women--;	// Decrease number of women that are not engaged yet
			}
		}
		flag = 0;
		// If all women are engaged
		if(women == 0){
			// Send termination signal to all processes 
			for(int i=1 ; i<=size-1; i++){
				MPI_Send(&terminate, 1, MPI_INT, i , 0, MPI_COMM_WORLD);
			}


		MPI_Finalize(); // Finalize the master 
		break;
	}
}else if(rank <= (size-1)/2){ // Men
	vector<int> prefs;	// Preference list of men

	//Recieve preference list from the master
	for(int i=0 ; i<(size-1)/2;i++){
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		prefs.push_back(number + (size-1)/2 );
	}
	MPI_Barrier(MPI_COMM_WORLD);	//  Waits until all processors recieves the data
}else{	// Women
	vector<int> prefs;	// Preference list of men

	//Recieve preference list from the master
	for(int i=0 ; i<(size-1)/2;i++){
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		prefs.push_back(number);
	}
	MPI_Barrier(MPI_COMM_WORLD);	//  Waits until all processors recieves the data



return 0;
}