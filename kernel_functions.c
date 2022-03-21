

/*=======================DECLARATION OF FUNCTIONS=========================*/

list * create_list(void);
listobj * create_listobj(int num);
void insert(list * mylist, listobj * pObj);
listobj * extract(listobj * pObj);


void insertBeforeMarker(listobj * pObj, listobj * pMarker);
msg * extractLast(mailbox * mBox);

void r_insert(list * mylist, listobj * pObj);
void t_insert(list * mylist, listobj * pObj);
void w_insert(list * mylist, listobj * pObj);
listobj * t_extract(list * mylist);
listobj * r_extract(list * mylist);
listobj * w_extract(list * mylist, listobj * pObj);
msg * pop(mailbox * mBox);
void push(mailbox * mBox, msg * newMsg);

int isEmptyMailbox(mailbox * mBox);
int isFull(mailbox * mBox);
int isEmptyList(list * mylist);



/*-------------------GREATE LIST--------------------------*/
list * create_list(){
    list * mylist = (list *)calloc(1, sizeof(list)); //alocate stance in memmory
    if (mylist == NULL) { // if no element in list
        return NULL;
    }
    
    mylist->pHead = (listobj *)calloc(1, sizeof(listobj));
    if (mylist->pHead == NULL) {
        free(mylist);
        return NULL;
    }
    
    mylist->pTail = (listobj *)calloc(1, sizeof(listobj));
    if (mylist->pTail == NULL) {
        free(mylist->pHead);
        free(mylist);
        return NULL;
    }
    mylist->pHead->pPrevious = mylist->pHead; //link head with the first obj
    mylist->pHead->pNext = mylist->pTail; //linked all objects with next address
    mylist->pTail->pPrevious = mylist->pHead; //linked treverse
    mylist->pTail->pNext = mylist->pTail; //the last object in list
    return mylist;
}

/*---------------------GREAT LIST OBJECT----------------------*/
listobj * create_listobj(int num){
    listobj * myobj = (listobj *)calloc(1, sizeof(listobj)); //allocate space in memmory
    if (myobj == NULL)//if we have no elem.
    {
        return NULL;
    }
    myobj->nTCnt = num; //data in my obj
    return (myobj);
}
/*---------------------INSERT-------------------------------*/
void insert(list * mylist, listobj * pObj){
    // insert first in list
    listobj *pMarker;
    pMarker = mylist->pHead;
    
    //Position found, insert element
    pObj->pNext = pMarker->pNext;
    pObj->pPrevious = pMarker;
    pMarker->pNext = pObj;
    pObj->pNext->pPrevious = pObj;
}
/*----------------------EXTRACT-------------------------------*/
listobj * extract(listobj *pObj){
    pObj->pPrevious->pNext = pObj->pNext;
    pObj->pNext->pPrevious = pObj->pPrevious;
    pObj->pNext = pObj->pPrevious = NULL;
    return (pObj);
}

/*-----------------------INSERTBEFORMARKER-----------------*/
void insertBeforeMarker(listobj * pObj, listobj * pMarker){
    pObj->pNext = pMarker;
    pObj->pPrevious = pMarker->pPrevious;
    pObj->pPrevious->pNext = pObj;
    pMarker->pPrevious = pObj;
}
/*-------------------- T_INSERT -----------------------*/
void t_insert(list * mylist, listobj * pObj){
    listobj *pMarker = mylist->pHead->pNext;
    while(pMarker != mylist->pTail){
        if(pMarker->nTCnt >= pObj->nTCnt) {
            break;
        }
        pMarker = pMarker->pNext;
    }
    insertBeforeMarker(pObj,pMarker);
}
/*--------------------Extract from head------------------------*/
listobj * t_extract(list * mylist){
    if(isEmptyList(mylist)){
        return FAIL;
    }
    listobj * pObj = mylist->pHead->pNext;
    pObj->pPrevious->pNext = pObj->pNext;
    pObj->pNext->pPrevious = pObj->pPrevious;
    pObj->pNext = pObj->pPrevious = NULL;
    return (pObj);
}
/*------------------Lowest deadline first----------------------*/
void w_insert(list * mylist, listobj * pObj){
    listobj *pMarker = mylist->pHead->pNext;
    while(pMarker != mylist->pTail){
        if(pMarker->pTask->Deadline >= pObj->pTask->Deadline) {
            break;
        }
        pMarker = pMarker->pNext;
    }
    insertBeforeMarker(pObj,pMarker);
}
/*--------------------Extract object---------------------------*/
listobj * w_extract(list * mylist, listobj * pObj){
    if(isEmptyList(mylist)==TRUE){
        return FAIL;
    }
    pObj->pNext->pPrevious = pObj->pPrevious;
    pObj->pPrevious->pNext = pObj->pNext;
    pObj->pNext = pObj->pPrevious = NULL;
    return pObj;
}
/*------------------Lowest deadline first----------------------*/
void r_insert(list * mylist, listobj * pObj){
    listobj * pMarker = mylist->pHead->pNext;
    while(pMarker != mylist->pTail){
        if(pMarker->pTask->Deadline >= pObj->pTask->Deadline){
            break;
        }
        pMarker = pMarker->pNext;
    }
    insertBeforeMarker(pObj,pMarker);
}
/*------------------Extract from head----------------------------*/
listobj * r_extract(list * mylist){
    listobj * pObj = mylist->pHead->pNext;
    pObj->pPrevious->pNext = pObj->pNext;
    pObj->pNext->pPrevious = pObj->pPrevious;
    pObj->pNext = pObj->pPrevious = NULL;
    return (pObj);
}


