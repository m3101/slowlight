rm -rf build/*
cp test_inputs/* build/
for file in $(ls tests)
do
    echo "Building $file..."
    gcc -o build/$file.out tests/$file src/slowlight.c -g -lm
    echo "Built."
done