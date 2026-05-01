# Ride Sharing Service - HLD + Basic Implementation (Without Patterns)

---

## 1. Overview

Basic ride-sharing system without design patterns. Focus is on core flow and entities.

---

## 2. Core Flow

1. Rider requests ride
2. System assigns first available driver
3. Ride completes
4. Payment processed

---

## 3. C++ Implementation (Simple, No Patterns)

```cpp
#include <bits/stdc++.h>
using namespace std;

enum RideStatus {
    REQUESTED,
    ACCEPTED,
    COMPLETED
};

class User {
protected:
    int id;
    string name;

public:
    User(int id, string name) : id(id), name(name) {}
};

class Rider : public User {
public:
    Rider(int id, string name) : User(id, name) {}
};

class Driver : public User {
    bool available;

public:
    Driver(int id, string name) : User(id, name), available(true) {}

    bool isAvailable() { return available; }
    void setAvailable(bool val) { available = val; }
};

class Ride {
public:
    int id;
    Rider* rider;
    Driver* driver;
    RideStatus status;

    Ride(int id, Rider* r) {
        this->id = id;
        rider = r;
        driver = nullptr;
        status = REQUESTED;
    }
};

class RideService {
    vector<Driver*> drivers;
    int rideId = 1;

public:
    void addDriver(Driver* d) {
        drivers.push_back(d);
    }

    Ride* requestRide(Rider* rider) {
        Ride* ride = new Ride(rideId++, rider);

        for (auto d : drivers) {
            if (d->isAvailable()) {
                ride->driver = d;
                d->setAvailable(false);
                ride->status = ACCEPTED;
                cout << "Driver assigned\n";
                break;
            }
        }

        return ride;
    }

    void completeRide(Ride* ride) {
        ride->status = COMPLETED;
        ride->driver->setAvailable(true);

        double fare = 100; // flat
        cout << "Ride completed. Fare: " << fare << endl;
    }
};

int main() {
    RideService service;

    Driver* d1 = new Driver(1, "Driver1");
    service.addDriver(d1);

    Rider* r1 = new Rider(1, "Rider1");

    Ride* ride = service.requestRide(r1);
    service.completeRide(ride);

    return 0;
}
```

---

## 4. Limitations

* Tight coupling
* No extensibility
* Hardcoded pricing
* No proper lifecycle handling
* No abstraction layers
