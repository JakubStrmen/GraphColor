//
// Created by jakub on 3/24/18.
//

#ifndef GRAPHCOLOR_FILTER_H
#define GRAPHCOLOR_FILTER_H

#include <iostream>

class Filter {
public:
    Filter();
    Filter(std::string paInputFile);
    Filter(const Filter& orig);
    virtual ~Filter();
    void startWork();


private:
    std::string inputFile;

};


#endif //GRAPHCOLOR_FILTER_H
