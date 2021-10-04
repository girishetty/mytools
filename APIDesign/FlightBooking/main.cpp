#include <map>
#include <vector>
#include <cstdint>
#include <cinttypes>

/**
 * - Book a flight for a passenger.
 * - Find all of the flights for a passenger.
 * - Find all of the passengers for a flight.
 */

class Passenger {
public:
  Passenger(uint64_t passenger_id, uint64_t flight_id) : passenger_id_(passenger_id) {
    addFlight(flight_id);
  }

  Passenger(const Passenger& passenger) : passenger_id_(passenger.passenger_id_) {
    flights_ = passenger.flights_;
  }

  Passenger& operator=(const Passenger& passenger) {
    passenger_id_ = passenger.passenger_id_;
    flights_ = passenger.flights_;
    return *this;
  }

  void addFlight(uint64_t flight_id) {
    if (flight_id != 0) {
      flights_.push_back(flight_id);
    }
  }

  const std::vector<uint64_t>& getFlights() const {
    return flights_;
  }

private:
  uint64_t passenger_id_;
  std::vector<uint64_t> flights_;
};

class Flight {
public:
  Flight(uint64_t flight_id, uint64_t passenger_id) : flight_id_(flight_id) {
    addPassenger(passenger_id);
  }

  Flight(const Flight& flight) : flight_id_(flight.flight_id_) {
    passengers_ = flight.passengers_;
  }

  Flight& operator=(const Flight& flight) {
    flight_id_ = flight.flight_id_;
    passengers_ = flight.passengers_;
    return *this;
  }

  void addPassenger(uint64_t passenger_id) {
    if (passenger_id != 0) {
      passengers_.push_back(passenger_id);
    }
  }

  const std::vector<uint64_t>& getPassengers() const {
    return passengers_;
  }

private:
  uint64_t flight_id_;
  std::vector<uint64_t> passengers_;
};

// Map of Passenger ID and Passenger.
std::map<uint64_t, Passenger> passengerList;

// Map of Flight ID and Flights.
std::map<uint64_t, Flight> flightList;

void makeBooking(uint64_t passenger_id, uint64_t flight_id) {
  if (passengerList.count(passenger_id) > 0) {
    passengerList.find(passenger_id)->second.addFlight(flight_id);
  } else {
    Passenger passenger(passenger_id, flight_id);
    passengerList.insert(std::pair<uint64_t, Passenger>(passenger_id, passenger));
  }

  if (flightList.count(flight_id) > 0) {
    flightList.find(flight_id)->second.addPassenger(passenger_id);
  } else {
    Flight flight(flight_id, passenger_id);
    flightList.insert(std::pair<uint64_t, Flight>(flight_id, flight));
  }
}

int main() {
  return 0;
}
