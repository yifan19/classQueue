//////////////////////////////////////////////////////////////
//
// Your #includes here; make sure you are allowed them ...
//

#include <limits.h>
#include <time.h>
#include <stdlib.h>
using namespace std;
//////////////////////////////////////////////////////////////
//
// #includes for local testing only
//


#ifndef MARMOSET_TESTING
#include <iostream>
#endif

//////////////////////////////////////////////////////////////
//
// Enums and structs for use in assignment

#ifndef MARMOSET_TESTING
enum COURSE
{
	CHE102,
	MATH115,
	MATH117,
	ECE105,
	ECE150,
	ECE190,
	Null
};

struct Assignment
{
	COURSE course;
	int    assnNum;      // Assignment number
	int    dueMonth;     // 1-12
	int    dueDay;       // 1-31
	char*  description;  // Assignment description
};

#endif

enum MYTIME {MONTH, DAY, YEAR};
const MYTIME monthConst = MONTH;
const MYTIME dayConst = DAY;

//////////////////////////////////////////////////////////////
//
// Class declaration; modify but do not remove
							 
class HomeworkQueue
{
public:
    bool                enqueue(const Assignment& assignment);
    const Assignment*   dequeue();
    int                 daysTillDue(const COURSE course);
    const Assignment*   dueIn(const int numDays);
    void                printNodes();

    HomeworkQueue();
    //constructor
    ~HomeworkQueue();   //destructor
private:
    bool                errorCheck(const Assignment& assignment);
    int                 getTime(MYTIME currTime);
    bool                isEarlier(const Assignment& a1, const Assignment& a2);
    
    
    
    struct HomeworkStruc{
        
        Assignment* assn;
        HomeworkStruc* nextInQueue;
    };
    
    HomeworkStruc* head;
    
    // It is up to you what you store here
};


//////////////////////////////////////////////////////////////
//
// Your code here ...
//
int HomeworkQueue:: getTime(MYTIME currTime){
	time_t currentTime; // create time_t type
    tm* timeinfo; // create pointer to tm struct
	currentTime = time(NULL); // get sec since 1990
    timeinfo =  localtime(&currentTime); //get local time
	
	switch (currTime){
		case MONTH:
			return (timeinfo -> tm_mon) +1;
		case DAY:
			return (timeinfo -> tm_mday);
		case YEAR:
			return (timeinfo -> tm_year)+1900;
	}
	return -99;
}

bool HomeworkQueue::isEarlier(const Assignment& a1, const Assignment& a2){
	if (a1.dueMonth == a2.dueMonth){ // month is equal
		if (a1.dueDay < a2.dueDay){
			//cout << a1.dueDay << "  dueDay smaller vs " << a2.dueDay << endl;
			return true; // due day smaller
		} else{
			//cout << a1.dueDay << "  dueDay bigger " << a2.dueDay << endl;
			return false; // due day bigger
		}
	}
	else if (a1.dueMonth < a2.dueMonth){ // month is smaller
		return true;
		
	} else{ // month is bigger
		//cout << "month is bigger"<< endl;
		return false;
	}
}

bool HomeworkQueue::errorCheck(const Assignment& assignment)
{
	 
	HomeworkStruc* cursor = head;

	if ( (assignment.dueMonth < 1) || (assignment.dueMonth > 12) ) {
		return false;
	}
	if ( (assignment.dueDay < 1) || (assignment.dueDay > 30) ) {
		return false;
	}
	if ( (assignment.description[0] == 0) ){
		return false;
	}
	
	if ( (assignment.course == Null) ){
		return false;
	}
	while (cursor){ // while cursor is null or found course is false
		// doesnt equal null pointer
		
		if ( !(cursor ->assn) ){ // if assignment is null
			return true; // the cursor has no data inside... not actuall an error here;
		}
		
		if ((cursor-> assn -> course == assignment.course) && (cursor -> assn -> assnNum == assignment.assnNum)){ //BUG BUG BUG BUG BUG
			return false; // duplication
		}
		cursor = cursor -> nextInQueue;
		
	}
	return true;

}

