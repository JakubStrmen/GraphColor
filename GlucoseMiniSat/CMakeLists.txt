cmake_minimum_required(VERSION 3.10)
project(GlucoseMiniSat)

set(CMAKE_CXX_STANDARD 11)

include_directories(core)
include_directories(mtl)
include_directories(parallel)
include_directories(simp)
include_directories(utils)

add_executable(GlucoseMiniSat
    core/BoundedQueue.h
    core/Constants.h
    core/Dimacs.h
    core/Solver.cc
    core/Solver.h
    core/SolverStats.h
    core/SolverTypes.h
    mtl/Alg.h
    mtl/Alloc.h
    mtl/Clone.h
    mtl/Heap.h
    mtl/IntTypes.h
    mtl/Map.h
    mtl/Queue.h
    mtl/Sort.h
    mtl/Vec.h
    mtl/VecThreads.h
    mtl/XAlloc.h
    parallel/ClausesBuffer.cc
    parallel/ClausesBuffer.h
    parallel/Main.cc
    parallel/MultiSolvers.cc
    parallel/MultiSolvers.h
    parallel/ParallelSolver.cc
    parallel/ParallelSolver.h
    parallel/SharedCompanion.cc
    parallel/SharedCompanion.h
    parallel/SolverCompanion.cc
    parallel/SolverCompanion.h
    parallel/SolverConfiguration.cc
    parallel/SolverConfiguration.h
    simp/Main.cc
    simp/SimpSolver.cc
    simp/SimpSolver.h
    utils/Options.cc
    utils/Options.h
    utils/ParseUtils.h
    utils/System.cc
    utils/System.h)
