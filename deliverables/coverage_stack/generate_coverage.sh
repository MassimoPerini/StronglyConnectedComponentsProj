git clone git@github.com:massimoPerini/StronglyConnectedComponentsProj.git sccp;
cd sccp;
git checkout project-structure;

cmake -H. -B_builds/app-coverage -DCMAKE_BUILD_TYPE=Debug -DSCC_APP_COVERAGE=ON -DSCC_ALGORITHMS_COVERAGE=ON -DSCC_APP_COVERAGE_ARGV="100 100 3 0.01 0.04 1.0" # or initial density 0.03 
cmake --build _builds/app-coverage --target SCCApp_coverage
rm -rf ../coverage
mv ./_builds/app-coverage/coverage ../.
cd ..
