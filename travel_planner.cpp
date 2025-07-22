#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

struct Destination {
    string name;
    string startDate;
    string endDate;
    int tripDuration = 0;
    float cost;
    vector<string> famousPlaces;
    map<string, string> visitTimings;
};

class TravelPlanner {
private:
    vector<Destination> itinerary;
    map<string, vector<string>> placeDatabase;
    map<string, float> placeAvgCost;
    float totalBudget = 0;

    void initDatabase() {
        placeDatabase["paris"] = {"Eiffel Tower", "Louvre Museum", "Notre-Dame Cathedral"};
        placeAvgCost["paris"] = 85000;

        placeDatabase["goa"] = {"Baga Beach", "Fort Aguada", "Basilica of Bom Jesus"};
        placeAvgCost["goa"] = 25000;

        placeDatabase["tokyo"] = {"Tokyo Tower", "Meiji Shrine", "Shibuya Crossing"};
        placeAvgCost["tokyo"] = 70000;

        placeDatabase["new york"] = {"Statue of Liberty", "Times Square", "Central Park"};
        placeAvgCost["new york"] = 90000;

        placeDatabase["delhi"] = {"Red Fort", "Qutub Minar", "India Gate"};
        placeAvgCost["delhi"] = 20000;

        placeDatabase["rome"] = {"Colosseum", "Pantheon", "Trevi Fountain"};
        placeAvgCost["rome"] = 75000;
    }

    int calculateDuration(const string& start, const string& end) {
        int sd = stoi(start.substr(0, 2));
        int ed = stoi(end.substr(0, 2));
        return ed - sd + 1;
    }

