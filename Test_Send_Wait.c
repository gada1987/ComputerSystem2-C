/*A test for the send_wait function*/ 

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "kernel_functions_march_2019.h"
#include "kernel_functions.c"
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <math.h> 
 
 
 mailbox* mBox; 
 int testValue = 0; 
 int nVar = 1; 
 int receive = 12; 
 
 
 void task1(){ 
   int data = 1; 
   receive_wait(mBox,&data);                          //receive message to mailbox
   terminate(); 
} 
 
 
 void task2(){ 
   send_wait(mBox,&receive);                        //receive is pointer to memory era
   terminate();                                    //terminate the running task,all data structure will be removed
 } 
 
 
int main(void){ 
      init_kernel();                             //initialzes the kernel 
       create_task(task1,8);                    //create task 1 with deadline = 8
       create_task(task2,16);                  //create task 1 with deadline =  16
       mBox = create_mailbox(10,sizeof(int)); //10 max number of messages in mailbox
      run(); //start the kernel

 
 } 
