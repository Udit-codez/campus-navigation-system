// Campus Navigation System (C++17)
// Weighted undirected graph (metres) -> Dijkstra for the shortest route.
// BFS is included only to compare "fewest hops" with "shortest distance".
#include <algorithm>
#include <cctype>
#include <climits>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

using Edge = pair<string, int>;  // (neighbour, metres)

static string lower(string s) {
    transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
    return s;
}

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

class CampusGraph {
public:
    void addLocation(const string& name) {
        adj[name];  // creates an empty list if missing
        lowerToName[lower(name)] = name;
    }

    void addPath(const string& a, const string& b, int metres) {
        if (metres <= 0)  // Dijkstra needs positive weights
            throw invalid_argument("Distance " + a + "-" + b + " must be positive");
        addLocation(a);
        addLocation(b);
        adj[a].push_back({b, metres});
        adj[b].push_back({a, metres});  // paths are two-way
    }

    // Case-insensitive lookup: "library" -> "Library"
    bool resolve(const string& text, string& out) const {
        auto it = lowerToName.find(lower(trim(text)));
        if (it == lowerToName.end()) return false;
        out = it->second;
        return true;
    }

    vector<string> locations() const {
        vector<string> v;
        for (auto& kv : adj) v.push_back(kv.first);
        return v;
    }

    // Shortest by total metres. Returns false if unreachable.
    bool dijkstra(const string& src, const string& dst, vector<string>& path, int& cost) const {
        map<string, int> dist;
        map<string, string> prev;
        using P = pair<int, string>;
        priority_queue<P, vector<P>, greater<P>> pq;  // min-heap
        dist[src] = 0;
        pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;  // stale heap entry
            if (u == dst) break;        // dst distance is final once popped
            for (const auto& [v, w] : adj.at(u)) {
                int nd = d + w;
                auto it = dist.find(v);
                if (it == dist.end() || nd < it->second) {
                    dist[v] = nd;
                    prev[v] = u;
                    pq.push({nd, v});
                }
            }
        }
        if (!dist.count(dst)) return false;
        path = rebuild(prev, src, dst);
        cost = dist[dst];
        return true;
    }

    // Fewest hops, ignores distance. Returns false if unreachable.
    bool bfs(const string& src, const string& dst, vector<string>& path, int& cost) const {
        map<string, string> prev;
        set<string> seen = {src};
        queue<string> q;
        q.push(src);
        while (!q.empty()) {
            string u = q.front();
            q.pop();
            if (u == dst) break;
            for (const auto& [v, w] : adj.at(u)) {
                (void)w;
                if (seen.insert(v).second) {
                    prev[v] = u;
                    q.push(v);
                }
            }
        }
        if (!seen.count(dst)) return false;
        path = rebuild(prev, src, dst);
        cost = pathCost(path);
        return true;
    }

private:
    map<string, vector<Edge>> adj;       // adjacency list
    map<string, string> lowerToName;

    int pathCost(const vector<string>& path) const {
        int total = 0;
        for (size_t i = 0; i + 1 < path.size(); ++i) {
            int best = INT_MAX;
            for (const auto& [v, w] : adj.at(path[i]))
                if (v == path[i + 1]) best = min(best, w);
            total += best;
        }
        return total;
    }

    static vector<string> rebuild(const map<string, string>& prev, const string& src,
                                  const string& dst) {
        vector<string> path = {dst};
        while (path.back() != src) path.push_back(prev.at(path.back()));
        reverse(path.begin(), path.end());
        return path;
    }
};

static CampusGraph buildSampleCampus() {
    CampusGraph g;
    // Replace with your real campus buildings and measured distances.
    g.addPath("Main Gate", "Admin Block", 150);
    g.addPath("Main Gate", "Parking", 80);
    g.addPath("Parking", "Canteen", 120);
    g.addPath("Admin Block", "Library", 100);
    g.addPath("Admin Block", "Canteen", 90);
    g.addPath("Library", "CS Block", 60);
    g.addPath("Canteen", "CS Block", 200);
    g.addPath("Canteen", "Auditorium", 110);
    g.addPath("CS Block", "Auditorium", 140);
    g.addPath("Auditorium", "Hostel", 250);
    g.addPath("Library", "Hostel", 400);
    g.addLocation("Old Annex");  // deliberately disconnected (edge case)
    return g;
}

static void show(const string& label, bool found, const vector<string>& path, int cost) {
    cout << label << ": ";
    if (!found) {
        cout << "no route found\n";
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) cout << (i ? " -> " : "") << path[i];
    cout << "  |  " << path.size() - 1 << " hops, " << cost << " m\n";
}

int main() {
    CampusGraph g = buildSampleCampus();
    cout << "Locations: ";
    auto names = g.locations();
    for (size_t i = 0; i < names.size(); ++i) cout << (i ? ", " : "") << names[i];
    cout << "\n";

    string s, d, src, dst;
    while (true) {
        cout << "\nStart (blank to quit): ";
        if (!getline(cin, s) || trim(s).empty()) break;
        cout << "Destination: ";
        if (!getline(cin, d)) break;
        if (!g.resolve(s, src) || !g.resolve(d, dst)) {
            cout << "Unknown location. Please pick from the list above.\n";
            continue;
        }
        if (src == dst) {
            cout << "You are already there (0 m).\n";
            continue;
        }
        vector<string> path;
        int cost = 0;
        bool ok = g.dijkstra(src, dst, path, cost);
        show("Shortest (Dijkstra)", ok, path, cost);
        ok = g.bfs(src, dst, path, cost);
        show("Fewest hops (BFS)  ", ok, path, cost);
    }
    return 0;
}
