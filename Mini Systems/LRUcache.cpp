/*
Design and implement a data structure for Least Recently Used (LRU) cache. 
It should support the following operations: get and set.

get(key) Get the value (will always be positive) of the key if the key exists in the cache,
otherwise return -1.
set(key, value) Set or insert the value if the key is not already present. 
When the cache reached its capacity, it should invalidate the least recently used item 
before inserting a new item.
Finally, you need to return the data from each get.

Example1:
Input:
LRUCache(2)
set(2, 1)
set(1, 1)
get(2)
set(4, 1)
get(1)
get(2)
Output: [1,-1,1]
Explanation：
cache cap is 2，set(2,1)，set(1, 1)，get(2) and return 1，set(4,1) and delete (1,1)，
because （1,1）is the least use

Example2:
Input：
LRUCache(1)
set(2, 1)
get(2)
set(3, 2)
get(2)
get(3)
Output：[1,-1,2]
Explanation：
cache cap is 1，set(2,1)，get(2) and return 1，set(3,2) and 
delete (2,1)，get(2) and return -1，get(3) and return 2.
*/

#include <list>
class LRUCache {
public:
    /*
    * @param capacity: An integer
    */LRUCache(int capacity) {
        // do intialization if necessary
        _capacity = capacity;
    }

    /*
     * @param key: An integer
     * @return: An integer
     */
    int get(int key) {
        // find the key node and promp it to the front of the list
        if (m.find(key) != m.end()) {
            auto it =  m[key];
            int val = it ->val;
            if (it != lData.begin()){
              lData.erase(it); 
              lData.emplace_front(key,val);
              m[key] = lData.begin();  
            }
            
            return val;
       }       
       else return -1;
    }

    /*
     * @param key: An integer
     * @param value: An integer
     * @return: nothing
     */
    void set(int key, int value) {
        // find the key node and delete it the list
        if (_capacity <= 0) return;
        if (m.find(key) != m.end()) {
            auto it =  m[key];
            lData.erase(it);            
        }
        else{
           // If it is a new node, check the capacity of the cache
           if (lData.size() >= _capacity){
               int k = lData.back().key;
               lData.pop_back();
               m.erase(k);
           } 
        }
        //Insert the node into the front of the list
        lData.emplace_front(key,value);
        m[key] = lData.begin();
    }
private:
     //Define a structure to pair key and value, save as a double linked list node
     struct data{
        int key;
        int val;
        data(int k, int v):key(k),val(v){}
    };
    
    //the double linked list iterator will not be changed when other nodes updates
    //so it is safe to be utilized as a pointer
    unordered_map<int,std::list<data>::iterator> m;
    std::list<data> lData;
    int _capacity = 0;
 
};