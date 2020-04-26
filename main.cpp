/******************************************************
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
int count = 0;
vector <int> buffer;

//function parameters
void init();
void *producer(void *arg);
void *consumer(void *arg);
int insert(int item);
void printBuffer();
int remove();
bool isEmpty();
bool isFull();

/*******************************************************************
Purpose: Used to make sure the user enters the right amound of parameters into the command line. The arguments entered are the sleep time, number of producer threads, and number of producer threads

Parameters:
int argc = amount of arguments
char *argv[] = an array of characters that is used for each argument
*******************************************************************/
int main(int argc, char *argv[])
{
  cout << "---------------------------------------------------" << endl;
  cout << "CS433: Programming Assignment 4" << endl;
  cout << "Authors: Dionisio de Leon and James Foerster" << endl;
  cout << "Due Date: 04/30/2020" << endl;
  cout << "Course: CS433 (Operating Systems)";
  cout << "This program is to implement the Producer-Consumer problem using pthre\
ads" << endl;
  cout << "---------------------------------------------------" << endl << endl;

  if(argc != 4)
    {
      cout << "Error: Must enter 3 arguments in the command line!" << endl;
      return 0;
    }

  main_sleep_time = atoi(argv[1]);
  numProducer = atoi(argv[2]);
  numConsumer = atoi(argv[3]);

  //If user enters values less than 0
  if(main_sleep_time <= 0 || numProducer <= 0 || numConsumer <= 0)
    {
      cout << "Error: User must enter values greater than 0" << endl;
      return 0;
    }

  //testing to see if the user input matches with everythinng else so far
  cout << "Sleep time: " << main_sleep_time << endl;
  cout << "# of producer threads: " << numProducer << endl;
  cout << "# of consumer threads: " << numConsumer << endl;

  //Creating mutexes and semaphores
  init();

  //creating threads for pthread_create(), which is used to create new threads
  pthread_t p_threads[numProducer];
  pthread_t c_threads[numConsumer];

  //pthread_create() uses 4 parameters
  //1) thread id 
  //2) thread attribute (NULL = default attributes)
  //3) start: thread begins executing code at this address
  //4) argument passed to start (NULL)

  int i = 0;

  while(i < numProducer)//creating producer threads
    {
      cout << "test: create Numproducer" << endl;
      pthread_create(&p_threads[i], NULL, producer , NULL);
      cout <<"test: D" << endl;
      i++;
    }

  int z = 0;

  while(z < numConsumer)//creating consumer threads
    {
      cout << "test: create Numconsumer" << endl;
      pthread_create(&c_threads[z], NULL, consumer, NULL);
      z++;
    }

  return 0;
}

//Functions
/***************************************************
purpose: creating and initializing mutex and semaphores
*******************************************/
void init()
{
  cout <<"test init()" << endl;
  pthread_mutex_init(&mutex, NULL);//Creating mutex lock
  sem_init(&empty, 0, 5);//creating empty semaphore w/ buffer size = 5
  sem_init(&full, 0, 0);//creating full semaphore
}

/**********************************************************
Purpose: Producer sleeps for a certain time while ddecrementing the pointer to empty. Then the mutex will lock as we enter the critical section. A random number is then generated and inserted into the buffer. Afterwards we exit the critical section where the mutex is unlocked and the semaphore increments the pointer to full

Parameter: arg is passed into producer from the argument in pthreads_create(), which is NULL in this case
******************************************************/
void *producer(void *arg)
{
  cout <<"calling *producer()" << endl;

  int item;

  while(true)
    {
      cout <<"while loop" << endl;
      sleep(rand() % main_sleep_time);//sleep for random period of time
      sem_wait(&empty);
      pthread_mutex_lock(&mutex);//acquiring the mutex lock

      //start of critical section
      item = rand() % 100;
      cout <<"item: " << item << endl;

      if(insert(item) == true)//book
        {
	  cout <<"item " << item << " inserted by a producer"<< endl;
	  printBuffer();
        }
      else
        cout <<"item " << item << " cannot be inserted by a producer"<< endl;

      //end of critical section
      
      pthread_mutex_unlock(&mutex);//releasing mutex lock
      sem_post(&full);//releasing the semaphore
    }
}
/****************************************************************
Purpose: Consumer sleeps for a certain amount of time incrementing the pointer tofull. Then the mutex will lock as we enter the critical section. Consumer will now remove from the buffer assuming the list isn't empty. Afterwarrds we exit the critical section where the mutex is unlocked and the semaphore drcements pointer to empty  

Parameter: *arg is passed into consumer from the 4th arugment in pthreads_create()
**************************************************************/
void *consumer(void *arg)
{
  cout << "calling *consumer()" << endl;
  
  int item;
  while(true)
    {
      sleep(rand() % main_sleep_time);
      sem_wait(&full);
      pthread_mutex_lock(&mutex);

      //Start of critical section
      
      item = remove();
      if(item != false)
        {
          
          cout << "test" << endl;
          printBuffer();
        }
      else
        {
          cout << item << endl;
          cout << "test: print" << endl;
        }

      //End of Critical section
      cout << " end of critical section" << endl;
      pthread_mutex_unlock(&mutex);
      sem_post(&empty);
    }
}
/********************************************************
purpose: Inserting an item to the end of the buffer

parameter: int item = item that gets inserted into buffer
**********************************************************/
int insert(int item)
{
  cout <<"calling insert()" << endl;

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
  cout <<"printBuffer()" << endl;

  cout << "[";
  for(int i = 0; i < buffer.size(); i++)
    cout << buffer.at(i) << " " << endl;
  cout << " ]" << endl;
}

/********************************************************
purpose: Removes the first element of the buffer. This function returns that remo\
ved element
*********************************************************/
int remove()
{
  cout <<"calling remove()" << endl;

  int temp;

  if(!isEmpty())//if buffer is not empty, remove first item
    {
      temp = buffer.front();
      buffer.erase(buffer.begin());
      count--;
      return temp;
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
  else
    return true;
}
/****************************************************************
Purpose: Checking if the buffer is empty. Count must not equal 0
**************************************************************/
bool isEmpty()
{
  if(count != 0)
    return false;
  else
    return true;
}
