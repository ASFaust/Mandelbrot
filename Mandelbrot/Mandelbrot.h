#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <thread>
#include <iostream>
#include <vector>
#include <math.h>
#include "utils.h"
#include "Eval.h"

namespace py = pybind11;
using namespace std;

#define PREC_FLOAT 0
#define PREC_DOUBLE 1
#define PREC_LONG_DOUBLE 2

#define RENDER_D 0
#define RENDER_TD 1

class Mandelbrot{
    public:
        Mandelbrot(int _precision, unsigned long _max_it, double _bailout);
        py::object search(unsigned int search_depth, double bias, long seed);

        py::object random_radius(py::object center, tuple<unsigned int, unsigned int> resolution, long seed);
        py::object min_radius(py::object center, tuple<unsigned int, unsigned int> resolution);
        py::array_t<float>render(py::object center, py::object radius, tuple<unsigned int, unsigned int> resolution, int type);
        py::array_t<float>render_orbit(py::object center, tuple<unsigned int, unsigned int> resolution);

        py::object distance(py::object position);

    private:

        template<typename FT>
        tuple<FT,FT> _search(unsigned int search_depth, FT bias = 0.5, long seed = 0);

        template<typename FT>
        tuple<tuple<FT,FT>,tuple<FT,FT> > _init_search();

