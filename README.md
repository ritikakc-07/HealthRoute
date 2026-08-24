# HealthRoute — A Smart Emergency Healthcare Assistance System

A menu-driven C++ console application combining three modules:

- **Admin Module** — `BSTAdmin` (Binary Search Tree) for hospital record
  management (insert, search, update, delete, display), plus `MergeSort`
  for ranking hospitals by available ICU beds, plus `FileHandler` for
  saving/loading hospital data to `data/hospitals.txt`.
- **Routing Module** — `Graph` representing the Lalitpur road network as a
  weighted adjacency list, with **Dijkstra's Algorithm** (min-heap based)
  for shortest-distance calculation and parent-array **route
  reconstruction**.
- **Ritika Module** — user location selection, ambulance dispatch via a
  FIFO `Queue` (`DispatchCenter`), and emergency contact display.

## Project structure

```
HealthRoute/
├── include/            # headers
│   ├── Hospital.h
│   ├── BSTAdmin.h
│   ├── MergeSort.h
│   ├── Graph.h
│   ├── Ambulance.h
│   └── FileHandler.h
├── src/                 # implementations + entry point
│   ├── BSTAdmin.cpp
│   ├── MergeSort.cpp
│   ├── Graph.cpp
│   ├── Ambulance.cpp
│   ├── FileHandler.cpp
│   └── main.cpp
├── data/
│   └── hospitals.txt    # created/updated at runtime
├── Makefile
└── README.md
```

## Build & run

```bash
make
./healthroute
```

`make clean` removes object files and the binary.

## What's implemented

- [x] Project structure (folders/files)
- [x] Hospital struct
- [x] BST admin module (insert, search, display, update, delete)
- [x] Merge sort (hospitals ranked by ICU beds)
- [x] Merge sort connected to the menu
- [x] File handling (save/load hospital records)
- [x] Graph data structure (Lalitpur road network, weighted adjacency list)
- [x] Dijkstra's Algorithm (shortest distance)
- [x] Shortest route finding (parent array + reconstruction)
- [x] Hospital information tied to graph nodes
- [x] Nearest available-ICU hospital recommendation
- [x] User location selection (any of 9 junctions — not fixed to Pulchowk)
- [x] Ritika's flow: ambulance Queue dispatch, status update, emergency contacts
- [x] All three modules integrated behind one menu

## What's intentionally left for you

- [ ] **Raylib GUI integration** — this needs a windowed display to build,
  run, and visually verify, which isn't available in this environment.
  The console menu above is fully functional and structured so a GUI can
  call the same underlying classes (`BSTAdmin`, `Graph`, `DispatchCenter`)
  without changing their logic — only `main.cpp` would need a Raylib
  front end in place of the `std::cin`/`std::cout` menu loop.
- [ ] Final integration testing on your own machine/compiler, and any
  team-specific formatting or menu wording changes.

## Sample locations

Selectable current locations (Lalitpur junctions): Pulchowk, PatanDhoka,
Jawalakhel, Kumaripati, Sanepa, Kupondole, Lagankhel, Satdobato, Ekantakuna.

Seeded hospitals: Patan Hospital, Kumudini Memorial Hospital, Nepal
Mediciti — edit or replace via the Admin Panel, or edit
`seedDefaultHospitals()` in `src/main.cpp`.
