#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <thread>
#include <iostream>
#include <vector>
#include "Mandelbrot.h"

namespace py = pybind11;
using namespace std;

Mandelbrot::Mandelbrot(int _precision, unsigned long _max_it, double _bailout)
{
    precision = _precision;
    max_it = _max_it;
    bailout = _bailout;
}

py::object Mandelbrot::search_v1(unsigned int search_depth, long seed){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_search_v1<float>(search_depth,seed));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_search_v1<double>(search_depth,seed));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_search_v1<long double>(search_depth,seed));
        break;
    }
    return ret;
}

py::object Mandelbrot::search_v2(unsigned int search_depth, long seed){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_search_v2<float>(search_depth,seed));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_search_v2<double>(search_depth,seed));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_search_v2<long double>(search_depth,seed));
        break;
    }
    return ret;
}

py::object Mandelbrot::search_v3(unsigned int search_depth, unsigned int outer_depth, long seed){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_search_v3<float>(search_depth,outer_depth,seed));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_search_v3<double>(search_depth,outer_depth,seed));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_search_v3<long double>(search_depth,outer_depth,seed));
        break;
    }
    return ret;
}

py::object Mandelbrot::max_zoom_level(py::object center, tuple<unsigned int, unsigned int> resolution){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_max_zoom_level<float>(center.cast<tuple<float,float>>(),resolution));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_max_zoom_level<double>(center.cast<tuple<double,double>>(),resolution));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_max_zoom_level<long double>(center.cast<tuple<long double,long double>>(),resolution));
        break;
    }
    return ret;
}

py::object Mandelbrot::random_zoom_level(py::object center, tuple<unsigned int, unsigned int> resolution, long seed){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_random_zoom_level<float>(center.cast<tuple<float,float>>(),resolution,seed));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_random_zoom_level<double>(center.cast<tuple<double,double>>(),resolution,seed));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_random_zoom_level<long double>(center.cast<tuple<long double,long double>>(),resolution,seed));
        break;
    }
    return ret;
}


py::array_t<float> Mandelbrot::render_distance(py::object center, py::object zoom_level, tuple<unsigned int, unsigned int> resolution){
    switch(precision){
        case PREC_FLOAT:
            return _render_distance<float>(
                center.cast<tuple<float,float>>(),
                zoom_level.cast<float>(),
                resolution);

        break;
        case PREC_DOUBLE:
            return _render_distance<double>(
                center.cast<tuple<double,double>>(),
                zoom_level.cast<double>(),
                resolution);
        break;
        case PREC_LONG_DOUBLE:
            return _render_distance<long double>(
                center.cast<tuple<long double,long double>>(),
                zoom_level.cast<long double>(),
                resolution);
        break;
    }
}

py::array_t<float> Mandelbrot::render_orbit(py::object center, tuple<unsigned int, unsigned int> resolution){
    switch(precision){
        case PREC_FLOAT:
            return _render_orbit<float>(
                center.cast<tuple<float,float>>(),
                resolution);
        break;
        case PREC_DOUBLE:
            return _render_orbit<double>(
                center.cast<tuple<double,double>>(),
                resolution);
        break;
        case PREC_LONG_DOUBLE:
            return _render_orbit<long double>(
                center.cast<tuple<long double,long double>>(),
                resolution);
        break;
    }
}

py::object Mandelbrot::distance(py::object position){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(
                eval_distance<float>(
                    get<0>(position.cast<tuple<float,float>>()),
                    get<1>(position.cast<tuple<float,float>>()),
                    max_it,
                    bailout)
                );
        break;
        case PREC_DOUBLE:
            ret = py::cast(
                eval_distance<double>(
                    get<0>(position.cast<tuple<double,double>>()),
                    get<1>(position.cast<tuple<double,double>>()),
                    max_it,
                    bailout)
                );
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(
                eval_distance<long double>(
                    get<0>(position.cast<tuple<long double,long double>>()),
                    get<1>(position.cast<tuple<long double,long double>>()),
                    max_it,
                    bailout)
                );
        break;
    }
    return ret;
}