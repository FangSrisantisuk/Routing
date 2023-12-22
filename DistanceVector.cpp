#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Node struct to hold information for each Distance Vector (DV) node
struct DVNode {
    int origin;
    int destination;
    int distance;
    string source;
};

// Hash map to convert routers into individual indexes [https://en.cppreference.com/w/cpp/container/map]
map<string, int> table;
const int negativeValue = -9999;

// Function to remove spaces from a string
vector<string> removeSpacesFromString(string input) {
    vector<string> values;
    istringstream iss(input);
    string word;

    while (iss >> word) {
        values.push_back(word);
    }

    return values;
}

// Method to implement the Distance Vector algorithm
void distanceVector(vector<string> routers, vector<string> links, vector<vector<string>> updated_links) {
    const long int infinity = 10000000;
    const int n = routers.size();
    int t = 0;
    vector<DVNode> result;
    int link_table[n][n];
    DVNode updated_table[n][n];
    // Initialize tables
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            link_table[i][j] = (i == j) ? 0 : negativeValue;
            updated_table[i][j].distance = (i == j) ? 0 : infinity;
        }
    }
    // Initialize table for link edges with edge weights
    for (unsigned int i = 0; i < links.size(); i++) {
        vector<string> updates = removeSpacesFromString(links[i]);
        link_table[table[updates[0]]][table[updates[1]]] = stoi(updates[2]);
        link_table[table[updates[1]]][table[updates[0]]] = stoi(updates[2]);
    }
    // Main distance vector algorithm
    for (unsigned int i = 0; i < updated_links.size() + 1; i++) {
        while (t == 0 || !result.empty()) {
            result.clear();
            vector<string> sorted_routers = routers;
            sort(sorted_routers.begin(), sorted_routers.end());
            for (const string &router : sorted_routers) {
                cout << router << " Distance Table at t=" << t << endl;
                for (const string &other_router : sorted_routers) {
                    if (other_router != router)
                        cout << "\t" << other_router;
                } cout << endl;
                // Calculate shortest paths for each destination router
                for (const string &dest_router : sorted_routers) {
                    if (dest_router != router) {
                        cout << dest_router << "\t";
                        int min = infinity;
                        DVNode new_update;
                        // Iterate over neighbor routers to find the minimum distance
                        for (const string &neighbor : sorted_routers) {
                            if (neighbor != router) {
                                if (updated_table[table[neighbor]][table[dest_router]].distance >= infinity || link_table[table[router]][table[neighbor]] == negativeValue) {
                                    cout << "INF\t";
                                    continue;
                                }
                                int total_dist = link_table[table[router]][table[neighbor]] + updated_table[table[neighbor]][table[dest_router]].distance;
                                cout << total_dist << "\t";
                                if (total_dist < min) {
                                    min = total_dist;
                                    new_update.origin = table[router];
                                    new_update.destination = table[dest_router];
                                    new_update.distance = total_dist;
                                    new_update.source = neighbor;
                                }
                            }
                        } cout << endl;
                        if (min != updated_table[table[router]][table[dest_router]].distance)
                            result.push_back(new_update);
                    }
                } cout << endl;
            } t++;
            // Update the routing tables with the new distances
            for (const DVNode &update : result) {
                updated_table[update.origin][update.destination].distance = update.distance;
                updated_table[update.origin][update.destination].source = update.source;
            }
        }
        // Print the final routing tables for each router
        for (int j = 0; j < n; j++) {
            cout << routers[j] << " Routing Table: " << endl;
            vector<string> sorted_routers = routers;
            sort(sorted_routers.begin(), sorted_routers.end());
            for (const string &router : sorted_routers) {
                if (router != routers[j])
                    cout << router << "," << updated_table[j][table[router]].source << "," << updated_table[j][table[router]].distance << endl;
            } cout << endl;
        }
        // Update table with the updated links
        if (i < updated_links.size()) {
            for (const string &updated_link : updated_links[i]) {
                vector<string> updates = removeSpacesFromString(updated_link);
                link_table[table[updates[0]]][table[updates[1]]] = (updates[2] != "-1") ? stoi(updates[2]) : negativeValue;
                link_table[table[updates[1]]][table[updates[0]]] = (updates[2] != "-1") ? stoi(updates[2]) : negativeValue;
            } DVNode a;
            result.push_back(a);
        }
    }
}

// Main function
int main(int argc, char **argv) {
    // Getting input for routers
    vector<string> routers;
    string input_1;

    // Read routers until empty line or "DISTANCEVECTOR" is encountered [https://www.geeksforgeeks.org/how-to-use-getline-in-c-when-there-are-black-lines-in-input/]
    while (getline(cin, input_1)) {
        if (input_1.empty() || input_1 == "DISTANCEVECTOR") {
            break;
        }
        routers.push_back(input_1);
    }

    // Fill out the hash table
    for (unsigned int i = 0; i < routers.size(); i++) {
        table.insert(pair<string, int>(routers[i], i));
    }

    // Read input for current states
    vector<string> links;
    string input_2;

    // Read links until empty line or "UPDATE" is encountered
    while (getline(cin, input_2)) {
        if (input_2.empty() || input_2 == "UPDATE") {
            break;
        }
        links.push_back(input_2);
    }

    // Read input for 2D array of updated edge values
    vector<vector<string>> updated_links;
    vector<string> link;
    string input_3;

    // Read updated links until empty line or "END" is encountered
    while (getline(cin, input_3)) {
        if (input_3.empty() || input_3 == "END") {
            break;
        } else {
            link.push_back(input_3);
            while (getline(cin, input_3))
            if (input_3.empty() || input_3 == "END") {
                break;
            } else {
                link.push_back(input_3);
            }
        }
        updated_links.push_back(link);
        link.clear();
    }

    // Call the distance vector function
    distanceVector(routers, links, updated_links);

    return 0;
}