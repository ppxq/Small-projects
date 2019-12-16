/*
Support two basic uber features:

Drivers report their locations.
Rider request a uber, return a matched driver.
When rider request a uber, match a closest available driver with him, then mark the driver not available.

When next time this matched driver report his location, return with the rider's information.

report(driver_id, lat, lng)
return null if no matched rider.
return matched trip information.
request(rider_id, lat, lng)
create a trip with rider's information.
find a closest driver, mark this driver not available.
fill driver_id into this trip.
return trip


Example 1:
Input:
  report(1, 36.1344, 77.5672) 
  report(2, 45.1344, 76.5672) 
  request(2, 39.1344, 76.5672) 
  report(1, 38.1344, 75.5672) 
  report(2, 45.1344, 76.5672) 
Output:
  null
  null
  Trip(rider_id: 2, driver_id: 1, lat: 39.1344, lng: 76.5672)
  Trip(rider_id: 2, driver_id: 1, lat: 39.1344, lng: 76.5672)
  null
  
  
Example 2:
Input:
  report(1, 36.1344, 77.5672)
  report(2, 45.1344, 76.5672)
  request(2, 39.1344, 76.5672)
  request(3, 78.1344, 134.5672)
Output:
  null
  null
  LOG(INFO): Trip(rider_id: 2, driver_id: 1, lat: 39.1344, lng: 76.5672)
  LOG(INFO): Trip(rider_id: 3, driver_id: 2, lat: 78.1344, lng: 134.5672)
*/




/**
 * Definition of Trip:
 * class Trip {
 * public:
 *     int id; // trip's id, primary key
 *     int driver_id, rider_id; // foreign key
 *     double lat, lng; // pick up location
 *     Trip(int rider, double lat, double lng);
 * }
 * Definition of Helper
 * class Helper {
 * public:
 *     static double get_distance(double lat1, double lng1,
 *                         double lat2, double lng2) {
 *         // return distance between (lat1, lng1) and (lat2, lng2)
 *     }
 * };
 */
class Location {
public:
    double lat, lng;
    Location(double _lat, double _lng): lat(_lat), lng(_lng) {};
};

class MiniUber {
private:
    unordered_map<int, Trip*> driver2Trip; //hashmap of (driverid, current trip)
    unordered_map<int, Location*> driver2Location; //hashmap of (driver id, location)
    
public:
    MiniUber() {
        
    }

    // @param driver_id an integer
    // @param lat, lng driver's location
    // return matched trip information if there have matched rider or NULL
    Trip* report(int driver_id, double lat, double lng) {
        //find the driver's current trip if he is working on a order
        // or save the new location of the driver
        if (driver2Trip.find(driver_id) != driver2Trip.end())
            return driver2Trip[driver_id];

        if (driver2Location.find(driver_id) != driver2Location.end()) {
            driver2Location[driver_id]->lat = lat;
            driver2Location[driver_id]->lng = lng;
        } else {
            driver2Location[driver_id] = new Location(lat, lng);
        }
        return NULL;
    }

    // @param rider_id an integer
    // @param lat, lng driver's location
    // return a trip
    Trip* request(int rider_id, double lat, double lng) {
        /
        Trip* trip = new Trip(rider_id, lat, lng);
        unordered_map<int, Location*>::iterator it = driver2Location.begin();

        double distance = -1;
        int driver_id = -1;
        //Find the nearest driver
        for (; it != driver2Location.end(); ++it) {
            double dis = Helper::get_distance(it->second->lat, it->second->lng, lat, lng);
            if (distance < 0 || distance > dis) {
                driver_id = it->first;
                distance = dis;
            }
        }
        
        //let the nearest driver take the trip order, and stop him from reporting location
        if (driver_id != -1)
            driver2Location.erase(driver2Location.find(driver_id));
        trip->driver_id = driver_id;
        driver2Trip[driver_id] = trip;
        return trip;
    }

};