#ifndef EVAL_H
#define EVAL_H
#include <vector>
#include <math.h>
using namespace std;

template<typename FT>
FT eval_escape_time(
        FT re,
        FT im,
        const FT max_it,
        const FT bailout = 2.0){
    FT counter = 0;
    FT z_re = re;
    FT z_im = im;
    FT t1,t2;
    t1 = z_re * z_re;
    t2 = z_im * z_im;
    const FT sq_bailout = bailout * bailout;
    do{
        z_im = 2.0 * z_re * z_im + im; //removing the factor 2 here gives a penis which is very epic
        z_re = t1 - t2 + re;
        counter++;
        t1 = z_re * z_re;
        t2 = z_im * z_im;
    }while(((t1 + t2) < sq_bailout) && (counter < max_it));
    return counter;
}

//http://www.imajeenyus.com/mathematics/20121112_distance_estimates/distance_estimation_method_for_fractals.pdf
template<typename FT>
FT eval_distance(
        FT re,
        FT im,
        const FT max_it,
        const FT bailout = 2.0){

    FT z_re = re;
    FT z_im = im;
    FT dz_re = 1.0; //1 or 0????
    FT dz_im = 0.0;
    FT t1,t2,t3;
    t1 = z_re * z_re;
    t2 = z_im * z_im;
    FT counter = 0;
    const FT sq_bailout = bailout * bailout;
    do{
        //dz = 2.0 * z * dz + 1.0;
        t3 = 2.0 * (z_re * dz_re - z_im * dz_im) + 1.0;
        dz_im = 2.0 * (z_im * dz_re + z_re * dz_im);
        dz_re = t3;
        z_im = 2.0 * z_re * z_im + im;
        z_re = t1 - t2 + re;
        counter++;
        t1 = z_re * z_re;
        t2 = z_im * z_im;
    }while(((t1 + t2) < sq_bailout) && (counter < max_it));
    if(counter < max_it){
        t1 = sqrt(dz_re * dz_re + dz_im * dz_im);
        t2 = z_im * z_im + z_re * z_re;
        return log(t2) * sqrt(t2) / t1;
    }else{
        return 0;
    }
}

template<typename FT>
FT eval_smooth_escape_time(
        FT re,
        FT im,
        const FT max_it,
        const FT bailout = 2.0){
    FT ret = 0;
    FT z_re = re;
    FT z_im = im;
    FT t1,t2;
    const FT sq_bailout = bailout * bailout;
    do{
        t1 = z_re * z_re;
        t2 = z_im * z_im;
        z_im = 2.0 * z_re * z_im + im;
        z_re = t1 - t2 + re;
        ret++;
    }while(((t1 + t2) < sq_bailout) && (ret < max_it));
    if(ret < max_it){
        t1 = z_re * z_re;
        t2 = z_im * z_im;
        ret += 1.0 - log(log(t1 + t2) / log(4.0)) / log(2.0);
    }
    return ret;
}


template<typename FT>
tuple<FT,FT> eval_time_distance(
        FT re,
        FT im,
        const FT max_it,
        const FT bailout = 2.0){
    FT counter = 0;
    FT z_re = re;
    FT z_im = im;
    FT dz_re = 1.0;
    FT dz_im = 0.0;
    FT t1,t2,sqlen;
    const FT sq_bailout = bailout * bailout;
    do{
        t1 = z_re * z_re;
        t2 = z_im * z_im;
        //dz = 2.0 * z * dz + 1.0;
        dz_re = 2.0 * (z_re * dz_re - z_im * dz_im) + 1.0;
        dz_im = 2.0 * (z_im * dz_re + z_re * dz_im);

        //yooo
        z_im = 2.0 * z_re * z_im + im;
        z_re = t1 - t2 + re;
        counter++;
    }while(((t1 + t2) < sq_bailout) && (counter < max_it));
    if(counter < max_it){
        sqlen = z_re * z_re + z_im * z_im;
        t2 = log(sqlen);
        counter += 1.0 - log(t2 / log(4.0)) / log(2.0);
        t1 = sqrt(4.0 * (dz_re * dz_re + dz_im * dz_im));
        return make_tuple(counter,FT(sqrt(sqlen) * t2 / t1));
    }else{
        return make_tuple(FT(max_it),FT(0));
    }
}

template<typename FT>
vector<tuple<FT,FT>> eval_orbit(
        FT re,
        FT im,
        FT max_it,
        FT bailout = 2.0){
    vector<tuple<FT,FT>> ret;
    FT counter = 0;
    FT z_re = re;
    FT z_im = im;
    FT t1,t2;
    t1 = z_re * z_re;
    t2 = z_im * z_im;
    const FT sq_bailout = bailout * bailout;
    ret.push_back(make_tuple(z_re,z_im));
    do{
        z_im = 2.0 * z_re * z_im + im; //removing the factor 2 here gives a penis which is very epic
        z_re = t1 - t2 + re;
        counter++;
        t1 = z_re * z_re;
        t2 = z_im * z_im;
        ret.push_back(make_tuple(z_re,z_im));
    }while(((t1 + t2) < sq_bailout) && (counter < max_it));
    return ret;
}

#endif //EVAL_H

/*
to evaluate orbits with higher precision, we can compute a reference orbit in low precision, and use the differential thing to calculate
each step's deviation. but the deviation from a higher degree polynomial is not trivial i think? but like, can't we add the deviation of the last step?
we must do so intelligently or the deviation will be shid and fard.
so we base our calculation on 2 complex numbers:
last position of rough calculation
last delta of smooth calculation

well is there a discrete way of calculation?
there is, but
*/

