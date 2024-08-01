# RecDeltri: Reconstruction Library based on Delaunay Triangulation and Graph Cuts
implementation of ["Efficient Multi-View Reconstruction of Large-Scale Scenes
using Interest Points, Delaunay Triangulation and Graph Cuts"](https://www.di.ens.fr/willow/pdfs/07iccv_a.pdf) in C++.

The algorithm is implemented in C++, and python bindings are available through pybind11.

## Dependencies
- libcgal-dev
- libgmp3-dev
(Dependencies is complicated, so I decided to use docker to build the project soon)

## Build and Run
build:
```bash
mkdir build
cd build
cmake ..
make
```
run example of hull generation and skeletonization:
```bash
python3 example.py ./resources/20_module_test.json
```
run example of c++, which is now under development:
```bash
./concavehull_example
```