/*---------------- insert from tail-------------------*/
void push(mailbox * mBox, msg * newMsg){
   // msg * mBoxMsg = mBox->pHead->pNext;
    if(isFull(mBox)== TRUE){ //if we haven't plats in mbox
        msg * removedMessage = pop(mBox);
        mBox->nMessages--;  //remove one message
        mBox->nBlockedMsg -= removedMessage->Status;
        free(removedMessage);
    }
    newMsg->pNext = mBox->pTail;  //we add a nwe message at tail
    newMsg->pPrevious = mBox->pTail->pPrevious;
    newMsg->pPrevious->pNext = newMsg;
    mBox->pTail->pPrevious = newMsg;
    
    mBox->nMessages++;
    mBox->nBlockedMsg += newMsg->Status;
}
/*------------------ extract from head-------------------*/
msg * pop(mailbox * mBox){
    if(isEmptyMailbox(mBox)==TRUE){ //if mbox was empty
        return FAIL;
    }
    msg * mBoxMsg = mBox->pHead->pNext;
    
    mBox->pHead->pNext = mBoxMsg->pNext;
    mBox->pHead->pNext->pPrevious = mBox->pHead;
    mBoxMsg->pNext = mBoxMsg->pPrevious = NULL;
    mBox->nMessages--;
    mBox->nBlockedMsg -= mBoxMsg->Status;
    return mBoxMsg;
}

/*===================END OF ALIST=======================*/

/*Multi tasking operating system with messaging for embbeded programming */

/*================Global variables==================*/
uint Ticks;               //global sys tick counter
int KernelMode;          // 1=RUNNING, 0=INIT


// data structures
list * ReadyList;
list * WaitingList;
list * TimerList;
TCB  *PreviousTask;
TCB  *NextTask;




/*===================FUNCTIONS==========================*/

void idle(){
    while(1){
    }
}

listobj * GetRdyListObj(){
    return ReadyList->pHead->pNext;
}

listobj * GetTimerListObj(){
    return TimerList->pHead->pNext;
}

listobj * GetWaitListObj(){
    return WaitingList->pHead->pNext;
}

/*void updateRunningTask(){
   NextTask = ReadyList->pHead->pNext->pTask;
}*/

msg * allocMsg(){
    msg * newMsg = (msg*)calloc(1,sizeof(msg));
    if(newMsg == NULL){
        return NULL;
    }
    newMsg->pBlock = NULL;
    return newMsg;
}

mailbox * allocmBox(){
    mailbox * mBox = (mailbox*)calloc(1,sizeof(mailbox));
    if(mBox == NULL){
        return NULL;
    }
    mBox->pHead = allocMsg();
    mBox->pTail = allocMsg();
    if(mBox->pHead == NULL){
        free(mBox);
        return NULL;
    }
    if(mBox->pTail == NULL){
        free(mBox->pHead);
        free(mBox);
        return NULL;
    }
    return mBox;
}

int isEmptyMailbox(mailbox * mBox){
    if(mBox->pHead->pNext==mBox->pTail){
        return TRUE;
    }
    return FALSE;
}

int isFull(mailbox * mBox){
    if(mBox->nMessages>=mBox->nMaxMessages){
        return TRUE;
    }
    return FALSE;
}

