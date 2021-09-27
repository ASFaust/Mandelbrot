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
    n_threads = thread::hardware_concurrency();
}

py::object Mandelbrot::search(unsigned int search_depth, double bias, long seed){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_search<float>(search_depth,bias,seed));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_search<double>(search_depth,bias,seed));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_search<long double>(search_depth,bias,seed));
        break;
    }
    return ret;
}

py::object Mandelbrot::min_radius(py::object center, tuple<unsigned int, unsigned int> resolution){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_min_radius<float>(center.cast<tuple<float,float>>(),resolution));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_min_radius<double>(center.cast<tuple<double,double>>(),resolution));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_min_radius<long double>(center.cast<tuple<long double,long double>>(),resolution));
        break;
    }
    return ret;
}

py::object Mandelbrot::random_radius(py::object center, tuple<unsigned int, unsigned int> resolution, long seed){
    py::object ret;
    switch(precision){
        case PREC_FLOAT:
            ret = py::cast(_random_radius<float>(center.cast<tuple<float,float>>(),resolution,seed));
        break;
        case PREC_DOUBLE:
            ret = py::cast(_random_radius<double>(center.cast<tuple<double,double>>(),resolution,seed));
        break;
        case PREC_LONG_DOUBLE:
            ret = py::cast(_random_radius<long double>(center.cast<tuple<long double,long double>>(),resolution,seed));
        break;
    }
    return ret;
}


py::array_t<float>Mandelbrot::render(
        py::object center,
        py::object radius,
        tuple<unsigned int, unsigned int> resolution,
        int type)
    {
    switch(type){
        case RENDER_D:
            switch(precision){
                case PREC_FLOAT:
                    return _render_distance<float>(
                        center.cast<tuple<float,float>>(),
                        radius.cast<float>(),
                        resolution);
                break;
                case PREC_DOUBLE:
                    return _render_distance<double>(
                        center.cast<tuple<double,double>>(),
                        radius.cast<double>(),
                        resolution);
                break;
                case PREC_LONG_DOUBLE:
                    return _render_distance<long double>(
                        center.cast<tuple<long double,long double>>(),
                        radius.cast<long double>(),
                        resolution);
                break;
            }
        break;/*
        case RENDER_TD:
            switch(precision){
                case PREC_FLOAT:
                    return _render_time_distance<float>(
                        center.cast<tuple<float,float>>(),
                        radius.cast<float>(),
                        resolution);
                break;
                case PREC_DOUBLE:
                    return _render_time_distance<double>(
                        center.cast<tuple<double,double>>(),
                        radius.cast<double>(),
                        resolution);
                break;
                case PREC_LONG_DOUBLE:
                    return _render_time_distance<long double>(
                        center.cast<tuple<long double,long double>>(),
                        radius.cast<long double>(),
                        resolution);
                break;
            }
        break;*/
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
