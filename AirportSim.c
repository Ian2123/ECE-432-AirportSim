/*
   Author: Ian Isely
   Date: 12/6/19
	This program simulates an airport's operation by having multiple planes competing for
        runway space while not crashing into each other. One thread is equal to one plane
        of which there are multiple types, each type having its own runway. Planes also
        have passengers which take time to load and unload. If a runway is full, then incoming
        planes must wait for available space.
   Contributors: Ian Isely
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAX_PLANES 50
#define MAX_RUNWAYS 5

// Global Variables
int Runway_Capacity, Num_Runways, Runway_Counts[MAX_RUNWAYS];
pthread_mutex_t SIU_mutex, ECE_mutex, ENG_mutex, ILL_mutex, COM_mutex;
pthread_mutex_t SIU_Space_Mutex, ECE_Space_Mutex, ENG_Space_Mutex, ILL_Space_Mutex, COM_Space_Mutex;
pthread_cond_t SIU_Space, ECE_Space, ENG_Space, ILL_Space, COM_Space; 

// Function Prototypes
void * Airport(void *);

// Plane Statistics 
struct Airplane_Info{
   char company[3];
   int passengers;
};

int main(int argc, char**argv){
   int random_company, i;

   // Mutex Initializations
   pthread_mutex_init(&SIU_mutex, NULL);
   pthread_mutex_init(&ECE_mutex, NULL);
   pthread_mutex_init(&ENG_mutex, NULL);
   pthread_mutex_init(&ILL_mutex, NULL);
   pthread_mutex_init(&COM_mutex, NULL);

   pthread_mutex_init(&SIU_Space_Mutex, NULL);
   pthread_mutex_init(&ECE_Space_Mutex, NULL);
   pthread_mutex_init(&ENG_Space_Mutex, NULL);
   pthread_mutex_init(&ILL_Space_Mutex, NULL);
   pthread_mutex_init(&COM_Space_Mutex, NULL);

   // Condition Variable Initializations
   pthread_cond_init(&SIU_Space, NULL);
   pthread_cond_init(&ECE_Space, NULL);
   pthread_cond_init(&ENG_Space, NULL);
   pthread_cond_init(&ILL_Space, NULL);
   pthread_cond_init(&COM_Space, NULL);

   Num_Runways = atoi(argv[1]);
   Runway_Capacity = atoi(argv[2]);
   for(i = 0; i < MAX_RUNWAYS; i++) 
      Runway_Counts[i] = 0;

   srand(time(NULL));
   
   while(1){
      pthread_t Plane;
      struct Airplane_Info Statistics;

      Statistics.passengers = rand()%200 + 1;

      random_company = rand()%Num_Runways;
      switch(random_company){
         case 0: strcpy(Statistics.company, "SIU");
            	 break;

	 case 1: strcpy(Statistics.company, "ECE");
                 break;
        
         case 2: strcpy(Statistics.company, "ENG");
                 break;

         case 3: strcpy(Statistics.company, "ILL");
                 break;

         case 4: strcpy(Statistics.company, "COM");
                 break;
      }

      printf("Plane: %s with %d passengers arriving\n", Statistics.company, Statistics.passengers);
      pthread_create(&Plane, NULL, Airport, (void*)&Statistics);
      
      sleep(1);
   }

   pthread_exit(NULL);
}

void * Airport(void * Plane_Attr){
   char my_company[3];
   int my_passengers;
   struct Airplane_Info * my_stats;

   // Reassign Structure Variables
   my_stats = (struct Airplane_Info *)Plane_Attr;
   strcpy(my_company, my_stats->company);
   my_passengers = my_stats->passengers;

   srand(time(NULL));

   // Enter Airport System

   if(strcmp(my_company, "SIU")){
      while(Runway_Counts[0] == Runway_Capacity)
      {
         printf("Plane: %s with %d passengers waiting to land\n", my_company, my_passengers);
         pthread_cond_wait(&SIU_Space, &SIU_Space_Mutex);
      }

      printf("Plane: %s with %d passengers landing\n", my_company, my_passengers);

      pthread_mutex_lock(&SIU_mutex);
      Runway_Counts[0]++;
      pthread_mutex_unlock(&SIU_mutex);

      sleep(my_passengers/10+1); // Landing Time

      pthread_mutex_lock(&SIU_mutex);
      Runway_Counts[0]--;
      pthread_cond_signal(&SIU_Space);
      pthread_mutex_unlock(&SIU_Space_Mutex);
      pthread_mutex_unlock(&SIU_mutex);

      printf("Plane: %s departing runway\n", my_company);
   }
   else if(strcmp(my_company, "ECE")){
      while(Runway_Counts[1] == Runway_Capacity)
      {
         printf("Plane: %s with %d passengers waiting to land\n", my_company, my_passengers);
         pthread_cond_wait(&ECE_Space, &ECE_Space_Mutex);
      }

      printf("Plane: %s with %d passengers landing\n", my_company, my_passengers);

      pthread_mutex_lock(&ECE_mutex);
      Runway_Counts[1]++;
      pthread_mutex_unlock(&ECE_mutex);

      sleep(my_passengers/10+1); // Landing Time

      pthread_mutex_lock(&ECE_mutex);
      Runway_Counts[1]--;
      pthread_cond_signal(&ECE_Space);
      pthread_mutex_unlock(&ECE_Space_Mutex);
      pthread_mutex_unlock(&ECE_mutex);

      printf("Plane: %s departing runway\n", my_company);
   }
   else if(strcmp(my_company, "ENG")){
      while(Runway_Counts[2] == Runway_Capacity)
      {
         printf("Plane: %s with %d passengers waiting to land\n", my_company, my_passengers);
         pthread_cond_wait(&ENG_Space, &ENG_Space_Mutex);
      }

      printf("Plane: %s with %d passengers landing\n", my_company, my_passengers);

      pthread_mutex_lock(&ENG_mutex);
      Runway_Counts[2]++;
      pthread_mutex_unlock(&ENG_mutex);

      sleep(my_passengers/10+1); // Landing Time

      pthread_mutex_lock(&ENG_mutex);
      Runway_Counts[2]--;
      pthread_cond_signal(&ENG_Space);
      pthread_mutex_unlock(&ENG_Space_Mutex);
      pthread_mutex_unlock(&ENG_mutex);

      printf("Plane: %s departing runway\n", my_company);
   }
   else if(strcmp(my_company, "ILL")){
      while(Runway_Counts[3] == Runway_Capacity)
      {
         printf("Plane: %s with %d passengers waiting to land\n", my_company, my_passengers);
         pthread_cond_wait(&ILL_Space, &ILL_Space_Mutex);
      }

      printf("Plane: %s with %d passengers landing\n", my_company, my_passengers);
    
      pthread_mutex_lock(&ILL_mutex);
      Runway_Counts[3]++;
      pthread_mutex_unlock(&ILL_mutex);

      sleep(my_passengers/10+1); // Landing Time

      pthread_mutex_lock(&ILL_mutex);
      Runway_Counts[3]--;
      pthread_cond_signal(&ILL_Space);
      pthread_mutex_unlock(&ILL_Space_Mutex);
      pthread_mutex_unlock(&ILL_mutex);

      printf("Plane: %s departing runway\n", my_company);
   }
   else if(strcmp(my_company, "COM")){
      while(Runway_Counts[4] == Runway_Capacity)
      {
         printf("Plane: %s with %d passengers waiting to land\n", my_company, my_passengers);
         pthread_cond_wait(&COM_Space, &COM_Space_Mutex);
      }

      printf("Plane: %s with %d passengers landing\n", my_company, my_passengers);

      pthread_mutex_lock(&COM_mutex);
      Runway_Counts[4]++;
      pthread_mutex_unlock(&COM_mutex);

      sleep(my_passengers/10+1); // Landing Time

      pthread_mutex_lock(&COM_mutex);
      Runway_Counts[4]--;
      pthread_cond_signal(&COM_Space);
      pthread_mutex_unlock(&COM_Space_Mutex);
      pthread_mutex_unlock(&COM_mutex);

      printf("Plane: %s departing runway\n", my_company);
      
   }
   else printf("Error");

   // Leave Airport System 
}



