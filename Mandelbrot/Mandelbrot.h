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

/*
mb = Mandel.Mandelbrot(precision = Mandel.DOUBLE, max_it = 1000, bailout = 2.0)
p1 = mb.search_v1(search_depth = 200, seed = np.random.randint(0,10000))
zoom = mb.random_zoom_level(p1, seed = np.random.randint(0,10000))
img = mb.render_distance(p1, zoom, resolution = (1000,1000), circular = True)
print(val)
*/

class Mandelbrot{
    public:
        Mandelbrot(int _precision, unsigned long _max_it, double _bailout);
        py::object search_v1(unsigned int search_depth, long seed);
        py::object search_v2(unsigned int search_depth, long seed);
        py::object search_v3(unsigned int search_depth, unsigned int outer_depth, long seed);


        py::object random_zoom_level(py::object center, tuple<unsigned int, unsigned int> resolution, long seed);
        py::object max_zoom_level(py::object center, tuple<unsigned int, unsigned int> resolution);
        py::array_t<float> render_distance(py::object center, py::object zoom_level, tuple<unsigned int, unsigned int> resolution);
        py::array_t<float> render_orbit(py::object center, tuple<unsigned int, unsigned int> resolution);
        py::object distance(py::object position);

    private:

        template<typename FT>
        tuple<FT,FT> _search_v1(unsigned int search_depth, long seed = 0);

        template<typename FT>
        tuple<FT,FT> _search_v2(unsigned int search_depth, long seed = 0);

        template<typename FT>
        tuple<FT,FT> _search_v3(unsigned int search_depth, unsigned int outer_depth, long seed = 0);

        template<typename FT>
        tuple<tuple<FT,FT>,tuple<FT,FT> > _init_search_v1();