        template<typename FT>
        FT _min_radius(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        FT _random_radius(tuple<FT,FT> center,tuple<unsigned int, unsigned int> resolution, long seed = 0);

        template<typename FT>
        py::array_t<float>_render_distance(tuple<FT,FT> center, FT radius, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        py::array_t<float>_render_time_distance(tuple<FT,FT> center, FT radius, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        py::array_t<float>_render_orbit(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        void render_dist_thread(int id,py::array_t<float> ret,tuple<FT,FT> center,FT radius,tuple<unsigned int, unsigned int> resolution);

        int precision;
        int n_threads;
        unsigned long max_it;
        double bailout;
};

template<typename FT>
FT Mandelbrot::_min_radius(tuple<FT,FT> center,tuple<unsigned int, unsigned int> resolution){
    FT dist = eval_distance<FT>(get<0>(center),get<1>(center),max_it,bailout) * 2.0;
    return max(FT(exp(FT(-32.0))),dist);
}

template<typename FT>
FT Mandelbrot::_random_radius(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution, long seed){
    FT min_r = _min_radius(center,resolution);
    if(seed == 0){
        srand(time(NULL));
    }else{
        srand(seed);
    }
    //TODO: This can become a problem for arbitrary precision libraries
    //FT ret_level = exp(frand(log(0.2),log(max_zoom)));
    return exp(frand(log(min_r),log(0.01)));
}

template<typename FT>
FT tuple_dist(tuple<FT,FT> v1, tuple<FT,FT> v2){
    FT d1 = get<0>(v1) - get<0>(v2);
    FT d2 = get<1>(v1) - get<1>(v2);
    return sqrt(d1*d1 + d2*d2);
}

template<typename FT>
tuple<FT,FT> Mandelbrot::_search(unsigned int search_depth, FT bias, long seed){
    if(seed == 0){
        srand(time(NULL));
    }else{
        srand(seed);
    }
    if((bias >= 1.0) or (bias <= 0.0)){
        throw "bias needs to be in (1,0)";
    }
    auto [ p1,p2 ] = _init_search<FT>();
    auto [ re1, im1] = p1;
    auto [ re2, im2] = p2;
    FT re3;
    FT im3;
    for(unsigned int i = 0; i < search_depth; i++){
        re3 = (re1 * bias + (re2 * (1.0 - bias)));
        im3 = (im1 * bias + (im2 * (1.0 - bias)));

        unsigned long c3 = eval_escape_time<FT>(re3,im3,max_it,bailout);
        if(c3 == max_it){
            re1 = re3;
            im1 = im3;
        }else{
            re2 = re3;
            im2 = im3;
            if(c3 == (max_it - 1)){
                break;
            }
        }
    }
    return make_tuple(re2,im2);
}

template<typename FT>
tuple<tuple<FT,FT>,tuple<FT,FT> > Mandelbrot::_init_search(){
    FT re1 = frand(-2,2);
    FT im1 = frand(-2,2);
    FT re2 = frand(-2,2);
    FT im2 = frand(-2,2);
    unsigned long c1 = eval_escape_time<FT>(re1,im1,max_it,bailout);
    unsigned long c2 = eval_escape_time<FT>(re2,im2,max_it,bailout);
    if(c1 == max_it){
        while(c2 == max_it){
            re2 = frand(-2,2);
            im2 = frand(-2,2);
            c2 = eval_escape_time<FT>(re2,im2,max_it,bailout);
        }
        return make_tuple(make_tuple(re1,im1),make_tuple(re2,im2));
    }else{
        while(c2 != max_it){
            re2 = frand(-2,2);
            im2 = frand(-2,2);
            c2 = eval_escape_time<FT>(re2,im2,max_it,bailout);
        }
        return make_tuple(make_tuple(re2,im2),make_tuple(re1,im1));
    }
}

template<typename FT>
py::array_t<float> Mandelbrot::_render_distance(
        tuple<FT,FT> center,
        FT radius,
        tuple<unsigned int, unsigned int> resolution){
    const unsigned int res_x = get<0>(resolution);
    const unsigned int res_y = get<1>(resolution);
    auto ret_arr = py::array_t<float>({res_y,res_x});
    vector<thread*> threads;
    for(int i = 0; i < n_threads; i++){
        threads.push_back(new thread(
            [this,i,ret_arr,center,radius,resolution] {render_dist_thread<FT>(i,ret_arr,center,radius,resolution);}
        ));
    }
    for(int i = 0; i < n_threads; i++){
        threads[i]->join();
        delete threads[i];
    }
    return ret_arr;
}

template<typename FT>
void Mandelbrot::render_dist_thread(
        int id,
        py::array_t<float> ret,
        tuple<FT,FT> center,
        FT radius,
        tuple<unsigned int, unsigned int> resolution){
    auto arr = ret.mutable_unchecked<2>();
    const unsigned int res_x = get<0>(resolution);
    const unsigned int res_y = get<1>(resolution);
    const float res = min(res_x,res_y);
    const FT px_size = radius * 2.0 / FT(res);
    for(int y = id; y < res_y; y += n_threads){
        FT im = (FT(y) - FT(res_y) / 2.0) / res; //now in the range of -1,1
        im *= radius;
        im += get<1>(center);
        for(int x = 0; x < res_x; x++){
            FT re = (FT(x) - FT(res_x) / 2.0) / res; //now in the range of -1,1
            re *= radius;
            re += get<0>(center);
            FT dist = eval_distance<FT>(re,im,max_it,bailout) / px_size;
            arr(y,x) = float(dist);
        }
    }
}

template<typename FT>
py::array_t<float> Mandelbrot::_render_orbit(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution){
    const unsigned int res_x = get<0>(resolution);
    const unsigned int res_y = get<1>(resolution);

    auto ret_arr = py::array_t<double>({res_y,res_x});
    auto ret_mu = ret_arr.mutable_unchecked<2>();

    auto orbit = eval_orbit<FT>(get<0>(center),get<1>(center),max_it,bailout);

    for(unsigned int y = 0; y < res_y; y++){
        for(unsigned int x = 0; x < res_x; x++){
            ret_mu(y,x) = 0;
        }
    }
    for(auto& c : orbit){
        //normalize the range (-2,2) to (0,1)
        int x = (get<0>(c) + 2.0) / 4.0 * res_x;//now relative to the center
        int y = (get<1>(c) + 2.0) / 4.0 * res_y;
        if((x < 0) || (x >= res_x) || (y < 0) || (y >= res_y)){
            continue;
        }
        ret_mu(int(y),int(x)) += 1.0;
    }
    return ret_arr;
}

#endif