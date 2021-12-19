#include <iostream>
#include <algorithm>
#include <vector>

#if !defined(temp_h_priority_queue)
#define temp_h_priority_queue

class priority_queue
{
public:
    priority_queue() : count(0) {}                                                           // Class constructor
    priority_queue(const priority_queue& que);                                                // method for copying a class object
    ~priority_queue();                                                                       // Class destructor 
    priority_queue& operator =(const priority_queue& que);                                    // operator overloading
    void chgPriority(int element, int priority);                                            // change priority of element
    void minPriority();                                                                     // removes the top element of the queue.
    bool contains(int queue_element);                                                       // does the queue contain queue_element.
    void insert(int queue_element, int priority, bool flag);                                // insert queue_element into queue
    int top() { return queue[0].second; }                                                   // returns the top element of the queue.
    int size() { return count; };                                                           // return the number of queue_elements.
    int get_priority(int queue_element);                                                    // return priority of element
    void print();                                                                           // tests method
private:
    int count;                                                                              // amount of elements
    std::vector<std::pair<int, int>> queue;
};

#endif