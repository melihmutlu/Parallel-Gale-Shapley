#include <mpi.h>
#include <iostream>

int main (int argc, char** argv){
int rank, size;

MPI_Init (&argc, &argv); /* starts MPI */

int master = 0;	 // rank of master processor

MPI_Comm_rank (MPI_COMM_WORLD, &rank); /* get current process id */
MPI_Comm_size (MPI_COMM_WORLD, &size); /* get number of processes */

if(rank == master){
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