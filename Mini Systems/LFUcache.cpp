/*
LFU (Least Frequently Used) is a famous cache eviction algorithm.

For a cache with capacity k, if the cache is full and need to evict a key in it, the key with the lease frequently used will be kicked out.

Implement set and get method for LFU cache.

Example
Input:
LFUCache(3)
set(2,2)
set(1,1)
get(2)
get(1)
get(2)
set(3,3)
set(4,4)
get(3)
get(2)
get(1)
get(4)

Output:
2
1
2
-1
2
1
4

*/

class LFUCache {
private:
    // ListNode a double linked list node to save key and value
    struct ListNode {
        int key;
        int value;
        ListNode* prev;
        ListNode* next;
        ListNode(int k, int v) : key(k), value(v), prev(NULL), next(NULL) {}; 
    };
    
    //Bucket a double linked list to save nodes with the same frequency
    struct BucketNode {
        int freq;
        ListNode* head;
        ListNode* tail;
        BucketNode* prev;
        BucketNode* next;
        
        BucketNode(int f) : freq(f), prev(NULL), next(NULL), head(NULL), tail(NULL) {};
        
        void removeOne(ListNode* node) {
            if (node == head && node == tail) {
                tail = head = NULL;
            } else if (node == head) {
                node->next->prev = NULL;
                head = node->next;
            } else if (node == tail) {
                node->prev->next = NULL;
                tail = node->prev;
            } else {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            }
            node->prev = node->next = NULL;
        }
        
        void addOne(ListNode* node) {
            if (head == NULL) {
                head = tail = node;
            } else {
                head->prev = node;
                node->next = head;
                head = node;
            }
        }
        
        bool empty() { return head == NULL; }
    };
    
public:
    //Initialization
    LFUCache(int capacity) : mCapacity(capacity) {
        head = tail = NULL;
    }
    
    // @param key an integer
    // return the value of the key
    int get(int key) {
        if (mCapacity == 0) return -1;
        ListNode* node = promote(key);
        return node ? node->value : -1;
    }
      
    // @param key an integer
    // @param value an integer
    // set the value of the key
    void set(int key, int value) {
        if (mCapacity == 0) return;
        //Increase the frequency of this node if it exists
        ListNode* node = promote(key);
        if (node) {
            node->value = value;
            return;
        } else {
            //If it is a new key, check the capacity
            //Evict the one with least frequency and with earliest time stamp
            if (freqTable.size() >= mCapacity) evictLast();
            node = new ListNode(key, value);        //Save the key in a new node
            BucketNode* bucket;
            if (tail == NULL || tail->freq != 1) {
                bucket = new BucketNode(1);
                bucket->addOne(node);
                if (tail == NULL) {
                    head = tail = bucket;
                } else {
                    bucket->prev = tail;
                    tail->next = bucket;
                    tail = bucket;
                }
            } else {
                bucket = tail;
                bucket->addOne(node);
            }
            freqTable[key] = pair<BucketNode*, ListNode*>(bucket, node);
        }
    }
    
private:
    // @param key an integer
    // increase the frequency of the integer
    inline ListNode* promote(int key) {
        auto it = freqTable.find(key);
        if (it == freqTable.end()) return NULL;
        BucketNode* bucket = it->second.first;
        ListNode* node = it->second.second;
        bucket->removeOne(node);          //remove the key node from the previous bucket
        BucketNode* newBucket;          
        if (bucket->prev == NULL || bucket->prev->freq != bucket->freq + 1) {
            //put the key node into a bucket with higher frequency (+1)
            //If there is no bucket with target frequency, create a new bucket
            newBucket = new BucketNode(bucket->freq + 1);
            newBucket->addOne(node);      
            if (bucket->prev == NULL) {
                head = newBucket;
                newBucket->next = bucket;
                bucket->prev = newBucket;
            } else {
                bucket->prev->next = newBucket;
                newBucket->prev = bucket->prev;
                newBucket->next = bucket;
                bucket->prev = newBucket;
            }
        } else {
            newBucket = bucket->prev;
            newBucket->addOne(node);
        }
        
        it->second.first = newBucket;
        it->second.second = node;
        
        if (bucket->empty()) removeBucket(bucket);
        
        return node;
    }
    
    // @param bucket a pointer to the bucket node
    // Remove a bucket  
    inline void removeBucket(BucketNode* bucket) {
        if (bucket == head && bucket == tail) {
            head = tail = NULL;
        } else if (bucket == head) {
            bucket->next->prev = NULL;
            head = bucket->next;
        } else if (bucket == tail) {
            bucket->prev->next = NULL;
            tail = bucket->prev;
        } else {
            bucket->prev->next = bucket->next;
            bucket->next->prev = bucket->prev;
        }
        delete bucket;
    }
       
    // Remove the node with least frequency  
    inline void evictLast() {
        ListNode* node = tail->tail;
        freqTable.erase(node->key);
        tail->removeOne(node);
        delete node;
        if (tail->empty()) removeBucket(tail);
    }
    
private:
    int mCapacity;
    BucketNode* head;
    BucketNode* tail;
    unordered_map<int, pair<BucketNode*, ListNode*>> freqTable;
};