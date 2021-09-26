#include "Mandelbrot.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

PYBIND11_MODULE(Mandelbrot3, m) {
    m.attr("FLOAT") = PREC_FLOAT;
    m.attr("DOUBLE") = PREC_DOUBLE;
    m.attr("LONG_DOUBLE") = PREC_LONG_DOUBLE;
    py::class_<Mandelbrot>(m, "Mandelbrot")
        .def(py::init<int,long,double>(), py::arg("precision"), py::arg("max_it"), py::arg("bailout"))
        .def("search_v1",&Mandelbrot::search_v1, py::arg("search_depth"), py::arg("seed") = 0)
        .def("random_zoom_level",&Mandelbrot::random_zoom_level, py::arg("center"), py::arg("resolution"), py::arg("seed") = 0)
        .def("max_zoom_level",&Mandelbrot::random_zoom_level, py::arg("center"), py::arg("resolution"), py::arg("seed") = 0)
        .def("render_distance",&Mandelbrot::render_distance)
        .def("distance",&Mandelbrot::distance)
        .def("render_orbit",&Mandelbrot::render_orbit)
        .def("search_v2",&Mandelbrot::search_v2, py::arg("search_depth"), py::arg("seed") = 0)
        .def("search_v3",&Mandelbrot::search_v3);
        /*
        .def("render_escape_time",&Mandelbrot::render_escape_time)

        .def("eval_escape_time",&Mandelbrot::eval_escape_time);*/

}

/*
mb = Mandel.Mandelbrot(precision = Mandel.DOUBLE, max_it = 1000, bailout = 2.0)
p1 = mb.search_v1(search_depth = 200, seed = np.random.randint(0,10000))
zoom = mb.random_zoom_level(p1, seed = np.random.randint(0,10000))
img = mb.render_distance(p1, zoom, resolution = (1000,1000), circular = True)
dist = mb.eval_distance(p1) #eval just
it = mb.eval_escape_time(p1)
orbit = mb.eval_orbit(p1)
print(val)
*/
