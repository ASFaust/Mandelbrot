# Mandelbrot renderer
![2462881304](./results/2462881304.jpg)

The mandelbrot set is a mathematical set of complex numbers with a fractal boundary, whose shape varies infinitely depending on location and zoom level. Choosing these parameters at random generates a unique and unpredictable work of art. Additionally, choosing a random color scheme creates another dimension of uniqueness for each piece.

The mandelbrot set is one of the most popular fractals and has been studied extensively in the field of complex dynamics. For generations, it attracted the attention of mathematicians and artists alike, with its ever changing, psychedelic and swirling patterns.

## Installation & Running
From the directory containing this file, execute:

```
pip3 install setuptools
pip3 install pybind11
sudo apt install python3-opencv
pip3 install Mandelbrot/
```
The script `run.py` can then be executed to generate 400 new images, which will be put into the `results` folder. Have a look at the script to see how to change resolution and how to run the algorithm for certain seeds.

## Gallery

[Here](results/gallery.md) is a gallery of 400 generated results with their respective seeds, which can be reproduced at arbitrary resolutions.
