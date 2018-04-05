//
// Created by gmullen on 05/04/2018.
//
#include "Measurement.h"
ostream & operator<<(ostream& os, const Measurement& M){
    os << "x = "<<M.getX()<< ", y = "<<M.getY()<< ", measurement error = "<<M.getmErr()<< ", standard error = "<<M.getsErr()<< ", combined error = "<<M.getcErr()<<endl;
    return os;
}

//because memebr variables const can't input measurements
istream & operator>>(istream& is, Measurement& M){
    cout<<"Please enter your data in the following way: x, y, measruement error, systematic error."<<endl;
    double dataValues[4];
    for(int i{0}; i < 4; i++)
        is>>dataValues[i];
    Measurement* p = new Measurement(dataValues[0], dataValues[1], dataValues[2], dataValues[3]);
    shared_ptr<Measurement> sp0(p);
    cout << *sp0 <<endl;

    return is;
}

//Parse Lines readin function:
shared_ptr <Measurement> parse(const string& dataLine){
    double dataValues[4];
    string dataPoint;
    stringstream dataValuesline(dataLine);
    for(int i{0}; i < 4; i++){
        getline(dataValuesline,dataPoint,',');
        dataValues[i] = stod(dataPoint);
    }
    Measurement* p = new Measurement(dataValues[0], dataValues[1], dataValues[2], dataValues[3]);
    PtrMeasurement sp0(p);
    return sp0;
}

