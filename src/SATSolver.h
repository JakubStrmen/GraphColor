//
// Created by jakub on 6/15/18.
//

#ifndef GRAPHCOLOR_SATSOLVER_H
#define GRAPHCOLOR_SATSOLVER_H

#include <fstream>
#include "../core/Solver.h"
#include "../GlucoseMiniSat/core/Solver.h"
#include "Graph.h"

//using namespace Glucose;

class SATSolver {
public:
    SATSolver();
    virtual ~SATSolver();

    int glucoseSimpFromFile(int argc, char **argv);
    //3coloring of graph based on Glucose Simp SAT Solver
    int doSATSolve3COL(Graph &inputGraph);
    static void printStats(Glucose::Solver& solver);
    static void SIGINT_interrupt(int signum);
    static void SIGINT_exit(int signum);

    void parseDimacs(std::ifstream& in, Glucose::Solver& S);

    void glucoseParallel();


private:
    static int parseInt(std::string inputString);
    static void readClause(std::stringstream& input, Glucose::Solver& S, Glucose::vec<Glucose::Lit>& lits);



};


#endif //GRAPHCOLOR_SATSOLVER_H
