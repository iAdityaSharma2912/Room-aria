#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory> // For smart pointers
#include <limits> // For input validation

using namespace std;

// Class representing a room with seating and reservation information.
class Room {
    string roomNumber;
    string host;
    string startTime;
    string endTime;
    string from;
    string to;
    vector<string> chairs; // Initialized dynamically to "Empty"

public:
    Room(const string& rn, const string& h, const string& st, const string& et, const string& f, const string& t)
        : roomNumber(rn), host(h), startTime(st), endTime(et), from(f), to(t) {}

    void initializeChairs(int numChairs) {
        chairs.resize(numChairs, "Empty");
    }

    const string& getRoomNumber() const {
        return roomNumber;
    }

    int getNumChairs() const {
        return chairs.size();
    }

    bool isSeatAvailable(int chairIndex) const {
        return chairIndex >= 0 && chairIndex < chairs.size() && chairs[chairIndex] == "Empty";
    }

    bool reserveSeat(int chairIndex, const string& name) {
        if (chairIndex < 0 || chairIndex >= chairs.size()) {
            cout << "Invalid seat number." << endl;
            return false;
        }
        if (isSeatAvailable(chairIndex)) {
            chairs[chairIndex] = name;
            return true;
        }
        cout << "Seat " << (chairIndex + 1) << " is already occupied." << endl;
        return false;
    }

    void displayRoom() const {
        cout << "Room Number: " << roomNumber << "\nHost: " << host << "\nStart Time: " << startTime
             << "\nEnd Time: " << endTime << "\nFrom: " << from << "\nTo: " << to << "\n\n";

        cout << "Seating Arrangement:\n";
        for (size_t i = 0; i < chairs.size(); ++i) {
            cout << "Seat " << (i + 1) << ": " << chairs[i];
            if ((i + 1) % 4 == 0) { // New line after every 4 seats
                cout << "\n";
            } else {
                cout << "\t"; // Separate chairs with a tab
            }
        }
    }
};

// Function to get a valid integer input
int getValidInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "Invalid input. Please enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    vector<unique_ptr<Room>> hotelRooms; // Using unique_ptr for memory management

    while (true) {
        cout << "\nHotel Room Booking System\n"
             << "1. Book Room\n"
             << "2. Reserve Seat\n"
             << "3. Display Room Information\n"
             << "4. List All Rooms\n"
             << "5. Exit\n"
             << "Enter your choice: ";
        
        int choice = getValidInt(""); // Validate integer input

        switch (choice) {
            case 1: { // Book a room
                string roomNumber, host, startTime, endTime, from, to;
                cout << "Enter room number: ";
                cin >> roomNumber;
                cout << "Enter host name: ";
                cin >> host;
                cout << "Enter start time: ";
                cin >> startTime;
                cout << "Enter end time: ";
                cin >> endTime;
                cout << "Enter 'from': ";
                cin >> from;
                cout << "Enter 'to': ";
                cin >> to;

                auto newRoom = make_unique<Room>(roomNumber, host, startTime, endTime, from, to);
                int numChairs = getValidInt("Enter the number of chairs in the room (1-40): ");
                newRoom->initializeChairs(numChairs);

                hotelRooms.push_back(move(newRoom)); // Store the new room
                cout << "Room booked successfully." << endl;
                break;
            }
            case 2: { // Reserve a seat
                string roomNumber;
                cout << "Enter room number: ";
                cin >> roomNumber;

                auto it = find_if(hotelRooms.begin(), hotelRooms.end(), [&roomNumber](const unique_ptr<Room>& r) {
                    return r->getRoomNumber() == roomNumber;
                });

                if (it != hotelRooms.end()) {
                    int chairNumber = getValidInt("Enter seat number (1-" + to_string((*it)->getNumChairs()) + "): ") - 1;
                    cout << "Enter traveler name: ";
                    string travelerName;
                    cin >> travelerName;

                    bool reserved = (*it)->reserveSeat(chairNumber, travelerName);
                    if (reserved) {
                        cout << "Seat reserved successfully." << endl;
                    }
                } else {
                    cout << "Room not found." << endl;
                }
                break;
            }
            case 3: { // Display room information
                string roomNumber;
                cout << "Enter room number: ";
                cin >> roomNumber;

                auto it = find_if(hotelRooms.begin(), hotelRooms.end(), [&roomNumber](const unique_ptr<Room>& r) {
                    return r->getRoomNumber() == roomNumber;
                });

                if (it != hotelRooms.end()) {
                    (*it)->displayRoom();
                } else {
                    cout << "Room not found." << endl;
                }
                break;
            }
            case 4: { // List all rooms
                if (hotelRooms.empty()) {
                    cout << "No rooms available." << endl;
                } else {
                    cout << "Available Rooms:\n";
                    for (const auto& room : hotelRooms) {
                        cout << "Room Number: " << room->getRoomNumber() << endl;
                    }
                }
                break;
            }
            case 5: // Exit
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
