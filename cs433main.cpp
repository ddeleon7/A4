#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      
#include <pthread.h>     // Required for pthreads
#include <semaphore.h>	 
#include <iostream>
#include <sys/wait.h>
using namespace std;

//parameters


int main(int argc, char *argv[])
{
	cout << "---------------------------------------------------" << endl;
	cout << "CS433: Programming Assignment 4" << endl;
	cout << "Authors: Dionisio de Leon and James Foerster" << endl;
	cout << "Due Date: 04/28/2020" << endl;
	cout << "Course: CS433 (Operating Systems)"
	cout << "This program is to implement the Producer-Consumer problem using pthreads" << endl;
	cout << "---------------------------------------------------" << endl << endl;

    return 0;
}