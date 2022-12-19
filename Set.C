#include <iostream>
#include <string>
#include <assert.h>
#include "Line.h"
#include "Set.h"

//TO DO: lots of work to do in this file. 
//       Look for TODO throughout the file

/*
 * initialize
 * Initialize the Set object.  
 * A Set is a linked list of Line objects.  Up to associativity Line objects can
 * be in the linked list.
 */
void Set::initialize(int32_t index, int32_t associativity, bool verbose)
{
   this->index = index;                   //index of this set
   this->associativity = associativity;   //way
   this->verbose = verbose;               //can be used to turn debugging prints on or off
   this->firstLine = NULL;                //pointer to the first Line object in the Set
}

/*
 * isHit
 * Looks in the Set for the tag. Returns true if found and false otherwise.
 */
bool Set::isHit(int64_t tag)
{

    Line * temp = firstLine;             //Here we have to assign a temp var to traverse the linked list
    //This is where we actually traverse the list
    while (temp != NULL) {             
        //Need to check and see if we have found the correct line/block
        if (temp->tag == tag) {
            return true;         //If we have found the right block, return true
        }
        //If we didn't find the correct block go to the next and keep looking
        temp = temp->next;
    }
    //If we traverse the entire set and don't find the correct block, return false
    return false;
}

/*
 * isFull
 * Returns true if the Set is full. The Set is full if the linked listed contains
 * associativity (way) Line objects. This is used to determine whether an eviction 
 * is necessary.
 */
bool Set::isFull()
{   
   //Hint: Loop through the linked list, counting the number of objects
    int count = 0;
    for (Line * itr = firstLine; itr != NULL; itr = itr->next) {
        count++;
    }
    return count == associativity;
}

/*
 * updateSet
 * Updates the Set with the provided tag.
 * The Line objects in the linked list encapsulated by the Set object need to be kept in
 * in the Set in LRU order so that the MRU Line object is in the front
 * of the linked list.
 * 
 * Suppose tag is T and associativity is 4.
 * If the tags of the nodes in the linked list are: A B C D
 * then after the update, the tags are: T A B C
 * Notice that D (the LRU) got evicted.
 *
 * If the tags of the nodes in the linked list are: A B T D
 * then after the update, the tags are: T A B D
 * Notice that T was moved from where it was to the front of the linked list.
 *
 * If the tags of the nodes in the linked list are: A B 
 * (only two nodes)
 * then after the update, the tags are: T A B
 * Notice nothing was evicted because the Set isn't full.
 * 
 */
void Set::updateSet(int64_t tag)
{
   //Note: this work is completed by calling other methods in this class.
  
   //First we need to check if the tag is already present in the set
   if (isHit(tag)) {
       removeLine(tag);
       insertInFront(tag); 
   }
   //If the tag isn't found, then we need to check and see if the set is full or not
   else if (isFull()) {
       //***If we fall in here, we know the set is full and we need to evict the LRU and update the set***//
       removeLine(getLRUTag());     //First thing is to remove the LRU(least recently used)
       insertInFront(tag);          //Add the new line to the front of the set
   }
   //If we fall in here, the tag wasn't found and the set is not full
   else{ 
       insertInFront(tag);      //Just insert the new tag in the front
   }
   
}

/*
 * insertInFront
 * Create a new Line object and insert it in the front of the linked list.
 * This method is called by updateSet.
 */
void Set::insertInFront(int64_t tag)
{
    Line * newLine = new Line(tag);      //Need to create a new block(line) to insert
    newLine->next = firstLine;           //Set the new block(line)'s next variable to the head of the list
    firstLine = newLine;                 //Update the head of the list to the new block(line)
}

/*
 * removeLine
 * Remove the Line object from the linked list with the specified tag.
 * This will update firstLine if the Line object is the first one in the
 * linked list; otherwise, it updates the next field of the previous
 * Line in the linked list.
 * This method is called by updateSet.
 */
void Set::removeLine(int64_t tag)
{
    Line * current = firstLine;
    Line * previous = firstLine;

    if (current != NULL) {
        if (current->tag == tag) {
            Line * l = current->next;
            delete firstLine;
            firstLine = l;
        } else {
            current = current->next;
            while (current != NULL) {
                if (current->tag == tag) {
                    previous->next = current->next;
                    delete current;
                    return;   
                } else {
                    current = current->next;
                    previous = previous->next;
                }
            }
        }
    }
}

/*
 * getLRUTag
 * Returns the tag of the LRU Line in the set. This is the tag of the
 * last Line object in the linked list.
 * This method is called by updateSet.
 */
int64_t Set::getLRUTag()
{
    Line * start = firstLine;
    Line * prev = start;
    if (start != NULL) {
        start = start->next;
        while(start != NULL) {
            prev = start;
            start = start->next;
        }
        return prev->tag;
    }
}

/*
 * Destructor for the dynamically allocated data.
 */
Set::~Set()
{ 
    //***********************************************************//
    //***** Here we need to loop through the entrie set and *****//
    //*****             delete all the lines                *****//
    //***********************************************************//

    Line * curr = firstLine;            //Initialize a temp variable to loop through the set
    Line * currToDelete = firstLine;    //Initalize a temp variable to hold the line to be deleted

    //Here we loop through the linked list in the set
    while(curr != NULL){
        
        curr = curr->next;          //Go ahead and update curr to the next line
        delete currToDelete;       //delete the line 
        currToDelete = curr;        //Update the tmp variable so we can delete the next one    
    }
}


/*
 * print
 * Prints the tags in the Set if the set is non-empty. If the Set is empty
 * nothing is printed. This can be used for debugging.
 */
void Set::print()
{
   
   if (firstLine)
   {
      std::cout << "Set: " << index << " Tags: ";
      Line * ptr = firstLine;   //initialize to point to first node in the linked list
      while (ptr != NULL)
      {
         std::cout << std::hex << ptr->tag << " ";
         ptr = ptr->next;
      }
      std::cout <<"\n";
   }
}