bool HomeworkQueue::enqueue(const Assignment& assignment)

{
		// add a thing to the queue
	if (!errorCheck (assignment)){
		return false;
	}
	
	HomeworkStruc* newHomework = new HomeworkStruc;// new homework element
	newHomework -> assn = new Assignment;
	*(newHomework -> assn) = assignment;
	//insert the assignment into a small linklist
	
	//now i have to find out where to put the thing;
	HomeworkStruc* cursor = head;
	// cursor points at head
	
	// if queue is null; just add the first element to it;
	
	if ( (cursor == NULL) || (cursor->assn == NULL) ){
		head = newHomework; //newHomework is the first element of our queue
		head -> nextInQueue = NULL; // set it to NULL
		return true;
	}
	
	
	//lets see if my element is earlier than the first linklist;
	if( isEarlier( *(newHomework->assn), *(cursor->assn) )  ){
		HomeworkStruc* temp = head; //hold the old chain
		head = newHomework;// point the head to the newone
		newHomework->nextInQueue = temp; // attach the first one to the temp;
		return true;
	}
	// not the earlier than the first on the list;
	//cout << "not the first, maybe the worst" << endl;
	
	
	
	while ( (cursor -> nextInQueue) && (cursor->nextInQueue -> assn ) ){ //does cursor have neigbour
		
		if ( isEarlier( *(newHomework->assn), *(cursor->nextInQueue -> assn ))  ){
			// is my newhomework earlier than your neigbour to the right
			HomeworkStruc* temp = cursor->nextInQueue; // hold on to the neigbour
			cursor->nextInQueue = newHomework;
			// person we are looking at links to the newNode Homework;
			newHomework->nextInQueue = temp; // attach the rest to the new chain
			return true;
		} else{
			cursor = cursor ->nextInQueue; // RUN go to your neigbour's place
			
		}
	}
	// IF NOTHING WORKS, it should be appended at the end;
		
	cursor -> nextInQueue = newHomework;
	newHomework -> nextInQueue = NULL;
	return true;
}

const Assignment* HomeworkQueue::dequeue()
{
	if (!head){
		//cout << "trying to access a NULL pointer, exiting" << endl;
		return NULL;
	} else {
		HomeworkStruc* removeq = head; //let remove point to the first element of the queue
		head = head -> nextInQueue; // the real queue now points to the second element of the queue;
		
		// set the remove  pointer to null;
		removeq -> nextInQueue  = NULL;
		return removeq->assn;
	}
    
}

int HomeworkQueue::daysTillDue(const COURSE course)
{
if (course == Null){
		return INT_MAX;
	}
	
	if (!head || !(head->assn) ){
		return INT_MAX;
	}
	HomeworkStruc* cursor;
	cursor = (HomeworkStruc*) head;
	
	bool foundCourse = false;
	
	int myDueMonth = 0;
	int myDueDay = 0;
	int currDay = getTime(dayConst);
	int currMonth = getTime(monthConst);
	
	while (cursor && !foundCourse ){ // while cursor is null or found course is false
		// doesnt equal null pointer
		
		if ( !(cursor ->assn) ){ // if assignment is null
			return INT_MAX;
		}
		
		if ( ( cursor->assn->course) == course ){
			myDueMonth = cursor -> assn -> dueMonth;
			myDueDay = cursor -> assn -> dueDay;
			//cout << "interesting numbers found about your exam.. "<< myDueDay << "..." << myDueMonth << endl;
			foundCourse = true;
		}
		cursor = cursor -> nextInQueue;
		
		
	}

	if (!foundCourse){
		// if that course was not found in the chain...
		return INT_MAX; //exit
	}
	
	myDueDay += 30*myDueMonth;
	currDay += 30*currMonth;
	
	return (myDueDay -currDay);
}

