rm -rf build/*
cp test_inputs/* build/
for file in $(ls tests)
do
    echo "Building $file..."
    gcc -o build/$file.out tests/$file src/slowlight.c src/slt.c external/gfx.c -g -lm -lX11
    echo "Built."
done