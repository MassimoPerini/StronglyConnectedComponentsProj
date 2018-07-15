# StronglyConnectedComponentsProj
Strongly connected components

[![Build Status](https://travis-ci.com/massimoPerini/StronglyConnectedComponentsProj.svg?token=2zyVh8xUmBzVfC1pCxp9&branch=project-structure)](https://travis-ci.com/massimoPerini/StronglyConnectedComponentsProj)

## Build the source

TODO

## Generate the reports

Once you are in your target output directory, you can run a comparison with:

```sh
./SCCApp <MAX_DENSITY> <MAX_V> <MIN_DENSITY> <MIN_V> <OFFSET_DENSITY> <OFFSET_V>
```
Where:
  - `<MAX_DENSITY>` : The maximum density value used to populate graph edges
  - `<MAX_V>` : The maximum number of verteces to stop in generating graphs
  - `<MIN_DENSITY>` : The minimum density value used to populate graph edges
  - `<MIN_V>` : The minimum number of verteces to start generating the graphs.
  - `<OFFSET_DENSITY>` : The increment of density between one generated graph and next one.
  - `<OFFSET_V>` : The increment between one generated graph and next one.

For example the comand:

```sh
./SCCApp 2 100 2 0.01 0.51 0.01
```

Will run the scc algorithms on several random graphs, generated with 2 to 100 verteces (scattering by 2 units), with edges probability from 0.01 to 0.51 (increasing by 0.01)
>>>>>>> Reports run instructions
