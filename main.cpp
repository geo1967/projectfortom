//
//  main.cpp
//  Read_measurements
//
//  Created by Thomas Soares Mullen on 29/03/2018.
//  Copyright © 2018 Thomas Soares Mullen. All rights reserved.
//

#include <assert.h>
#include "Measurement.h"
#include "Experiment.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//choices namespace contains enum for linear and constant fit used for switch case
namespace choices{
    enum ExperimentTYPE{
        Linear = 1 ,
        Constant = 2,
    };
    
    map<string, ExperimentTYPE> trends;
    
    void register_trends(){
        trends["Linear"]   = Linear;
        trends["Constant"] = Constant;
    }
}

////////////////////////////////////////////////////////             Experiment type Assignment                ///////////////////////////////////////////////////////////////

 //Function that Assigns data set to experiment class
 void Results_Assignment(ResultsSet& temporary_dataSet){
     cout<<"Please enter the experiment name: "<<endl;
     string experimentTitle;
     getline(cin,experimentTitle);
 
     choices::register_trends();
     bool repeat {true};
     cout<<"Please enter the data trend expected from the experimental results [Linear/Constant]: "<<endl;
     string input;
     cin >> input;
     do{
         switch( choices::trends[input] ){
             case choices::Linear:{
                 
                 cout << "Linear fit selected"<<endl;
                 Linear expL(experimentTitle, temporary_dataSet);
                 expL.getInfo();
                 expL.dataFit();
                 repeat = !repeat;
                 break;
             }
             case choices::Constant:{
                 cout << "Constant fit selected!"<<endl;
                 Constant expC(experimentTitle, temporary_dataSet);
                 expC.getInfo();
                 expC.dataFit();
                 repeat = !repeat;
                 break;
             }
             default:
                 cout << "Invalid input please select Linear/Constant." <<endl;
                 cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 cin >> input;

         }
     }while( repeat );
     return;
}

////////////////////////////////////////////////////////                 Read in file                        ///////////////////////////////////////////////////////////////

void readinMeasurements(){
    //Get raw data file
    cout<<"Please enter the file name you wish to open"<<endl;
    string fileName; //fileName: (/Users/Tom/Library/Developer/Xcode/DerivedData/Read_measurements-abewmwsgentonadxkftczgzdiphj/Build/Products/Debug) called 1.txt
    getline(cin,fileName);
    fstream dataFile(fileName);
    //check open success
    if(dataFile.fail()){
        cerr<<"file cannot open"<<endl;
        exit(-1);
    }
    int measurementCount {0};
    int rogueDataCount{0};
    //read in line
    ResultsSet fileResultSet;
    string measurementData;
    while (getline(dataFile,measurementData)){
        try{
            measurementCount++;
            shared_ptr<Measurement> sp = parse(measurementData);
            //            cout << *sp;
            fileResultSet.push_back(*sp);
        }
        catch (const std::invalid_argument& ia) {
            cerr << "Invalid argument: " << ia.what() << '\n';
            rogueDataCount++;
        }
        catch(const exception& e){
            cerr<< e.what() <<endl;
        }
    }
    
    //print out vector of measurements
    ResultsSet::iterator pcur;
    for (auto pcur = fileResultSet.begin(); pcur < fileResultSet.end(); ++pcur){
        pcur->printReading();
    }
    
    cout<<"Number of readable measurements: " << measurementCount <<". \nNumber of rogue measurements: " << rogueDataCount <<endl<<endl;
    //close file
    dataFile.clear();
    dataFile.close();
    Results_Assignment(fileResultSet);
    return;
}

////////////////////////////////////////////////////             Store experiment into dictionary            ///////////////////////////////////////////////////////////////


/************************************************
 * Test functions defined here
 */

void test_add_RegStats()
{
    Measurement m1(1,2,3,4), m2(m1);
    RegStats a;
    //TODO this properly
    assert(a.X_invSqrErr==0 && "RegStats += failed "  );
    assert(a.invSqrErr ==0 && "RegStats += failed ");
    assert(a.Y_invSqrErr ==0 && "RegStats += failed ");
    a + m1;
    assert(a.X_invSqrErr==1 && "RegStats += failed " );
    assert(a.invSqrErr ==2&& "RegStats += failed ");
    assert(a.Y_invSqrErr ==3 && "RegStats += failed ");

    a + m1;
    assert(a.X_invSqrErr==2 && "RegStats += failed " );
    assert(a.invSqrErr ==3&& "RegStats += failed ");
    assert(a.Y_invSqrErr ==4 && "RegStats += failed ");
}

void run_all_tests()
{
    test_add_RegStats();
}



int main()
{

//     /* comment this back in later after testing
//     *
//     * readinMeasurements();
//     *
//     * /

     run_all_tests();
    return 0;
}



/*
 SAVE NOTES:
 Urgent changes:
 *** Change the Switch case Funciton and use polymorphism to store temp vector as one of the derived experiment class
 *** Store experiment in a <Map> with experiment name as Key
 *** Ask user if he has another experiment to add
 
 Later Add:
 * Measurement as abstract class --> derived Systematic, Non-Systematic
 * User manually input measurement data
 * Print experiemtn data to output fil
 
 */

//    //Copy and Move overloads:
//    //Copy constructor
//    /*Measurement(Measurement const& Copy)
//    {
//        //deep copy
//        //    cout << " deep copy\n";
//        xValue = Copy.xValue;
//        yValue = Copy.yValue;
//        mError =
//    }*/
//
//    //Move constructor
//    Measurement(Measurement&& Move)
//    {
//        cout << "move constructor" << endl;
//        xValue = Move.xValue;
//        yValue = Move.yValue;
//        mError = Move.mError;
//        sError = Move.sError;
//        cError = Move.cError;
//
//    }
//    //Copy Overload
//    Measurement & operator =(Measurement &Copy)
//    {
//        if (&Copy == this) return*this; //no self assignment
//        //First delete this objects array
//        xValue = yValue = mError = sError = cError = 0;
//        xValue = Move.xValue;
//        yValue = Move.yValue;
//        mError = Move.mError;
//        sError = Move.sError;
//        cError = Move.cError;
//        return *this; // Special pointer
//    }
//
//    //Move Overload
//    Measurement & operator =(Measurement&& Move)
//    {
////        cout << "Move assignment" << endl;
//        swap(xValues, Move.xValues);
//        swap(yValues, Move.yValues);
//        swap(mError, Move.mError);
//        swap(sError, Move.sError);
//        swap(cError, Move.cError);
//        return*this;
//    }

