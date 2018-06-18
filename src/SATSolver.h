//
// Created by jakub on 6/15/18.
//

#ifndef GRAPHCOLOR_SATSOLVER_H
#define GRAPHCOLOR_SATSOLVER_H

#include <fstream>
#include "../core/Solver.h"
#include "../GlucoseMiniSat/core/Solver.h"

//using namespace Glucose;

class SATSolver {
public:
    SATSolver();
    virtual ~SATSolver();

    int glucoseSimp(int argc, char** argv);
    static void printStats(Glucose::Solver& solver);
    static void SIGINT_interrupt(int signum);
    static void SIGINT_exit(int signum);

    static void parseDimacs(std::ifstream in, Glucose::Solver& S);

    void glucoseParallel();





};


#endif //GRAPHCOLOR_SATSOLVER_H