int isEmptyList(list * mylist){
    if(mylist->pHead->pNext==mylist->pTail){
        return TRUE;
    }
    return FALSE;
}

/*========================TASK ADMINISTRATION =======================*/
/*------------------------INIT_KERNEL-------------------------------*/
/* This function initialzes the kernel and its data strukture */
exception init_kernel(){
    Ticks = 0;                        //set ticks counter to zero
    ReadyList = create_list();       //create necessary data structue
    TimerList = create_list();
    WaitingList = create_list();
  
    
    if(ReadyList == NULL){         //check if ready is null => not ok!
        return FAIL;
    }
    if(WaitingList == NULL){
        free(ReadyList);
        return FAIL;
    }
    if(TimerList == NULL){
        free(ReadyList);
        free(WaitingList);
        return FAIL;
    }
    
    create_task(idle,UINT_MAX);     //greate an idle task,0xFFFFFFFF
    KernelMode = INIT;             //set the kernel in start up mode
    return OK;                    //return status
}

/*---------------------GREATE TASK-------------------------*/
/*this function creates a task */
exception create_task(void(*body)(),uint d){
    listobj * newObj = create_listobj(0);
    TCB * newTCB = (TCB *)calloc(1,sizeof(TCB)); //Allocate memory for TCB
    if (newObj == NULL) {
        return FAIL;
    }
    if(newTCB == NULL){
        free(newObj);
        return FAIL;
    }
   
    newTCB->PC = body;             // set program counter to point to the taskbody
    newTCB->SPSR = 0x21000000;
    newTCB->Deadline = d;        //set deadline in TCB
    newTCB->StackSeg[STACK_SIZE-2] = 0x21000000 ;
    newTCB->StackSeg[STACK_SIZE-3] = ( unsigned int ) body ;
    newTCB->SP = &(newTCB->StackSeg[STACK_SIZE-9]);
    // newTCB->SP = &(newTCB->StackSeg[STACK_SIZE-1]);//set TCB'S SP to point to the stack segment
    newObj->pTask = newTCB;
    if(KernelMode==INIT){     //if start up mode
        r_insert(ReadyList, newObj);  //insert newtask in ready list
      // NextTask = ReadyList->pHead->pNext->pTask;
        return OK;                      //return status
    }
    else
    {
        isr_off();                      //disable interrupts
        PreviousTask = NextTask;
        r_insert(ReadyList, newObj);//insert new task in ready list
        NextTask = ReadyList->pHead->pNext->pTask;
        SwitchContext();
    }
    return OK;                      //return status
}
/*---------------------RUN---------------------*/
/*This function start the kernel and thus the system created tasks*/

void run(){
    Ticks = 0;
    KernelMode= RUNNING;          //set the kernel in running mode
    NextTask = ReadyList->pHead->pNext->pTask;
    LoadContext_In_Run();
}
/*------------------- TERMINATE ------------------*/
/* this call will terminate the running task, all the data structures will be removed*/
void terminate(){
    isr_off () ;
    listobj * leavingObj = r_extract(ReadyList); //remove running task from readylist
    NextTask = ReadyList->pHead->pNext->pTask;
    switch_to_stack_of_next_task();
    free(leavingObj->pTask);//set next task to be the running task
    free(leavingObj);       //update
    LoadContext_In_Terminate();//load
}

/*================= COMMUNICATION ======================*/
/*-----------------GREATE MAILBOX-----------------------*/
/*this call will creat maial box FIFO*/

mailbox* create_mailbox( uint nMessages, uint nDataSize){
    mailbox * mBox =  allocmBox(); //allocate memory for the mailbox
    if(mBox == NULL){             //check
        return NULL;
    }
    mBox->pHead->pNext = mBox->pTail;    //initial mailbox structure
    mBox->pHead->pPrevious = mBox->pHead;
    mBox->pTail->pNext = mBox->pTail;
    mBox->pTail->pPrevious = mBox->pHead;
    mBox->nMaxMessages = nMessages;
    mBox->nDataSize = nDataSize;
    return mBox;   //return mail box
}
/*--------------number of messages------------------*/

int no_messages(mailbox *mBox){
  return mBox-> nMessages + mBox-> nBlockedMsg;
}

/*----------------REMOVE MAILBOX---------------------*/
/*this call will remove the mailbox*/
exception remove_mailbox(mailbox *mBox){
    if(isEmptyMailbox(mBox)==TRUE){ //check if the mailbox is empty
        free(mBox);                   //free the memory for the mailbox
        return OK;
    }
    else{
        return NOT_EMPTY;
    }
}
/*----------------SEND WAIT_ SYNC-------------------------*/

