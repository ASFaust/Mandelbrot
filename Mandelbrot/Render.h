#ifndef RENDER_H
#define RENDER_H

#include <thread>
#include <iostream>
#include <vector>
//#include "Eval.h"

namespace py = pybind11;
using namespace std;

//but how would i assign thread_fn based on
template<typename FT>
class Renderer{
    public:
        Renderer(){
            center = make_tuple<FT,FT>(0,0);
            zoom = 1;
            resolution = make_tuple<int,int>(400,400);
            max_it = 200;
            bailout = 2;
            n_threads = 5;
            circular = false;
        };
        tuple<FT,FT> center;
        FT zoom;
        tuple<int,int> resolution;
        FT max_it;
        FT bailout;
        int type;
        int n_threads;
        bool circular;
};

//depending on the type, the renderer instance invokes other rendering things.
//the combination of circular and the other argument is shit.

/*
template<typename FT>
py::array_t<float> render(
        tuple<FT,FT> center,
        FT zoom,
        tuple<int,int> resolution,
        FT max_it,
        FT bailout = 2.0,
        int type = 0,
        int n_threads = -1,
        bool circular = false
){
    //the renderer renders to a buffer
    //it's a bit arbitrary which parts i let the renderer hadle
    Renderer<FT> renderer(center,zoom,resolution,max_it,bailout,circular);

    switch(type){
        case TYPE_ITERATIONS:
            renderer.eval_fn = eval_escape_time<FT>;
        break;
        case TYPE_ITERATIONS_SMOOTH:
            renderer.eval_fn = eval_smooth_escape_time<FT>;
        break;
        case TYPE_DISTANCE:
            renderer.eval_fn = eval_distance<FT>;
        break;
        default:
            renderer.eval_fn = eval_escape_time<FT>;
            cerr << type << " is not a recognized type. falling back to basic escape time algorithm." << endl;
        break;
    }


    if(n_threads <= 0){
        n_threads = thread::hardware_concurrency();
    }
    thr.n_threads = n_threads;
    thr.center = center;
    thr.zoom =

    thr.spawn_workers();
    thr.spawn_printer();
    thr.join_all();

    //std::thread second (bar,0);

    //second.join();
    const auto [ res_x, res_y ] = resolution;

    auto _np_arr = py::array_t<float>({res_y,res_x,1});
    auto np_arr = _np_arr.mutable_unchecked<3>();

    //copy result into np array
    //normalize to pixel distance if TYPE_DISTANCE
    return _np_arr;
}

//two more arguments: mutex and counter.
template<typename FT>
void render_thread(ThreadData<FT> td){
    FT c_re,c_im,t1;
    for(int x = 0; x < td.res_x; x++){
        c_re = FT(x) / FT(td.res_x - 1.0) * 2.0 - 1.0;
        t1 = c_re * c_re;
        c_re = c_re / td.zoom + td.re;
        for(int y = td.y_offset; y < td.res_y; y += td.n_threads){
            c_im = FT(y) / FT(td.res_y - 1.0) * 2.0 - 1.0;
            if((c_im * c_im + t1) > 1.0){
                continue;
            }
            c_im = c_im / td.zoom + td.im;
            td.result[x][y] = td.eval_fn(c_re,c_im,td.max_it,td.bailout);
        }
    }
}

//wait wait
//the fukin shit is too long
//this really should be a struct
template<typename FT>
void render_thread_circular(ThreadData<FT> td){
    FT c_re,c_im,t1;
    for(int x = 0; x < td.res_x; x++){
        c_re = FT(x) / FT(td.res_x - 1.0) * 2.0 - 1.0;
        t1 = c_re * c_re;
        c_re = c_re / td.zoom + td.re;
        for(int y = td.y_offset; y < td.res_y; y += td.n_threads){
            c_im = FT(y) / FT(td.res_y - 1.0) * 2.0 - 1.0;
            if((c_im * c_im + t1) > 1.0){
                continue;
            }
            c_im = c_im / td.zoom + td.im;
            td.result[x][y] = td.eval_fn(c_re,c_im,td.max_it,td.bailout);
        }
    }
}

template<typename FT>
struct Thread
{
    Thread(vector<vector<FT> >& _result):result(_result){

    }
    FT im,re;
    FT zoom;
    int res_x, res_y;
    FT max_it;
    FT bailout;
    int n_threads;
    bool circular;
    FT (*eval_fn)(FT,FT,const FT,const FT);
    int y_offset;
    vector<vector<FT> >& result;
};

/*
template<typename FT>
py::object render_ghost(
        std::complex<FT> centers,
        long zoom,
        int res_x,
        int res_y,
        long max_it,
        int n_threads = -1,
        FT max_r = 2.0,
        circular = false
){

}
*/

#endif