    string toLower(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    float currentTotalCost() {
        float total = 0;
        for (auto& d : itinerary) {
            total += d.cost;
        }
        return total;
    }

public:
    TravelPlanner() {
        initDatabase();
    }

    void setBudget() {
        cout << "Enter your total travel budget (in ₹): ";
        cin >> totalBudget;
    }

    void addDestination() {
        if (totalBudget == 0) {
            cout << "\u26a0 Please set your budget first.\n";
            return;
        }

        Destination d;
        cin.ignore();
        cout << "Enter destination name: ";
        getline(cin, d.name);
        string key = toLower(d.name);

        cout << "Enter start date (dd-mm-yyyy): ";
        getline(cin, d.startDate);
        cout << "Enter end date (dd-mm-yyyy): ";
        getline(cin, d.endDate);
        cout << "Enter estimated cost for this destination (in ₹): ";
        cin >> d.cost;
        cin.ignore();

        d.tripDuration = calculateDuration(d.startDate, d.endDate);

        if (placeDatabase.count(key)) {
            vector<string> options = placeDatabase[key];
            cout << "\nAvailable Attractions in " << d.name << ":\n";
            for (size_t i = 0; i < options.size(); ++i)
                cout << i + 1 << ". " << options[i] << endl;

            cout << "Enter place numbers to visit (space-separated): ";
            string input;
            getline(cin, input);
            istringstream iss(input);
            int choice;
            while (iss >> choice) {
                if (choice > 0 && choice <= options.size()) {
                    string place = options[choice - 1];
                    d.famousPlaces.push_back(place);
                    cout << "Enter preferred timing for " << place << ": ";
                    string timing;
                    getline(cin, timing);
                    d.visitTimings[place] = timing;
                }
            }
        } else {
            cout << "\nNo data for " << d.name << ".\n";
        }

        itinerary.push_back(d);
        cout << "Destination added.\n";
    }

    void showItinerary() {
        if (itinerary.empty()) {
            cout << "No destinations added.\n";
            return;
        }

        float totalCost = 0;
        int totalDuration = 0;
        cout << "\n========= Travel Itinerary =========\n";
        for (size_t i = 0; i < itinerary.size(); ++i) {
            Destination& d = itinerary[i];
            totalCost += d.cost;
            totalDuration += d.tripDuration;

            cout << i + 1 << ". " << d.name << "\n";
            cout << "   Dates: " << d.startDate << " to " << d.endDate << "\n";
            cout << "   Duration: " << d.tripDuration << " days\n";
            cout << "   Cost: ₹" << fixed << setprecision(2) << d.cost << "\n";

            if (!d.famousPlaces.empty()) {
                cout << "   Visits:\n";
                for (auto& place : d.famousPlaces)
                    cout << "    - " << place << " (Timing: " << d.visitTimings[place] << ")\n";
            }
            cout << "----------------------------------\n";
        }

        cout << "Total Cost: ₹" << totalCost << "\n";
        cout << "Total Duration: " << totalDuration << " days\n";
        cout << "Your Budget: ₹" << totalBudget << "\n";
        if (totalCost > totalBudget)
            cout << "\u26a0 Budget Exceeded by ₹" << totalCost - totalBudget << "\n";
        else
            cout << "✅ Within Budget. Remaining: ₹" << totalBudget - totalCost << "\n";
        cout << "===================================\n";
    }

    void suggestDestination() {
        float remaining = totalBudget - currentTotalCost();
        cout << "\nSuggesting destinations under ₹" << remaining << ":\n";
        bool found = false;
        for (auto& entry : placeAvgCost) {
            if (entry.second <= remaining) {
                found = true;
                cout << "- " << entry.first << " (Est. Cost: ₹" << entry.second << ")\n";
            }
        }
        if (!found) cout << "No destinations fit within your remaining budget.\n";
    }

    void editDestination() {
        showItinerary();
        cout << "Enter destination number to edit: ";
        int idx;
        cin >> idx;
        cin.ignore();

        if (idx < 1 || idx > itinerary.size()) {
            cout << "Invalid index.\n";
            return;
        }

        Destination& d = itinerary[idx - 1];
        cout << "Editing: " << d.name << "\n";

        cout << "Enter new name (or press Enter to keep as " << d.name << "): ";
        string input;
        getline(cin, input);
        if (!input.empty()) d.name = input;

        cout << "Enter new start date (dd-mm-yyyy) [current: " << d.startDate << "]: ";
        getline(cin, input);
        if (!input.empty()) d.startDate = input;

        cout << "Enter new end date (dd-mm-yyyy) [current: " << d.endDate << "]: ";
        getline(cin, input);
        if (!input.empty()) d.endDate = input;

        cout << "Enter new cost [current: ₹" << d.cost << "]: ";
        getline(cin, input);
        if (!input.empty()) d.cost = stof(input);

        d.tripDuration = calculateDuration(d.startDate, d.endDate);

        cout << "Destination updated successfully.\n";
    }

    void sortItineraryByDate() {
        sort(itinerary.begin(), itinerary.end(), [](const Destination& a, const Destination& b) {
            return a.startDate < b.startDate;
        });
        cout << "Sorted by start date.\n";
    }

    void sortItineraryByCost() {
        sort(itinerary.begin(), itinerary.end(), [](const Destination& a, const Destination& b) {
            return a.cost < b.cost;
        });
        cout << "Sorted by cost.\n";
    }

    void searchDestination() {
        cin.ignore();
        cout << "Enter destination name to search: ";
        string name;
        getline(cin, name);
        bool found = false;
        for (const auto& d : itinerary) {
            if (toLower(d.name) == toLower(name)) {
                found = true;
                cout << "\nFound: " << d.name << ", " << d.startDate << " to " << d.endDate;
                cout << ", Cost: ₹" << d.cost << "\n";
                break;
            }
        }
        if (!found) cout << "Not found.\n";
    }

    void deleteDestination() {
        showItinerary();
        cout << "Enter destination number to delete: ";
        int index;
        cin >> index;
        if (index > 0 && index <= itinerary.size()) {
            itinerary.erase(itinerary.begin() + index - 1);
            cout << "Destination removed.\n";
        } else {
            cout << "Invalid index.\n";
        }
    }

    void menu() {
        int choice;
        do {
            cout << "\n--- Smart Travel Planner Menu ---\n";
            cout << "1. Set Budget\n";
            cout << "2. Add Destination\n";
            cout << "3. Show Itinerary\n";
            cout << "4. Search Destination\n";
            cout << "5. Sort by Date\n";
            cout << "6. Sort by Cost\n";
            cout << "7. Delete Destination\n";
            cout << "8. Edit Destination\n";
            cout << "9. Suggest Place (Based on Budget Left)\n";
            cout << "10. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: setBudget(); break;
                case 2: addDestination(); break;
                case 3: showItinerary(); break;
                case 4: searchDestination(); break;
                case 5: sortItineraryByDate(); break;
                case 6: sortItineraryByCost(); break;
                case 7: deleteDestination(); break;
                case 8: editDestination(); break;
                case 9: suggestDestination(); break;
                case 10: cout << "Thank you for using Smart Travel Planner!\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } while (choice != 10);
    }
};

int main() {
    TravelPlanner planner;
    planner.menu();
    return 0;
}
