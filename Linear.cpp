#include "Experiment.h"
RegStats Linear::Summation(){
    Measurement M(0,0,0,0); //doesn't work as R need identified as a measurement class
    RegStats total(M);
    //ResultsSet v = getDataVec();
    for_each( results.begin(), results.end(), [& total] (Measurement aMeasurement){
        total + aMeasurement;
    } );
    return total;
}

Linear::regVariable Linear::regressionCalc(RegStats& R)const{
    regVariable variables;
    variables.grad = (R.XY_invSqrErr*R.invSqrErr - R.Y_invSqrErr*R.X_invSqrErr)/(R.X_invErr_sqrd*R.invSqrErr - pow(R.X_invSqrErr,2));
    variables.intcpt = (R.X_invErr_sqrd*R.Y_invSqrErr - R.XY_invSqrErr*R.X_invSqrErr)/(R.X_invErr_sqrd*R.invSqrErr - pow(R.X_invSqrErr,2));
    variables.gradErr = sqrt(R.invSqrErr/(R.X_invErr_sqrd*R.invSqrErr - pow(R.X_invSqrErr,2)));
    variables.intcptErr = sqrt(R.X_invErr_sqrd/(R.X_invErr_sqrd*R.invSqrErr - pow(R.X_invSqrErr,2)));
    return variables;
}

double Linear::ChiSqr_Calc(regVariable& V, ResultsSet results)const{
    double ChiSqr{0};
    for_each( results.begin(), results.end(), [&ChiSqr, V] (Measurement R){
        double delta {0};
        delta = R.getY() - (V.grad * R.getX()) - V.intcpt;
        ChiSqr +=  pow(delta/R.getcErr(), 2);
    });
    return ChiSqr;
}

//Defined Overloads:
RegStats & RegStats::operator+(Measurement& M){
    RegStats aReading(M);
    X_invSqrErr += aReading.X_invSqrErr;
    invSqrErr += aReading.invSqrErr;
    Y_invSqrErr += aReading.Y_invSqrErr;
    X_invErr_sqrd += aReading.X_invErr_sqrd;
    XY_invSqrErr += aReading.XY_invSqrErr;
    Y_invErr_sqrd += aReading.Y_invErr_sqrd;
    return *this;
}

RegStats::RegStats (Measurement& M){
    X_invSqrErr = M.getX()/(pow(M.getcErr(),2));
    invSqrErr = pow(M.getcErr(),2);
    Y_invSqrErr = M.getY()/(pow(M.getcErr(),2));
    X_invErr_sqrd = pow(M.getX(),2) / pow(M.getcErr(),2);
    XY_invSqrErr = ( M.getX()*M.getY() )/(pow(M.getcErr(),2));
    Y_invErr_sqrd = pow(M.getY(),2) / pow(M.getcErr(),2);
}

