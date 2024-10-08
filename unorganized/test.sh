#run the tests
#!/bin/bash
cd cmake-build-debug/
cmake ..
make
find . -name "*.gcda" -type f -delete
./unit-tests/Uno-tests
cd ..

