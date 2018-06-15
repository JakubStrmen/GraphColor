//
// Created by jakub on 6/15/18.
//

#ifndef GRAPHCOLOR_SATSOLVER_H
#define GRAPHCOLOR_SATSOLVER_H

#include <errno.h>

#include <signal.h>
#include <zlib.h>
#include <sys/resource.h>

#include "../GlucoseMiniSat/utils/System.h"
#include "../GlucoseMiniSat/utils/ParseUtils.h"
#include "../GlucoseMiniSat/utils/Options.h"
#include "../GlucoseMiniSat/core/Dimacs.h"
#include "../GlucoseMiniSat/simp/SimpSolver.h"

class SATSolver {
public:
    SATSolver();
    virtual ~SATSolver();

    void glucoseSimp();
    void glucoseParallel();


};


#endif //GRAPHCOLOR_SATSOLVER_H
