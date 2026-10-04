# DECISIONS

## Design change: BFS -> Dijkstra
I first considered BFS because it is simple. Testing USDI Block -> Auditorium on my campus map showed BFS returns 2 hops but 530 m, while the real shortest route is 3 hops and 430 m. BFS ignores distance, so I switched the main algorithm to Dijkstra and kept BFS only as a comparison.

## Edge cases
- **Two gates:** the campus has two entrances, so I modelled them as two nodes: "Main Gate" (the college's main entry, next to the Auditorium) and "Gurdwara Rd Gate" (on the road side). Each connects to the nearest buildings.
- **Unreachable location:** if a location has no path to the start, Dijkstra never reaches it and the program prints "no route found" instead of crashing.
- **Non-positive distances:** rejected in `addPath`, since Dijkstra is only correct for non-negative weights.
- **Same start and destination:** handled up front (0 m).
- **Unknown or differently-cased names:** resolved case-insensitively, otherwise the user is asked to re-enter.

## Early exit
Dijkstra stops when the destination is popped from the heap, because its distance is final at that point. This saves work on large graphs.

<!-- Edit this file in your own words before submitting. You may be asked about every line. -->

## Map data
I replaced my first sample graph with the real campus layout. Distances are estimates read from satellite screenshots, so I treat them as approximate. The algorithm does not depend on the exact numbers.
