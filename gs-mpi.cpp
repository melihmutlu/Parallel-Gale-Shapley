#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm> 


int main (int argc, char** argv){
int rank, size;

MPI_Init (&argc, &argv); /* starts MPI */

int master = 0;	 // rank of master processor
int terminate = -2;

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


		}
	}
}else if(rank <= (size-1)/2){ // Men
	vector<int> prefs;	// Preference list of men
	int woman2prop = 0; // next woman to be proposed
	int flag = 0;

	//Recieve preference list from the master
	for(int i=0 ; i<(size-1)/2;i++){
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		prefs.push_back(number + (size-1)/2 );
	}
	MPI_Barrier(MPI_COMM_WORLD);	//  Waits until all processors recieves the data

	// Send first proposes to women that are first in the list
	MPI_Send(&rank, 1, MPI_INT, prefs[woman2prop], 0, MPI_COMM_WORLD);
	woman2prop++; // Increase the pointer

	//Listen the answer from a woman or signal from the master
	while(!flag){
		MPI_Iprobe( MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
		if(flag){
			MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(number == terminate){ // If Recived data from master
				// Break the while
				break;
			}else{	// If recieved data from a woman
				// This means this man is rejected. Propose the next woman
				MPI_Send( &rank , 1, MPI_INT, prefs[woman2prop], 0, MPI_COMM_WORLD);
				woman2prop++;
			}
		}
		flag = 0;
	}

	MPI_Finalize();	// Finalize the man
}else{	// Women
	vector<int> prefs;	// Preference list of men
	int flag = 0;
	int isEngaged = 0;	// It is 0 if not engaged yet. Otherwise the rank of the man that this woman engaged with
	
	//Recieve preference list from the master
	for(int i=0 ; i<(size-1)/2;i++){
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		prefs.push_back(number);
	}
	MPI_Barrier(MPI_COMM_WORLD);	//  Waits until all processors recieves the data

	//Listen the proposal from a man or signal from the master
	while(!flag){
		MPI_Iprobe( MPI_ANY_SOURCE , 0 , MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
		if(flag){
			MPI_Recv(&number, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			flag = 0;
			if(number == terminate){	// Recieved data from the master
				// Break the while
				break;	
			}else{	// Recieved data from a man
				if(!isEngaged){	// The woman is not engaged yet
					isEngaged = number;	// Store the rank of the man
					MPI_Send(&terminate, 1, MPI_INT, 0 , 0, MPI_COMM_WORLD);
				}else{	// Recieved a proposal but already engaged
					
					_old = find (prefs.begin(), prefs.end(), isEngaged);	//Iterator for the old man in the preference list
					_new = find (prefs.begin(), prefs.end(), number);		//Iterator for the new man in the preference list
					
					if(_old < _new)	// If old man comes before the new one
						MPI_Send(&rejection, 1, MPI_INT, number , 0, MPI_COMM_WORLD);	// Reject the new man
					else{ // Otherwise
						MPI_Send(&rejection, 1, MPI_INT, isEngaged, 0, MPI_COMM_WORLD);	// Reject the old man
						isEngaged = number;	// Engaged with new man
					}	
				}
			}
		}
		flag = 0;
	}


return 0;
}