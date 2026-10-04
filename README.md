# Campus Navigation System (C++)

Finds the shortest walking route between two campus locations.

## Build and run
```
g++ -std=c++17 -o navigator main.cpp
./navigator          # Windows: navigator.exe
```
Type a start and destination (case-insensitive). Output shows the Dijkstra route and total metres, plus a BFS (fewest hops) route for comparison.

## Campus map
The graph models the USAR / GGSIPU Dwarka campus: Gurdwara Rd Gate, USAP Block, USDI Block, USAR Block, Admin Block, Auditorium, Main Gate, Sports Hall, Boys Hostel and USAR Ground. Locations were read from Google Maps satellite views. **Distances are estimates in metres** along walkable roads and paths, not surveyed measurements, and can be changed in `buildCampus()`.

## Graph representation
Undirected weighted graph as an **adjacency list**: `map<string, vector<pair<string,int>>>` (location -> list of (neighbour, metres)). Each path is added in both directions. Campuses are sparse, so this uses O(V + E) space; a matrix would use O(V^2).

## Why Dijkstra
Edges have different walking distances, so "shortest" means lowest total metres, not fewest hops. BFS only minimises hops, so it can return a longer walk (USDI Block -> Auditorium: BFS 2 hops but 530 m, Dijkstra 3 hops but 430 m). Dijkstra is correct because all weights are positive (enforced in `addPath`). A* is optional and would need coordinates for a heuristic, which is unnecessary for a graph this small.

## Complexity
V = locations, E = paths. (Using `string` keys in `std::map` adds a small constant-factor cost; mapping names to integer ids would remove it.)
| Part | Time | Space |
|---|---|---|
| Dijkstra (min-heap) | O((V + E) log V) | O(V) |
| BFS | O(V + E) | O(V) |
| Graph storage | - | O(V + E) |
| Path rebuild | O(V) | O(V) |

## Edge cases handled
unknown location, same start and destination, unreachable locations (prints "no route found" instead of crashing), non-positive distances rejected, case-insensitive input.

## Files
`main.cpp`, `DECISIONS.md`, `AI_USAGE.md`
