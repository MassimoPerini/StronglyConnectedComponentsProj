# Coverage notes

## Tarjan

- It pushes on stack every vertex

## Others algorithms

- Not every vertex is pushed on stack.
- The higher the number of components, the less nodes on stack

## Additional notes

- You can see the number of components from coverage screenshots, looking how many times the number of components is increased (`num_sccs++` or `total_scc++`)
- run `./generate_coverage.sh` to generate the HTML reports
- debug build, gcov, lcov and gen_html are required