        template<typename FT>
        FT _max_zoom_level(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        FT _random_zoom_level(tuple<FT,FT> center,tuple<unsigned int, unsigned int> resolution, long seed = 0);

        template<typename FT>
        py::array_t<float> _render_distance(tuple<FT,FT> center, FT zoom_level, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        py::array_t<float> _render_orbit(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution);

        template<typename FT>
        tuple<unsigned int,FT> _eval_orbit(vector<tuple<FT,FT>> orbit);

        int precision;
        unsigned long max_it;
        double bailout;
};

template<typename FT>
FT Mandelbrot::_max_zoom_level(tuple<FT,FT> center,tuple<unsigned int, unsigned int> resolution){
    //this function is actually quite shit to implement.
    //it depends on the precision of the FT and also on the center and the resolution
    //so we first determine a hard baseline from the FT and resolution
    //then we evaluate the distance to the closest point from the center
    //which gives us a second, FT-invariant upper bound for the shit
    //so right now we just use a fixed upper bound for dev purposes.
    FT max_level = exp(32);
    FT dist_level = 0.5 / eval_distance<FT>(get<0>(center),get<1>(center),max_it,bailout);
    //the distance est
    return min(max_level,dist_level);
}

template<typename FT>
FT Mandelbrot::_random_zoom_level(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution, long seed){
    FT max_zoom = _max_zoom_level(center,resolution);
    if(seed == 0){
        srand(time(NULL));
    }else{
        srand(seed);
    }
    //TODO: This can become a problem for arbitrary precision libraries
    FT ret_level = exp(frand(log(0.2),log(max_zoom)));
    return ret_level;
}

template<typename FT>
FT dist(tuple<FT,FT> v1, tuple<FT,FT> v2){
    FT d1 = get<0>(v1) - get<0>(v2);
    FT d2 = get<1>(v1) - get<1>(v2);
    return sqrt(d1*d1 + d2*d2);
}

template<typename FT>
tuple<FT,FT> Mandelbrot::_search_v1(unsigned int search_depth, long seed){
    if(seed == 0){
        srand(time(NULL));
    }else{
        srand(seed);
    }
    auto [ p1,p2 ] = _init_search_v1<FT>();
    auto [ re1, im1] = p1;
    auto [ re2, im2] = p2;
    FT re3;
    FT im3;
    for(unsigned int i = 0; i < search_depth; i++){
        re3 = (re1 + re2) / 2.0;
        im3 = (im1 + im2) / 2.0;
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
tuple<FT,FT> Mandelbrot::_search_v2(unsigned int search_depth, long seed){
    if(seed == 0){
        srand(time(NULL));
    }else{
        srand(seed);
    }
    //use distance estimator and random noise to get closer to the set
    FT re1 = 0;
    FT im1 = 0;
    unsigned long c1 = max_it;
    while(c1 == max_it){
        re1 = frand(-2,2);
        im1 = frand(-2,2);
        c1 = eval_escape_time<FT>(re1,im1,max_it,bailout);
    }
    FT dist = eval_distance<FT>(re1,im1,max_it,bailout);
    FT re2,im2;
    unsigned int best_k = 0;
    FT best_k_dist = 1000;
    for(int i = 0; i < search_depth; i++){
        do{
            re2 = frand(-dist,dist);
            im2 = frand(-dist,dist);
        }while((re2*re2 + im2*im2) >= (dist*dist));
        re2 += re1;
        im2 += im1;
        FT dist2 = eval_distance<FT>(re2,im2,max_it,bailout);
        auto [k,t2] = _eval_orbit<FT>(eval_orbit<FT>(re1,im1,max_it,bailout));
        if((dist2 > 0) && (k >= best_k) && (dist2 <= dist)){
            re1 = re2;
            im1 = im2;
            dist = dist2;
            best_k = k;
            best_k_dist = t2;
            cout << log(1.0 / dist) << " " << k << " " << t2 << endl;
        }
    }
    return make_tuple(re1,im1);
}

template<typename FT>
tuple<FT,FT> Mandelbrot::_search_v3(unsigned int search_depth, unsigned int outer_depth, long seed){
    unsigned int best_k = 0;
    FT best_k_dist = 10000000;
    FT best_re, best_im;
    if(seed == 0){
        srand(time(NULL));
    }else{
        srand(seed);
    }
    for(int i = 0; i < outer_depth; i++){
        long rval = rand();
        auto [re, im] = _search_v1<FT>(search_depth,rval);
        auto [k,k_dist] = _eval_orbit<FT>(eval_orbit<FT>(re,im,max_it,bailout));
        if((k >= best_k) && (best_k_dist >= k_dist)){
            best_k_dist = k_dist;
            best_k = k;
            best_re = re;
            best_im = im;
            cout << best_k << " " << best_k_dist << endl;
        }
    }
    return make_tuple(best_re,best_im);
}

template<typename FT>
tuple<unsigned int,FT> Mandelbrot::_eval_orbit(vector<tuple<FT,FT>> orbit){
    unsigned int min_dist_it = 0;
    FT min_dist = 1000;
    for(int i = 1; i < orbit.size() - 1; i++){
        for(int j = 0; j < orbit.size() - i; j++){
            int i1 = j;
            int i2 = j + i;
            FT c_dist = dist<FT>(orbit[i1],orbit[i2]);
            if(c_dist <= min_dist){
                min_dist = c_dist;
                min_dist_it = i;
            }
        }
    }
    return make_tuple(min_dist_it,min_dist);
}

template<typename FT>
tuple<tuple<FT,FT>,tuple<FT,FT> > Mandelbrot::_init_search_v1(){
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

//the return is a 2-dimensional numpy float array of distance in pixels.
//okok
//:)
//first implementation: non-threaded, simple estimation

template<typename FT>
py::array_t<float> Mandelbrot::_render_distance(tuple<FT,FT> center, FT zoom_level, tuple<unsigned int, unsigned int> resolution){

    FT zoom_radius = 0.5 / zoom_level;
    const unsigned int res_x = get<0>(resolution);
    const unsigned int res_y = get<1>(resolution);
    const float res = (float(res_x) + float(res_y) ) / 2.0;
    const FT px_size = zoom_radius / FT(res) * 2.0;

    auto ret_arr = py::array_t<float>({res_y,res_x});
    auto ret_mu = ret_arr.mutable_unchecked<2>();

    for(unsigned int x = 0; x < res_x; x++){
        FT re = (FT(x) - FT(res_x) / 2.0) / res;
        re *= zoom_radius;
        re += get<0>(center);
        for(unsigned int y = 0; y < res_y; y++){
            FT im = (FT(y) - FT(res_y) / 2.0) / res;
            im *= zoom_radius;
            im += get<1>(center);
            FT dist = eval_distance<FT>(re,im,max_it,bailout) / px_size;
            ret_mu(y,x) = float(dist);
        }
    }
    return ret_arr;
}

template<typename FT>
py::array_t<float> Mandelbrot::_render_orbit(tuple<FT,FT> center, tuple<unsigned int, unsigned int> resolution){
    const unsigned int res_x = get<0>(resolution);
    const unsigned int res_y = get<1>(resolution);

    auto ret_arr = py::array_t<float>({res_y,res_x});
    auto ret_mu = ret_arr.mutable_unchecked<2>();

    auto orbit = eval_orbit<FT>(get<0>(center),get<1>(center),max_it,bailout);

    for(unsigned int x = 0; x < res_x; x++){
        for(unsigned int y = 0; y < res_y; y++){
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

/*
FT eval_distance(
        FT re,
        FT im,
        const FT max_it,
        const FT bailout = 2.0){
}*/




#endif