const Assignment* HomeworkQueue::dueIn(const int numDays)
{
	int dueMonthNow = getTime(MONTH);
	//time right now;
	int dueDayNow = getTime(DAY);
	//cout << "today's day is: " << dueDayNow << endl;
	// month right now;
	
	
	Assignment* errorStruc = new Assignment[1];
	Assignment* dueStruc = new Assignment;
	//basically putting the actual time into a assignment structure
	
	dueDayNow += numDays + 1; // add the due in date in
	// off by 1 error  28 is not earlier than 28
	
	while (dueDayNow > 30){
		dueMonthNow++;
		dueDayNow -= 30;
	}
	
	dueStruc->dueMonth = dueMonthNow;
	dueStruc->dueDay = dueDayNow;
	//copying the data in a struct (assignment)
	dueStruc->course = Null;
	// dueStruc becomes now the Null termination structure;
	
	if (!head || !(head->assn) ){
		
		errorStruc[0] = *dueStruc;
		return errorStruc;
	} else{
		delete errorStruc;
	}
	
	
	
	
	
	//cout << "looking for assignment earlier than " << dueDayNow;
	//cout << " " << dueMonthNow << endl;
	//////////////////////////////////////// what if it goes to next year????
	
	
	
	int arraySize= 0;
    
	// how big is our array????
	//test
	HomeworkStruc* cursor = (HomeworkStruc*)head;
	HomeworkStruc* cursor2 = cursor; // copy of the cursor
	while (cursor){ // while our q is not NULL
	    if( isEarlier(*(cursor->assn), *dueStruc) ){
	        // 24 is earlier than 27 (dueDate);"
	        arraySize++;
	       
	    }
			
		cursor = cursor->nextInQueue;
	}
	
	//cout << "arraySize = " << arraySize << endl;
	
	Assignment* resList = new Assignment[arraySize+1]; // 1 for the null termination
	int arrayIndex = 0;
	
	if (arraySize){ // if there is something to lookout for if not...
		while (cursor2){ // while our q is not NULL
			if( isEarlier(*(cursor2->assn), *dueStruc ) ){
				// 24 is earlier than 27
				resList [arrayIndex] = *(cursor2->assn);
				
				//cout << "arrayIndex " << arrayIndex << endl;
				arrayIndex++;
			}
			
			cursor2 = cursor2->nextInQueue;
		}
	}
	
	
	
	resList[arrayIndex] = *dueStruc;
	// null termination
	
	return resList;
	
}

HomeworkQueue::HomeworkQueue()
{
    
    head = NULL;

}

HomeworkQueue::~HomeworkQueue()
{
	head = NULL;
}

void HomeworkQueue::printNodes(){ // does modify header
	if (head == NULL){ // if header points to nothing
		//cout << "what are you trying to do m8" << endl;
		return;
	}
	
	while ((head->nextInQueue)){
		//cout << head->assn -> assnNum << "--->";
		head = head ->nextInQueue;
	}
	//cout << head->assn -> assnNum << endl;
	return;
}

		

#ifndef MARMOSET_TESTING
int main(const int argc, const char* const args[])
{
	//formatting
	std::cout << std::endl;

	HomeworkQueue hwq;

	char boring[] = "Something something solubility something something gases something";

	Assignment a3 =
	{
		.course = CHE102,
		.assnNum = 4,
		.dueMonth = 12,
		.dueDay = 4,
		.description = boring
	};
	Assignment a4 =
	{
		.course = CHE102,
		.assnNum = 3,
		.dueMonth = 12,
		.dueDay = 4,
		.description = boring
	};
	Assignment a2 =
	{
		.course = CHE102,
		.assnNum = 2,
		.dueMonth = 12,
		.dueDay = 4,
		.description = boring
	};
	Assignment a1 =
	{
		.course = CHE102,
		.assnNum = 1,
		.dueMonth = 12,
		.dueDay = 4,
		.description = boring
	};

    HomeworkQueue Marmoset;
    
    Marmoset.enqueue(a1);
    Marmoset.enqueue(a2);
    Marmoset.enqueue(a3);
    Marmoset.enqueue(a4);
    Marmoset.printNodes();
	
    return 0;
}
#endif
