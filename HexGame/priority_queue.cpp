#include <iostream>
#include <algorithm>
#include <vector>

#include "priority_queue.h"

priority_queue::priority_queue(const priority_queue& que)
{
    count = (int)queue.size();
    std::pair<int, int> element;

    for (int i = 0; i < count; ++i) {
        queue[i] = que.queue[i];
    }
}

priority_queue::~priority_queue()
{
    if (count > 0) {
        queue.clear();// dosen't metter
        count = 0;
    }
}

priority_queue& priority_queue::operator= (const priority_queue& que)
{
    count = (int)queue.size();
    std::pair <int, int> element;

    for (int i = 0; i < count; ++i) {
        queue[i] = que.queue[i];
    }

    return *this;
}

void priority_queue::chgPriority(int element, int priority)
{
    bool flag = false;

    for (int i = 0; (i < count) && !flag; ++i) {
        if (queue[i].second == element) {
            queue[i].first = priority;
            flag = true;
        }
    }
    if (!flag)
        std::cout << "Nothing change --------------" << std::endl;
    else
        std::sort(queue.begin(), queue.end());
}

void priority_queue::minPriority()
{
    queue.erase(queue.begin());
    --count;
}

bool priority_queue::contains(int queue_element)
{
    bool flag = false;
    for (int i = 0; (i < queue.size()) && !flag; ++i) {
        if (queue[i].second == queue_element) {
            flag = true;
        }
    }
    return flag;
}

void priority_queue::insert(int queue_element, int priority, bool flag)
{
    std::pair <int, int> element;

    element.first = priority;
    element.second = queue_element;

    queue.push_back(element);
    ++count;
    if (flag)
        std::sort(queue.begin(), queue.end());
}
int priority_queue::get_priority(int queue_element)
{
    int output = count;
    for (int i = 0; i < count; ++i) {
        if (queue[i].second == queue_element) {
            output = queue[i].first;
            break;
        }
    }
    return output;
}
void priority_queue::print()
{
    for (int i = 0; i < count; ++i) {
        std::cout << "Elems: " << queue[i].second;
        std::cout << " Priority" << queue[i].first << std::endl;
    }
};