exception send_wait(mailbox * mBox, void * pData){
    isr_off();               //disable interrupts
    listobj* rdyListObj = NULL;
    msg* mBoxMsg = mBox->pHead->pNext;
    if(mBoxMsg->Status==RECEIVER){   //if receiving tasks is waiting
        memcpy(mBoxMsg->pData,pData,mBox->nDataSize);//copy sender's data to data area
        pop(mBox); //remove receiving task's message
       PreviousTask = NextTask;
        r_insert(ReadyList,w_extract(WaitingList,mBoxMsg->pBlock));//struct from the mailbox and move the receiving task to readlist
       NextTask = ReadyList->pHead->pNext->pTask;
    }
    else{
        msg * newMsg = allocMsg(); //allocate message structure
        if(newMsg == NULL){
            return FAIL;
        }
        newMsg->pData = pData;   //set data pointer
        newMsg->pBlock = rdyListObj;
        newMsg->Status = SENDER;
        rdyListObj = GetRdyListObj();
        rdyListObj->pMessage = newMsg;
        push(mBox,newMsg);       //add message to the mailbox
       PreviousTask = NextTask;
        w_insert(WaitingList,r_extract(ReadyList)); //move sendeng task from readylist to waitinglist
        NextTask = ReadyList->pHead->pNext->pTask;
    }
    SwitchContext();
    if(Ticks>=PreviousTask->Deadline){  //if deadline is reached
        isr_off();                      //disable interrupts
        mBoxMsg = pop(mBox);           //remove send message
        free(mBoxMsg);
        isr_on();                     //enable interrupt
        return DEADLINE_REACHED;
    }
    
    return OK;
}
/*-----------------RECEIVE WAIT-SYNC-------------------------*/
exception receive_wait(mailbox* mBox, void* pData){
    isr_off();                //disable interrupt
    msg* mBoxMsg = mBox->pHead->pNext;
    listobj* rdyListObj = GetRdyListObj();
    if(mBoxMsg->Status==SENDER){  //if send message is waiting
        memcpy(pData,mBoxMsg->pData,mBox->nDataSize); //copy sende's data to receiving task's data area
        mBoxMsg = pop(mBox); //remove sending task's message from the mail box
        if(mBoxMsg->pBlock!=NULL){ //if message was wait type
            PreviousTask = NextTask;
            r_insert(ReadyList,w_extract(WaitingList,mBoxMsg->pBlock));//move sending task to readylist
            NextTask = ReadyList->pHead->pNext->pTask;
        }
        else{
            free(mBoxMsg->pData); //free snders data area
        }
    }
    else{
        msg * newMsg = allocMsg();//allocate message structe
        if(newMsg == NULL){
            return FAIL;           //Other return value?
        }
        newMsg->pData = pData;
        newMsg->pBlock = rdyListObj;
        newMsg->Status = RECEIVER;
        rdyListObj->pMessage = newMsg;
        push(mBox,newMsg); //add message to mailbox
        PreviousTask = NextTask;
        w_insert(WaitingList,r_extract(ReadyList)); //move receiving task to waitinglist
        NextTask = ReadyList->pHead->pNext->pTask;
        rdyListObj = GetRdyListObj();
    }
    SwitchContext();
    if(Ticks>=NextTask->Deadline){ //if deadline is reached
        isr_off();                     //disable interrupt
        mBoxMsg = pop(mBox);          //remove receive message
        free(mBoxMsg);
        rdyListObj->pMessage = NULL;
        isr_on();                    //enable interrupt
        return DEADLINE_REACHED;
    }
    
    return OK;
}
/*-----------------SEND NO WAIT_ASYNC----------------------*/
/*this call will send a message to the specified mailbox*/
exception send_no_wait(mailbox * mBox, void* pData){
    
    isr_off();              //disable interrupt
    
    msg * mBoxMsg = mBox->pHead->pNext;
    listobj * rdyListObj = GetRdyListObj();
    
    if(mBoxMsg->Status==RECEIVER){   //if receiving task is waiting
        memcpy(mBoxMsg->pData,pData,mBox->nDataSize); //copy data to receiving task's area
        mBoxMsg = pop(mBox);     //remove receiving task message struct from the mail box
        PreviousTask = NextTask;
        r_insert(ReadyList,w_extract(WaitingList,mBoxMsg->pBlock));//move receiving task to readylist
        NextTask = ReadyList->pHead->pNext->pTask;
        SwitchContext();
        
    }
    else{
        msg * newMsg = allocMsg();  //allocate a message struct
        if(newMsg == NULL){
            return FAIL; //Alternative return?
        }
        newMsg->pData = pData; //copy data to the message
        newMsg->Status = SENDER;
        rdyListObj->pMessage = newMsg;
        if(mBox->nMessages >= mBox->nMaxMessages){ //if mailbox is full
            pop(mBox);    //remove the oldest message
        }
        push(mBox,newMsg);    //add message to the mail box
       
    }
    
    return OK;
}
/*----------------------RECEIVE NO WAIT_ASYNC---------------------*/
/*this call will attempt to receive amessage from the specified mailbox */
exception receive_no_wait(mailbox * mBox, void * pData){
    
    isr_off();     //disable interrupt
    
    msg * mBoxMsg = mBox->pHead->pNext;
    
    if(mBoxMsg->Status==SENDER){   //if send message is waiting
        memcpy(pData,mBoxMsg->pData,mBox->nDataSize); //copy sender's data to receiving task's data area
        pop(mBox);  //remove
        if(mBoxMsg->pBlock != NULL){ //if message was the wait type
          PreviousTask = NextTask;
            r_insert(ReadyList,w_extract(WaitingList,mBoxMsg->pBlock));//move sending task to readylist
             NextTask = ReadyList->pHead->pNext->pTask;
        }
        else{
            mBoxMsg->pData = NULL; //free sender data area
        }
       
        }
        
    
   
     return OK;
}

