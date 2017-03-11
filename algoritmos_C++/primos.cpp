#include <iostream>

#include "mpi.h"

#include <stdio.h>

#include <stdlib.h>

using namespace std;

int Factores[2];


/**
El algoritmo realiza la ejecución paralela únicamente con uno o dos procesos, si es un proceso, entonces lo ejecuta de manera secuencial, en caso contrario, los primos del número a factorizar son obtenidos de manera paralela, donde cada proceso (de los dos que existen) sincroniza sus resultados para encontrar los números primos. La operación se realiza a través de una variable compartida, que contendrá el número y sus factores equivalentes.
**/

//Encuentra los 2 factores principales del numero
void encontrar_factor(int num){
	for(int i = 2;i<=num/i;i++){
		if(num%i==0){
			Factores[0] =  num/i;
			Factores[1] = i;
			break;
		}

	}
}

//Obtiene los valores primos del numero
void  sacar_Primos(int num){
	int y = 2;
	while(num != 1){
		if(num%y == 0){
			num = num/y;
			printf("%i, ", y);
		}
		else{
			y++;
		}
	}
}

int main(int argc, char *argv[])
{
    int nprocs,proc;  //ID y # de procedimientos
    
    int num;
    cin>>num;
 	
    MPI_Status status;
    double time1,time2;
    MPI::Init(argc, argv);  //Inicializa el proceso en el cluster, el paralelismo.
    proc =  MPI::COMM_WORLD.Get_rank(); //ID del proceso que se esta ejecutando.
    nprocs = MPI::COMM_WORLD.Get_size(); //Número de procesos que se estan ejecutando.
    if(proc == 0){  // Condiciones para controlar el proceso que se estaba ejecutando.
	//Si el proceso es igual a cero eso indica que es el master, sino trasmite al slave
	if(nprocs==2){
		printf("Factores con 2 procesos de %d son: ",num);
		
		if(num % 2 == 0){
		cout<<" 2, ";
		}
		if(num % 3 == 0){
		cout<<" 3, ";
		}

		encontrar_factor(num);
		sacar_Primos(Factores[0]);
	}
	else if(nprocs==1){
		printf("Factores con un proceso de %d son: ",num);
                sacar_Primos(num);
	}
	else{
		printf("Solo se puede hacer con uno o dos procesos.");
		MPI::Finalize();  //Termina la ejecución del cluster.
		exit(-1);         //Finaliza el programa.
	}
    }
    MPI_Bcast(&Factores,2,MPI_INT,0,MPI_COMM_WORLD);   //IMPORTANTE: Instruccion para compartir variables entre procesos.
    MPI::Finalize();
    return 0;
}

