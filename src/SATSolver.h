//
// Created by jakub on 6/15/18.
//

#ifndef GRAPHCOLOR_SATSOLVER_H
#define GRAPHCOLOR_SATSOLVER_H

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

    void glucoseParallel();

    static Glucose::Solver* solver;
    static const char* _certified = "CORE -- CERTIFIED UNSAT";


};


#endif //GRAPHCOLOR_SATSOLVER_H
