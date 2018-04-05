//
// Created by gmullen on 05/04/2018.
//

#ifndef PROJECT_MEASUREMENT_H
#define PROJECT_MEASUREMENT_H
#include<map>
#include<iterator>
#include<algorithm>
#include<cmath>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include <memory>


using namespace std;
class  Measurement{
    //Friends:
    friend ostream & operator<<(ostream& os, const Measurement& M);
    friend istream & operator>>(istream& is, Measurement& M);

private:
    const double xValue, yValue, mError;
    double sError, cError; // conbined error of sys and meas error

public:
    //Measurement(){}
    ~Measurement(){}

    //parameterised with systematic
    Measurement(const double _xValue,const  double _yValue,const  double _mError, double _sError):
            xValue(_xValue), yValue(_yValue), mError(_mError), sError(_sError){
        cError = quadr();
    }

    //parametised without systematic
    Measurement(const double _xValue,const  double _yValue,const double _mError):
            xValue(_xValue), yValue(_yValue), mError(_mError), cError  (_mError){
        sError = 0;
    }

    //Accessors don't need with immutable values can be const
    double quadr()const{return sqrt(pow(sError,2) +  pow(mError,2));}
    double quadr(Measurement& M)const{return sqrt(pow(cError,2) +  pow(M.cError,2));}
    double errProd(Measurement& M)const{
        return (yValue*M.yValue) * sqrt(pow(cError/yValue,2) +  pow(M.cError/M.yValue,2));
    }

    double getX()const{return xValue;}
    double getY()const{return yValue;}
    double getmErr()const{return mError;}
    double getsErr()const{return sError;}
    double getcErr()const{return cError;}
    void printReading()const{
        cout<< "Measurement: " <<*this;
        return;
    }


    //Operation Overloads:
    Measurement operator*(Measurement& M)const{
        double c{0};
        c = this->errProd(M);
        Measurement temp(xValue, yValue*M.yValue, c);
        return temp;
    }

    Measurement operator/(Measurement& M)const{
        double c{0};
        c = this->errProd(M);
        Measurement temp(xValue, yValue/M.yValue, c);
        return temp;
    }

    Measurement operator+(Measurement& M)const{
        double c = this->quadr(M);
        Measurement temp(xValue, (yValue+M.yValue)/2, c);
        return temp;
    }

    Measurement operator-(Measurement& M)const{
        double c = this->quadr(M);
        Measurement temp(xValue, (yValue-M.yValue)/2, c);
        return temp;
    }


};

//function prototypes
shared_ptr <Measurement> parse(const string& dataLine);

// type defs
typedef std::shared_ptr<Measurement> PtrMeasurement;
typedef vector<Measurement> ResultsSet;
#endif //PROJECT_MEASUREMENT_H
