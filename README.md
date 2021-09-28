# Automatic art generation with the Mandelbrot set
![2462881304](./results/2462881304.jpg)

The mandelbrot set is a mathematical set of complex numbers with a fractal boundary, whose shape varies infinitely depending on location and zoom level. Choosing these parameters at random generates a unique and unpredictable work of art. Additionally, choosing a random color scheme creates another dimension of uniqueness for each piece.

The mandelbrot set is one of the most popular fractals and has been studied extensively in the field of complex dynamics. For generations, it attracted the attention of mathematicians and artists alike, with its ever changing, psychedelic and swirling patterns.

By creating NFTs based on this set, we honor the legacy of all minds that have occupied themselves with this complex mathematical object.

## Installation & Running
From the directory containing this file, execute:

```
pip3 install setuptools
pip3 install pybind11
sudo apt install python3-opencv
pip3 install Mandelbrot/
```
The script `run.py` can then be executed to generate 400 new images, which will be put into the `results` folder.