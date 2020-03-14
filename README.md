# slowlight
A C-based raycaster renderer in which every ray is not instantly calculated, but rather moves perceptibly slowly, thus making it easier to visualize interesting near-lightspeed geometrical distortions.

## Building
For building the examples, run buildtests.sh on linux environments or

`gcc -o build/$file.out tests/<FILENAME> src/slowlight.c src/slt.c external/gfx.c -g -lm -lX11`

for each example you want to build on other environments (though this was only tested on linux).

## Maths
This project uses basic vector operations. If you want to understand them better, I have attached a GeoGebra 3D file at the docs folder with which you can play around to get a more intuitive notion of what is going on ([Triangle_Subspace_Collision(1).ggb](./docs/Triangle_Subspace_Collision(1).ggb)).

There are wikipedia links at the maths-heavy parts of the code.

## History
Once I was bored and decided to spin around in my swivel chair. After stopping, I thought something like "Hey, were light to be slower, the background would keep spinning for a little longer, as it's farther away".

I decided to make a little raycaster to test whether that would really happen. I quickly figured this method I used is definitely not optimal, but as I had already spent a whole day making the raycaster, I decided to keep it.

One day and lots of workarounds later I could prove that yes, the background would keep spinning, and the closer objects would start spinning first when starting the rotation.

## Disclaimer
I don't guarantee the extraction of any kind of scientific data or meaning from the results of this project (or anything, really. Please refer to the [MIT License](./LICENCE) under which this project is distributed).

This project was built and is maintained by a single person, so there are probably bugs aplenty. If you have any suggestion, please open an Issue or e-mail me.

I hope this project brought you some interesting ideas or just a fun pastime.

^_^

\- Amélia