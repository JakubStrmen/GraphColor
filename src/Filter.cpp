/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Filter.cpp
 * Author: jakub
 *
 * Created on March 21, 2018, 2:31 PM
 */

#include "../include/Filter.h"
#include <fstream>

Filter::Filter() {
}

Filter::Filter(std::string paInputFile){
    inputFile = paInputFile;
}


Filter::Filter(const Filter& orig) {
}

Filter::~Filter() {
}

void Filter::startWork(){
    if(inputFile.empty()){
        std::cout<<"Error! Input file not specified! \n";
        return;
    }

    std::ifstream myfile(inputFile);
    std::string line;
    // read line from input file
    std::getline(myfile, line);

    myfile.close();

    std::cout<<"File specified!  \n";

}


