/******************************************************
how to compile: g++ main.cpp -lpthread (enter)
(-lpthread enables us to use sem_init)

how to run this code:
make a makefile
./prog4 X X X
X = int greater than 0

 ****************************************************/
#include <stdio.h>
#include <stdlib.h>    //Required for rand()
#include <unistd.h>    //Required for sleep()  
#include <pthread.h>   // Required for pthreads (pthread_mutex_t)
#include <semaphore.h> // Required for semaphores (sem_t, sem_init)
#include <iostream>
#include <sys/wait.h> //Required for wait()
#include <vector>

using namespace std;

//initializing variables
int main_sleep_time;
int numProducer;
int numConsumer;
pthread_mutex_t mutex;
sem_t empty;
sem_t full;

//Initiliazing variables for the buffer
vector <int> buffer;
int count = 0;

//function parameters
void init();
void *producer(void *arg);
void *consumer(void *arg);
int insert(int item);
void printBuffer();
int remove();
bool isEmpty();
bool isFull();

/**********************************************************************
Parameters:
int argc = amount of arguments
char *argv[] = an array of characters

 *******************************************************************/
int main(int argc, char *argv[])
{
  cout << "---------------------------------------------------" << endl;
  cout << "CS433: Programming Assignment 4" << endl;
  cout << "Authors: Dionisio de Leon and James Foerster" << endl;
  cout << "Due Date: 04/28/2020" << endl;
  cout << "Course: CS433 (Operating Systems)";
  cout << "This program is to implement the Producer-Consumer problem using pthreads" << endl;
  cout << "---------------------------------------------------" << endl << endl;

  if(argc != 4)
    {
      cout << "Error: Must enter 3 argumets in the command line!" << endl;
      return 0;//segmentation fault if I don't include return 0
    }

  main_sleep_time = atoi(argv[1]);
  numProducer = atoi(argv[2]);
  numConsumer = atoi(argv[3]);

  //User enters values less than 0
  if(main_sleep_time <= 0 || numProducer <= 0 || numConsumer <= 0)
    {
      cout << "Error: User must enter values greater than 0" << endl;
      return 0;
    }

  //testing to see if the user input matches with everythinng else so far
  cout << "Sleep time: " << main_sleep_time << endl;
  cout << "# of producer threads: " << numProducer << endl;
  cout << "# of consumer threads: " << numConsumer << endl;

  //figure out how to make a mutex and semaphore
  init();

  //creating threads for pthread_create() - used to create a new thread
  pthread_t p_threads[numProducer];
  pthread_t c_threads[numConsumer];

  //need to figure out how pthread_create() works...
  //according to google it uses 4 parameters
  //1) thread id 2)thread attribute (NULL = default attributes
  //3) start - thread begins executing code at this address
  //4) argument passed to start (NULL)
  
  int i = 0;

  while(i < numProducer)//creating producer threads
    {
      cout << "test: create producer" << endl;
      pthread_create(&p_threads[i], NULL, producer , NULL);
      i++;
    }

  int y = 0;

  while(y < numConsumer)//creating consumer threads
    {
      cout << "test: create consumer" << endl;
      //pthread_create(c_threds[i], NULL, consumer, NULL);
      y++;
    }

  return 0;
}

//Functions

/***************************************************
code from the book

purpose: initializing everthing
 *******************************************/
void init()
{
  pthread_mutex_init(&mutex, NULL);//Create mutex LOCK
  sem_init(&empty, 0, 5);//create empty semaphore w/ buffer size = 5
  sem_init(&full, 0, 0);//create full semaphore
}

/**********************************************************
Purpose: pointer to producer function to create a thread

Parameter: arg is passed into producer from the argument in pthreads_create() NULL 
 ******************************************************/
void *producer(void *arg)
{
  int item;//from the book
  
  while(true)
    {
      //sleep for random period of time
      sleep(rand() % main_sleep_time);
      sem_wait(&empty);//something from the book
      pthread_mutex_lock(&mutex);//acquire the mutex lock

      //start of critical section
      item = rand() % 100;//random number generation(i randomly picked 100)
      
      //not printing...
      if(insert(item) == true)//book
	{
	  cout << "test" << endl;
	  printf("test");
	  printBuffer();
	}
      else
	cout << "test: item cannot be inserted by producer: " << item << endl;

      //end of critical section
      pthread_mutex_unlock(&mutex);//from the book
      sem_post(&full);
    }
}
/****************************************************************
Purpose:


 **************************************************************/
void *consumer(void *arg)
{
  int item;
  while(true)
    {
      sleep(rand() % main_sleep_time);
      sem_wait(&full);
      pthread_mutex_lock(&mutex);
      
      //Start of critical section
      cout << "start of critical section" << endl;
      if(item != false)
	{
	  cout << item << endl;
	  cout << "test" << endl;
	  printf("printf test\n ");
	  printBuffer();
	}
      else
	{
	  cout << item << endl;
	  cout << "test: print" << endl;
	}

      //End of Critical section
      cout << " end of critical section" << endl;
      pthead_mutex_unlock(&mutex);
      sem_post(&empty);

}
/********************************************************
purpose: Remove the first element from the buffer. Return this removed element

parameter: int item = item that gets inserted into buffer
**********************************************************/
int insert(int item)
{
  if(!isFull())//assuming buffer isn't full, then insert item
    {
      buffer.push_back(item);
      count++;
      return true;
    }
  else
    return false;
}

/**********************************************************************
Purpose: Prints out the buffer
 ********************************************************************/
void printBuffer()
{
  printf("test123\n");
  cout << "[";
  for(int i = 0; i < buffer.size(); i++)
    cout << buffer.at(i) << " " << endl;
  cout << " ]" << endl;
}

/********************************************************
purpose: Removes the first element of the buffer. This function returns that removed element
*********************************************************/
int remove()
{
  int temp;

  if(!isEmpty())//if buffer is not empty, remove first item
    {
      //to work on....
    }

  else
    return false;
}

/*********************************************************
Purpose: Checking the buffer if its full. Count has to be a value less than 5
 ***********************************************/
bool isFull()
{
  if(count < 5)
    return false;
  return true;
}

/****************************************************************
Purpose: Checking if the buffer is empty. Count must not equal 0
 **************************************************************/
 bool isEmpty()
{
  if(count != 0)
    return false;
  return true;
}



