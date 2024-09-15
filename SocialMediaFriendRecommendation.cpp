#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>

using namespace std;

// Function to perform BFS and recommend friends based on mutual connections
vector<pair<string, int>> bfsRecommendations(unordered_map<string, vector<string>>& graph, const string& startUser) 
{
    queue<string> q;                              // Queue for BFS traversal
    unordered_set<string> visited;                // To keep track of visited users
    unordered_map<string, int> recommendations;   // To store potential friends and mutual friend counts

    // Start BFS from the given user
    q.push(startUser);
    visited.insert(startUser);

    while (!q.empty()) {
        string currentUser = q.front();
        q.pop();

        for (const string& friendUser : graph[currentUser]) {
            // If friendUser hasn't been visited yet, add it to the queue
            if (visited.find(friendUser) == visited.end()) {
                visited.insert(friendUser);
                q.push(friendUser);
            }

            // Check friends of the current friend (mutual friends check)
            for (const string& mutualFriend : graph[friendUser]) {
                // Ensure mutualFriend is not the original user and is not a direct friend of startUser
                if (mutualFriend != startUser && find(graph[startUser].begin(), graph[startUser].end(), mutualFriend) == graph[startUser].end()) {
                    recommendations[mutualFriend]++; // Increment count for mutual friend
                }
            }
        }
    }

    // Convert the recommendations map to a vector of pairs for sorting
    vector<pair<string, int>> sortedRecommendations(recommendations.begin(), recommendations.end());

    // Sort recommendations based on the number of mutual friends (in descending order)
    sort(sortedRecommendations.begin(), sortedRecommendations.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return b.second > a.second; // Sort in descending order of mutual friend count
    });

    return sortedRecommendations;
}

int main() {
    unordered_map<string, vector<string>> graph;
    int numUsers, numFriends;
    string user, friendUser;

    // Input: Number of users
    cout << "Enter the number of users: ";
    cin >> numUsers;

    // Input: User connections (graph creation)
    for (int i = 0; i < numUsers; i++) {
        cout << "Enter the name of user " << i + 1 << ": ";
        cin >> user;

        cout << "Enter the number of friends for " << user << ": ";
        cin >> numFriends;
        
        for (int j = 0; j < numFriends; j++) {
            cout << "Enter friend " << j + 1 << " of " << user << ": ";
            cin >> friendUser;
            graph[user].push_back(friendUser);  // Add friend to user's list
            graph[friendUser].push_back(user);  // Add the reverse connection for an undirected graph
        }
    }

    // Input: User for whom recommendations are required
    string startUser;
    cout << "\nEnter the user for whom you want friend recommendations: ";
    cin >> startUser;

    // Check if the user exists in the graph
    if (graph.find(startUser) == graph.end()) {
        cout << "User " << startUser << " does not exist in the graph.\n";
        return 1;
    }

    // Get friend recommendations
    vector<pair<string, int>> recommendations = bfsRecommendations(graph, startUser);

    // Output: Recommended friends based on mutual connections
    cout << "\nFriend recommendations for " << startUser << " based on mutual friends:\n";
    if (recommendations.empty()) {
        cout << "No friend recommendations available.\n";
    } 
    else 
    {
        for (const auto& rec : recommendations) 
        {
            cout << rec.first << " with " << rec.second/4 << " mutual friends.\n";
        }
    }

    return 0;
}
