//
//
//

#ifndef PROJECT_EXPERIMENT_H
#define PROJECT_EXPERIMENT_H

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

#include "Measurement.h"
using namespace std;

typedef vector<Measurement> ResultsSet;


class AbstractExperiment{
protected:
    ResultsSet results;
    string ExperimentName;

public:
    AbstractExperiment(string _ExperimentName, ResultsSet _results):
            ExperimentName(_ExperimentName), results(_results){}

    virtual ~AbstractExperiment(){}

    void printDataSet(){
        ResultsSet::iterator pcur;
        for (auto pcur = results.begin(); pcur < results.end(); ++pcur){
            pcur->printReading();
        }
        return;
    }

    void getInfo(){
        cout<<"Experiment name: "<< ExperimentName <<" :" << endl;
        printDataSet();
        return;
    }

    virtual void dataFit()=0;

//    virtual void printDataSet()=0;
//        virtual void Add()=0; //Add Measurement to an experiment Measurement:: PtrMeasurement

};


////////////////////////////////////////////////////////             Linear Class                     ///////////////////////////////////////////////////////////////
//The Regression Statistic struct stores all the constants used to find the gradient of a linear regression.
struct RegStats{
    //Member variables:
    double X_invSqrErr, invSqrErr, Y_invSqrErr, X_invErr_sqrd, XY_invSqrErr, Y_invErr_sqrd;
    // Parameterised Constructor
    RegStats(Measurement& M);
    RegStats(){};
    //overload summation:
    RegStats & operator+=(RegStats& other);
    RegStats & operator+(Measurement& M);
//        friend ostream & operator<<(ostream& os, const RegStats& R);
};

class Linear: public AbstractExperiment{
public:
    //parametised constructor:
    Linear(string _ExperimentName, ResultsSet _results):
            AbstractExperiment(_ExperimentName, _results){}

    //Destructor
    ~Linear(){results.clear();}

    //Member Funcitons:
    void dataFit(){
        //Calculate the summations for the regression:
        RegStats Sum = Summation();
//        cout<<total<<endl;

        //Regression Line:
        regVariable Variables = regressionCalc(Sum);
        cout<<"Line of best fit (y = mx + c): y = " << Variables.grad <<" (+/- "<<Variables.gradErr<<" ) x " <<showpos <<Variables.intcpt<<" (+/- "<<noshowpos<<Variables.intcptErr<<" )"<<endl;

        //Chi Sqr:
        double Chi = ChiSqr_Calc(Variables, results);
        cout << "Chi Squared Values: " << Chi <<endl;
        return;
    }


private:
//    //The Regression Statistic struct stores all the constants used to find the gradient of a linear regression.
//    struct RegStats{
//        //Member variables:
//        double X_invSqrErr, invSqrErr, Y_invSqrErr, X_invErr_sqrd, XY_invSqrErr, Y_invErr_sqrd;
//        // Parameterised Constructor
//        RegStats(Measurement& M);
//        //overload summation:
//        RegStats & operator+=(RegStats& other);
//        RegStats & operator+(Measurement& M);
////        friend ostream & operator<<(ostream& os, const RegStats& R);
//    };

    //The Regression Variable struct stores constants that define the linear refression
    struct regVariable{
        double grad; double gradErr;
        double intcpt; double intcptErr;
    };

    //Calculates the constants to define the regression line
    regVariable regressionCalc(RegStats& R)const;

    //Calculate Chi square (measurment vector and regression line variables)
    double ChiSqr_Calc(regVariable& V, ResultsSet results)const;

    //Regression Statistic summation calculation:
    RegStats Summation();

};

class Constant: public AbstractExperiment{
private:
    struct WeightStats{
        //Variables:
        double Y_InvsqrErr;
        double InvsqrErr;
        //Parametised constructor:
        WeightStats(Measurement& M);
        //overload operator:
        WeightStats & operator+(Measurement& M);
    };

    //Calc weighted mean/SD:
    WeightStats WeightedCalc();

public:
    //paramterised constructor
    Constant(string _ExperimentName, ResultsSet _results):
            AbstractExperiment(_ExperimentName, _results){}

    ~Constant(){results.clear();}

    void dataFit(){
        WeightStats variables = WeightedCalc();
        double wMean = variables.Y_InvsqrErr/variables.InvsqrErr;
        double wSD = sqrt(1 / pow(variables.InvsqrErr,2));
        cout << "Weighted Values: " << wMean <<" +/- "<< wSD <<endl;
        return;
    }
};

#endif //PROJECT_EXPERIMENT_H
