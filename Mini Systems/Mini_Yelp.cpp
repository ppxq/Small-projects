/*
Design a simple yelp system. Support the following features:

Add a restaurant with name and location.
Remove a restaurant with id.
Search the nearby restaurants by given location.
A location is represented by latitude and longitude, both in double. 
A Location class is given in code.

Nearby is defined by distance smaller than k Km .

Restaurant class is already provided and you can directly call Restaurant.create() to 
create a new object. Also, a Helper class is provided to calculate the distance between 
two location, use Helper.get_distance(location1, location2).

A GeoHash class is provided to convert a location to a string. 
Try GeoHash.encode(location) to convert location to a geohash string and 
GeoHash.decode(string) to convert a string to location.

*/


/**
 * Definition of Location:
 * class Location {
 * public:
 *     double latitude, longitude;
 *     static Location create(double lati, double longi) {
 *         // This will create a new location object
 *     }
 * };
 * Definition of Restaurant
 * class Restaurant {
 * public:
 *     int id;
 *     string name;
 *     Location location;
 *     static Restaurant create(string &name, Location &location) {
 *         // This will create a new restaurant object,
 *         // and auto fill id
 *     }
 * };
 * Definition of Helper
 * class Helper {
 * public:
 *     static get_distance(Location &location1, Location &location2) {
 *         // return distance between location1 and location2.
 *     }
 * };
 * class GeoHash {
 * public:
 *     static string encode(Location &location) {
 *         // return convert location to a GeoHash string
 *     }
 *     static Location decode(string &hashcode) {
 *          // return convert a GeoHash string to location
 *     }
 * };
 */
class MiniYelp {
public:
    MiniYelp() {
        // initialize your data structure here.
        nRes = 0;
        ranges = {2500,630,78,20,2.4,0.61,0.076,0.019};
    }

    // @param name a string
    // @param location a Location
    // @return an integer, restaurant's id
    int addRestaurant(string name, Location &location) {
        // Write your code here
        Restaurant newRes =  Restaurant::create(name,location);
        restaurants[++nRes] = newRes;
        //Add into the geohash map
        string hashcode = GeoHash::encode(location);
        for (int i = 0; i < 8; ++i){
            string temps = hashcode.substr(0,i);
            if (m_geoHash.find(temps) == m_geoHash.end())
               m_geoHash[temps] = unordered_set<int>();
            m_geoHash[temps].insert(nRes);
        }
        return nRes;
    }

    // @param restaurant_id an integer
    void removeRestaurant(int restaurant_id) {
        // Write your code here
        if (restaurants.find(restaurant_id) == restaurants.end())
            return;
        restaurants.erase(restaurant_id);
        string hashcode = GeoHash::encode(restaurants[restaurant_id].location);
        for (int i = 0; i < 8; ++i){
            string temps = hashcode.substr(0,i);
            if (m_geoHash.find(temps) != m_geoHash.end())
                m_geoHash[temps].erase(restaurant_id);
        }
    }

    // @param location a Location
    // @param k an integer, distance smaller than k miles
    // @return a list of restaurant's name and sort by 
    // distance from near to far.
    vector<string> neighbors(Location &location, double k) {
        // Write your code here
        vector<string> res;
        int nchar = 1;
        //Find how many number of chars we should check
        for (int i = 0; i < 8; ++i){
            if (k >= ranges[i]){
                nchar = i;
                break;
            }
        }
        //Check all the restaurants in the ranges, and compute the specific distance
        string hashcode = GeoHash::encode(location);
        hashcode = hashcode.substr(0,nchar);
        
        multimap<double,string> reslist;
        for (auto it = m_geoHash[hashcode].begin(); it != m_geoHash[hashcode].end(); ++it){
            Location loc = restaurants[*it].location;
            double dis = Helper::get_distance(location,loc);
            if (dis <= k)
              reslist.insert({dis,restaurants[*it].name});
        }
        for (auto it = reslist.begin(); it != reslist.end(); ++it){
            res.push_back(it->second);
        }
        return res;
    }
private:
    vector<double> ranges;
    unordered_map<string,unordered_set<int>> m_geoHash;
    unordered_map<int,Restaurant> restaurants;
    int nRes;
};