#include "GeographicLock.h"
#include "../../testing/catch.hpp"

#include "../Coordinates/Coordinates.h"
#include "../CurrentLocationStrategy/MockCurrentLocationStrategy.h"

TEST_CASE("get_state throws if no zones", "[GeographicLock]") {
    Coordinates coordinates = Coordinates(42.359285, -71.068276);
    CurrentLocationStrategy* current_location_strategy = new MockCurrentLocationStrategy(coordinates);
    GeographicLock lock = GeographicLock(current_location_strategy);

    REQUIRE_THROWS(lock.get_state());
}

TEST_CASE("when no visits get_state returns 0 zones visited and false for all zones visited", "[GeographicLock]") {
    Coordinates coordinates = Coordinates();
    CurrentLocationStrategy* current_location_strategy = new MockCurrentLocationStrategy(coordinates);
    GeographicLock lock = GeographicLock(current_location_strategy);
    vector<Zone> zones {
        Zone(42.359285, -71.068276, 0.015),
        Zone(42.354927, -71.091457, 0.030)
    };
    lock.set_zones(zones);

    GeographicLockState state = lock.get_state();

    REQUIRE(state.number_of_zones_visited == 0);
    REQUIRE(state.have_all_zones_been_visited == false);
}

TEST_CASE("when 1 of 2 visits, get_state returns 1 zones visited and false for all zones visited", "[GeographicLock]") {
    Coordinates coordinates = Coordinates(42.359285, -71.068276);
    CurrentLocationStrategy* current_location_strategy = new MockCurrentLocationStrategy(coordinates);
    GeographicLock lock = GeographicLock(current_location_strategy);
    vector<Zone> zones {
        Zone(42.359285, -71.068276, 0.015),
        Zone(42.354927, -71.091457, 0.030)
    };
    lock.set_zones(zones);

    GeographicLockState state = lock.get_state();

    REQUIRE(state.number_of_zones_visited == 1);
    REQUIRE(state.have_all_zones_been_visited == false);
}

TEST_CASE("when 2 of 2 visits, get_state returns 2 zones visited and true for all zones visited", "[GeographicLock]") {
    Coordinates coordinates = Coordinates(42.359285, -71.068276);
    MockCurrentLocationStrategy* current_location_strategy = new MockCurrentLocationStrategy(coordinates);
    GeographicLock lock = GeographicLock(current_location_strategy);
    vector<Zone> zones {
        Zone(42.359285, -71.068276, 0.015),
        Zone(42.354927, -71.091457, 0.030)
    };
    lock.set_zones(zones);

    GeographicLockState state = lock.get_state();
    current_location_strategy->coordinates = Coordinates(42.354927, -71.091457);
    state = lock.get_state();

    REQUIRE(state.number_of_zones_visited == 2);
    REQUIRE(state.have_all_zones_been_visited == true);
}

TEST_CASE("get_state returns distance to next zone", "[GeographicLock]") {
    Coordinates coordinates = Coordinates(42.359285, -71.068276);
    CurrentLocationStrategy* current_location_strategy = new MockCurrentLocationStrategy(coordinates);
    GeographicLock lock = GeographicLock(current_location_strategy);
    vector<Zone> zones { Zone(42.354927, -71.091457, 0.015) };
    lock.set_zones(zones);

    GeographicLockState state = lock.get_state();

    REQUIRE(state.distance_to_zone_being_located == Approx(1.2213).epsilon(0.01));
}