/*=======================TIMING===============================*/
/*-----------------------WAIT---------------------------------*/
/*this call will block the calling task unit the given number of ticks*/

exception wait(uint nTicks){
    int status;
    isr_off();     //disable interrupt
   PreviousTask = NextTask;
    listobj * rdyListObj = GetRdyListObj();
    rdyListObj->nTCnt = Ticks+ nTicks;
    t_insert(TimerList,r_extract(ReadyList));//place running task in the timer list
    NextTask = ReadyList->pHead->pNext->pTask;
    SwitchContext();
    if(NextTask->Deadline >= Ticks){//if deadline is reached
        status = DEADLINE_REACHED; //return status
    }
    else{
      status = OK;
    }
    return status;
}
/*-------------------SET TICKS-----------------------------*/
/*this call will set the tick counter to the given value */
void set_ticks(uint  no_of_ticks){
    Ticks=  no_of_ticks;
}
/*------------------------TICKS----------------------------------*/
/*this call will return a 32 bit value of the tick counter */
uint ticks(void){
    return Ticks;
}
/*-----------------------DEADLINE---------------------------------*/
/*this call will return the deadline of the specified task*/
uint deadline(void){
    return NextTask->Deadline;
}
/*-----------------------SET DEADLINE------------------------------*/
/*this call will set the dead line for the calling task*/
void set_deadline(uint nNew){
    isr_off();            //disable interrupt
    NextTask->Deadline = nNew;   //set the deadline field in the callingTCB
    PreviousTask = NextTask;
    r_insert(ReadyList,r_extract(ReadyList));//reschedule readylist
    NextTask = ReadyList->pHead->pNext->pTask;
    SwitchContext();
}

/*---------------------TIMER INT------------------------------*/
/*this function is not able for the user to call it is call by ISR*/
void TimerInt(){
    Ticks++;  //increment tick counter
    listobj * timerListObj = GetTimerListObj();
    listobj * waitListObj = GetWaitListObj();
    //check the timerlist for tasks that are ready for execution, move these to ready list
    while(timerListObj->nTCnt <= ticks()&&timerListObj!=TimerList->pTail){
        r_insert(ReadyList,t_extract(TimerList));
       NextTask = ReadyList->pHead->pNext->pTask;
        timerListObj = GetTimerListObj();
    }
    //chech the waitinglist for tasks that have expired deadlines move these to readylist and clean up their mailbox enty
    while(waitListObj->pTask->Deadline <= ticks()&&waitListObj!=WaitingList->pTail){
        r_insert(ReadyList,w_extract(WaitingList,waitListObj));
        NextTask = ReadyList->pHead->pNext->pTask;
        waitListObj = GetWaitListObj();
    }
}
/*-------------------FINISHED-----------------------------------*/
