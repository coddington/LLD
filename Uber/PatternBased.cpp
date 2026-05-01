# Ride Sharing Service - LLD + Full Pattern-Based Implementation

---

## 1. Overview

This implementation integrates multiple design patterns:

* Strategy
* Observer
* State
* Decorator
* Command
* Repository

---

## 2. Full C++ Implementation

```cpp
#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////
// Observer Pattern
//////////////////////////////////////////////////////

class Observer {
public:
    virtual void update(const string& msg) = 0;
};

class Subject {
    vector<Observer*> observers;

public:
    void subscribe(Observer* o) {
        observers.push_back(o);
    }

    void notify(const string& msg) {
        for (auto o : observers)
            o->update(msg);
    }
};

//////////////////////////////////////////////////////
// Users
//////////////////////////////////////////////////////

class User {
protected:
    int id;
    string name;

public:
    User(int id, string name) : id(id), name(name) {}
};

class Rider : public User, public Observer {
public:
    Rider(int id, string name) : User(id, name) {}

    void update(const string& msg) override {
        cout << "[Rider] " << name << ": " << msg << endl;
    }
};

class Driver : public User, public Observer {
    bool available;

public:
    Driver(int id, string name)
        : User(id, name), available(true) {}

    bool isAvailable() { return available; }
    void setAvailable(bool val) { available = val; }

    void update(const string& msg) override {
        cout << "[Driver] " << name << ": " << msg << endl;
    }
};

//////////////////////////////////////////////////////
// State Pattern
//////////////////////////////////////////////////////

class Ride;

class RideState {
public:
    virtual void next(Ride* ride) = 0;
    virtual string name() = 0;
};

class RequestedState : public RideState {
public:
    void next(Ride* ride) override;
    string name() override { return "REQUESTED"; }
};

class AcceptedState : public RideState {
public:
    void next(Ride* ride) override;
    string name() override { return "ACCEPTED"; }
};

class CompletedState : public RideState {
public:
    void next(Ride* ride) override {
        cout << "Already completed\n";
    }
    string name() override { return "COMPLETED"; }
};

//////////////////////////////////////////////////////
// Ride
//////////////////////////////////////////////////////

class Ride : public Subject {
    int id;
    Rider* rider;
    Driver* driver;
    RideState* state;

public:
    Ride(int id, Rider* r)
        : id(id), rider(r), driver(nullptr) {
        state = new RequestedState();
        subscribe(rider);
    }

    void assignDriver(Driver* d) {
        driver = d;
        subscribe(d);
        notify("Driver assigned");
    }

    void setState(RideState* s) {
        state = s;
    }

    void next() {
        state->next(this);
        notify("State: " + state->name());
    }

    Driver* getDriver() { return driver; }
};

void RequestedState::next(Ride* ride) {
    ride->setState(new AcceptedState());
}

void AcceptedState::next(Ride* ride) {
    ride->setState(new CompletedState());
}

//////////////////////////////////////////////////////
// Strategy Pattern (Matching)
//////////////////////////////////////////////////////

class MatchingStrategy {
public:
    virtual Driver* match(vector<Driver*>& drivers) = 0;
};

class SimpleMatching : public MatchingStrategy {
public:
    Driver* match(vector<Driver*>& drivers) override {
        for (auto d : drivers)
            if (d->isAvailable()) return d;
        return nullptr;
    }
};

//////////////////////////////////////////////////////
// Decorator Pattern (Pricing)
//////////////////////////////////////////////////////

class Fare {
public:
    virtual double cost() = 0;
};

class BaseFare : public Fare {
public:
    double cost() override { return 100; }
};

class FareDecorator : public Fare {
protected:
    Fare* fare;
public:
    FareDecorator(Fare* f) : fare(f) {}
};

class SurgeFare : public FareDecorator {
public:
    SurgeFare(Fare* f) : FareDecorator(f) {}
    double cost() override { return fare->cost() + 50; }
};

class NightFare : public FareDecorator {
public:
    NightFare(Fare* f) : FareDecorator(f) {}
    double cost() override { return fare->cost() + 30; }
};

//////////////////////////////////////////////////////
// Command Pattern
//////////////////////////////////////////////////////

class Command {
public:
    virtual void execute() = 0;
};

class AcceptRideCommand : public Command {
    Ride* ride;
    Driver* driver;

public:
    AcceptRideCommand(Ride* r, Driver* d)
        : ride(r), driver(d) {}

    void execute() override {
        driver->setAvailable(false);
        ride->assignDriver(driver);
        ride->next();
    }
};

class CompleteRideCommand : public Command {
    Ride* ride;

public:
    CompleteRideCommand(Ride* r) : ride(r) {}

    void execute() override {
        ride->next();
    }
};

//////////////////////////////////////////////////////
// Repository Pattern
//////////////////////////////////////////////////////

class RideRepository {
    unordered_map<int, Ride*> db;

public:
    void save(int id, Ride* ride) {
        db[id] = ride;
    }

    Ride* get(int id) {
        return db[id];
    }
};

//////////////////////////////////////////////////////
// RideService
//////////////////////////////////////////////////////

class RideService {
    vector<Driver*> drivers;
    MatchingStrategy* strategy;
    RideRepository repo;
    int rideId = 1;

public:
    RideService(MatchingStrategy* s) : strategy(s) {}

    void addDriver(Driver* d) {
        drivers.push_back(d);
    }

    Ride* requestRide(Rider* rider) {
        Ride* ride = new Ride(rideId, rider);

        Driver* driver = strategy->match(drivers);

        if (driver) {
            AcceptRideCommand cmd(ride, driver);
            cmd.execute();
        }

        repo.save(rideId++, ride);
        return ride;
    }

    void completeRide(Ride* ride) {
        CompleteRideCommand cmd(ride);
        cmd.execute();

        Fare* fare = new BaseFare();
        fare = new SurgeFare(fare);
        fare = new NightFare(fare);

        cout << "Total Fare: " << fare->cost() << endl;

        ride->getDriver()->setAvailable(true);
    }
};

//////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////

int main() {
    MatchingStrategy* strategy = new SimpleMatching();
    RideService service(strategy);

    Driver* d1 = new Driver(1, "Driver1");
    service.addDriver(d1);

    Rider* r1 = new Rider(1, "Rider1");

    Ride* ride = service.requestRide(r1);
    service.completeRide(ride);

    return 0;
}
```

---

## 3. Key Benefits

* Fully modular
* Extensible system
* Real-world architecture alignment
* Easy to scale and maintain
