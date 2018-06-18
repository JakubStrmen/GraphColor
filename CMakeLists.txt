cmake_minimum_required(VERSION 3.9)
project(GraphColor)

set(CMAKE_CXX_STANDARD 11)

# added -lz
set(GCC_COVERAGE_COMPILE_FLAGS "-lz")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lz ")

#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -lz -Wall "      )

#SET_TARGET_PROPERTIES(zlib PROPERTIES LINK_FLAGS -Wl,-lz)

#find_package (zlib-1.2.11)

# added -fopenmp
include(FindOpenMP)
if(OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else(OPENMP_FOUND)
    message("ERROR: OpenMP could not be found! ")
endif(OPENMP_FOUND)


add_subdirectory (GlucoseMiniSat)

include_directories( /home/jakub/Git/GraphColor/GlucoseMiniSat/core )
include_directories( /home/jakub/Git/GraphColor/GlucoseMiniSat/mtl )
include_directories(core)
include_directories(mtl)
include_directories(parallel)
include_directories(simp)
include_directories(utils)

set(GLUCOSE_FILES
        GlucoseMiniSat/core/BoundedQueue.h
        GlucoseMiniSat/core/Constants.h
        GlucoseMiniSat/core/Dimacs.h
        GlucoseMiniSat/core/Solver.cc
        GlucoseMiniSat/core/Solver.h
        GlucoseMiniSat/core/SolverStats.h
        GlucoseMiniSat/core/SolverTypes.h
        GlucoseMiniSat/mtl/Alg.h
        GlucoseMiniSat/mtl/Alloc.h
        GlucoseMiniSat/mtl/Clone.h
        GlucoseMiniSat/mtl/Heap.h
        GlucoseMiniSat/mtl/IntTypes.h
        GlucoseMiniSat/mtl/Map.h
        GlucoseMiniSat/mtl/Queue.h
        GlucoseMiniSat/mtl/Sort.h
        GlucoseMiniSat/mtl/Vec.h
        GlucoseMiniSat/mtl/VecThreads.h
        GlucoseMiniSat/mtl/XAlloc.h
        GlucoseMiniSat/parallel/ClausesBuffer.cc
        GlucoseMiniSat/parallel/ClausesBuffer.h
        GlucoseMiniSat/parallel/MultiSolvers.cc
        GlucoseMiniSat/parallel/MultiSolvers.h
        GlucoseMiniSat/parallel/ParallelSolver.cc
        GlucoseMiniSat/parallel/ParallelSolver.h
        GlucoseMiniSat/parallel/SharedCompanion.cc
        GlucoseMiniSat/parallel/SharedCompanion.h
        GlucoseMiniSat/parallel/SolverCompanion.cc
        GlucoseMiniSat/parallel/SolverCompanion.h
        GlucoseMiniSat/parallel/SolverConfiguration.cc
        GlucoseMiniSat/parallel/SolverConfiguration.h
        GlucoseMiniSat/simp/SimpSolver.cc
        GlucoseMiniSat/simp/SimpSolver.h
        GlucoseMiniSat/utils/Options.cc
        GlucoseMiniSat/utils/Options.h
        GlucoseMiniSat/utils/ParseUtils.h
        GlucoseMiniSat/utils/System.cc
        GlucoseMiniSat/utils/System.h)

add_executable(GraphColor
        main.cpp
        src/Graph.cpp
        include/Graph.h
        src/depthFirst.c
        include/depthFirst.h
        src/mystack.c
        include/mystack.h
        src/Filter.cpp
        include/Filter.h
        src/BreadthFirst.cpp
        include/BreadthFirst.h
        src/Vertex.cpp
        src/Vertex.h
        src/GraphNew.cpp
        src/GraphNew.h
        src/SATSolver.cpp
        src/SATSolver.h

        ${GLUCOSE_FILES})

