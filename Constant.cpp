//
//
#include "Experiment.h"
#include "Measurement.h"


//Calculate the weighted mean/SD:
Constant::WeightStats Constant::WeightedCalc(){
    Measurement M(0,0,0,0); //doesn't work as M need to be mutable!
    WeightStats total(M);
    for_each( results.begin(), results.end(), [&total] (Measurement R){
        total + R;
    } );
    return total;
}

Constant::WeightStats::WeightStats(Measurement& M){
    Y_InvsqrErr = M.getY()/(pow(M.getcErr(),2));
    InvsqrErr = pow(M.getcErr(),2);
}

//overload operator:
Constant::WeightStats & Constant::WeightStats::operator+(Measurement& M){
    WeightStats other(M);
    InvsqrErr += other.InvsqrErr;
    Y_InvsqrErr += other.Y_InvsqrErr;
    return *this;
}

