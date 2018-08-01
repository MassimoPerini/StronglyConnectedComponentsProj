# get project
git clone git@github.com:massimoPerini/StronglyConnectedComponentsProj.git sccp;
cd sccp;
git checkout project-structure;

# build release
cmake -H. -B_builds/release -DCMAKE_BUILD_TYPE=Realese 
cmake --build _builds/release
rm ../SCCApp
cp ./_builds/release/app/SCCApp ../.
cd ..

# generate report
./SCCApp 10 1000 2 0.01 0.3 0.02
