# DECISIONS

## Design change: BFS -> Dijkstra
I first considered BFS because it is simple. Testing Main Gate -> Hostel showed BFS returns 3 hops but 650 m, while the real shortest route is 4 hops and 560 m. BFS ignores distance, so I switched the main algorithm to Dijkstra and kept BFS only as a comparison.

## Edge cases
- **Disconnected location (Old Annex):** Dijkstra never reaches it, so I return "no route found" instead of crashing.
- **Non-positive distances:** rejected in `add_path`, since Dijkstra is only correct for non-negative weights.
- **Same start and destination:** handled up front (0 m).
- **Unknown or differently-cased names:** resolved case-insensitively, otherwise the user is asked to re-enter.

## Early exit
Dijkstra stops when the destination is popped from the heap, because its distance is final at that point. This saves work on large graphs.

<!-- Edit this file in your own words before submitting. You may be asked about every line. -->
