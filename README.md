# StronglyConnectedComponentsProj
Strongly connected components project 

- Andrea Corsini : [andrea.corsini@mail.polimi.it](andrea.corsini@mail.polimi.it)
- Massimo Perini : [massimo.perini@mail.polimi.it](massimo.perini@mail.polimi.it)

[![Build Status](https://travis-ci.com/massimoPerini/StronglyConnectedComponentsProj.svg?token=2zyVh8xUmBzVfC1pCxp9&branch=master)](https://travis-ci.com/massimoPerini/StronglyConnectedComponentsProj)

## Project structure

The project contains two modules:
  - `./libsccalgorithms` : the library containing several generic implementations to compute SCCs.
  - `./app` : program that generates random graphs with different dimensions and compare the implementations given by the library.
  The main program will then write reports about timing and memory on `.csv` files. See Running the app section.
  
We use `cmake` to generate build systems.

We use [Hunter](https://www.github.com/ruslo/hunter) to manage dependencies such us BGL and GTest.

## Build the source

First generate out-of-source the build system:
``` sh
cmake -H. -B_builds/release -DCMAKE_BUILD_TYPE=Release
```

Then build the project:
```sh
cmake --build _builds
```

### Tests

`libsccalgorithms` comes with some tests, in order to enable them, you need to generate the build with test support enabled.
So generate the build system adding `-DSCC_ALGORITHM_BUILD_TEST=ON` (by default `OFF`) e.g.:

``` sh
cmake -H. -B_builds/lib-tests -DSCC_ALGORITHM_BUILD_TEST=ON
cmake --build _builds/lib-tests
./_builds/lib-tests/libsccalgorithms/SCCAlgorithms_test
```

### Coverage

Coverage reports are available for the main `app` and for the library tests. 
In any case, coverage reports require the build to be in debug mode, 
adding `-DCMAKE_BUILD_TYPE=Debug` in the generation step.

As for tests, you need to enable covereage for the build.
`gcov`, `lcov` and `genhtml` are required to generate HTML reports.

#### Library tests coverage

For the library test coverage you need to enable tests as explained in previous paragraph and also enables 
the coverage reports by adding `-DSCC_ALGORITHMS_COVERAGE=ON` (`OFF` by default).

``` sh
cmake -H. -B_builds/lib-tests-coverage -DSCC_ALGORITHM_BUILD_TEST=ON -DSCC_ALGORITHMS_COVERAGE=ON
```

Therefore will be added the new target `SCCAlgorithms_test_coverage` that will create reports in `./_builds/lib-tests-coverage/test_coverage` directory, 
by running:

``` sh
cmake --build _builds/lib-tests-coverage --target SCCAlgorithms_test_coverage
```

#### App coverage

Coverage on main app is helpful to observe how many times SCC algorithms pushes nodes on stacks.

For `app` coverage you still need to enable the coverage support on `libsccalgorithms`, 
by adding `-DSCC_ALGORITHMS_COVERAGE=ON`, and also on the main program as well with `-DSCC_APP_COVERAGE=ON` (`OFF` by default):

``` sh
cmake -H. -B_builds/app-coverage -DSCC_APP_COVERAGE=ON -DSCC_ALGORITHMS_COVERAGE=ON
```

Moreover you can customise the parameters on which run the coverage by modifying the cmake variable `SCC_APP_COVERAGE_ARGV` (See Running the app section for parameters).

Finally run the new target `SCCApp_coverage` and you will find the reports in `./_builds/app-coverage/coverage` directory:

``` sh
cmake --build _builds/app-coverage --target SCCApp_coverage
```

## Running the app

There are two modes: _timing_ analysis and _memory_ analysis

### Time analysis

Main application will randomly generates as many graph as required and on each of those run the scc algorithms.

Once you are in your target output directory, you can run a comparison with:

```sh
./SCCApp <MAX_DENSITY> <MAX_V> <MIN_DENSITY> <MIN_V> <OFFSET_DENSITY> <OFFSET_V>
```
Where:
  - `<MAX_DENSITY>` : The maximum density value used to populate graph edges
  - `<MAX_V>` : The maximum number of vertices to stop in generating graphs
  - `<MIN_DENSITY>` : The minimum density value used to populate graph edges
  - `<MIN_V>` : The minimum number of vertices to start generating the graphs.
  - `<OFFSET_DENSITY>` : The increment of density between one generated graph and next one.
  - `<OFFSET_V>` : The increment between one generated graph and next one.

For example the comand:

```sh
./SCCApp 2 100 2 0.01 0.51 0.01
```

Will run the scc algorithms on several random graphs, generated with 2 to 100 vertices (scattering by 2 units), with edges probability from 0.01 to 0.51 (increasing by 0.01).

Moreover you can specify the algorithms to compare. Checks `./SCCApp --help` or `-h` for the available list.
To choose them, just pass the same names given by the program help to the `--algorithm` parameter, e.g.:

``` sh
./SCCapp --algorithm boost tarjan nuutila1 --gengraph 2 100 2 0.01 0.3 0.02
```

- **!**  Use `--gengraph` parameter to specify the previous numeric parameters.
- **!!** If no algorithm is specified, then the program runs on all available ones.

### Memory analysis

This mode runs a given algorithm on a random graph, 
outputing memory usage (in bytes) before running the algorithm and the memory peak reached by its execution.

Once you are in your target output directory, you can run with:

```sh
./SCCApp --mem-analysis  <ALGORITHM_NAME> <DENSITY_EDGE> <V> [<OUTPUT_FILENAME>]
```
Where:
  - `<ALGORITHM_NAME>` : The name of choosen algorithm to run memory analysis on (see above available ones).
  - `<DENSITY_EDGE>` : The density value used to populate graph edges.
  - `<V>` : The number of verteces to generating the graph with.
  - `[<OUTPUT_FILENAME>]` : The file name where to append the result (optional).

For example the comand:

```sh
./SCCApp --mem-analysis tarjan 3000 0.07 mem_report.csv
```

runs Tarjan algorithm on one random graphs, generated with 3000 vertices and edge density 7% and the record of results are appended in the given file.
