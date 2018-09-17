//
// Created by jakub on 9/7/18.
//
// Borrowed and modified from:
// Created by Jan Karabas on 12/01/2017.
//

#include "io_bagraph.h"

#include <iostream>
#include <sstream>
#include <iterator>

//#include <boost/filesystem.hpp>

std::vector<int> GraphBAReader::next_line() {
    std::vector<int> ret_vec = {};
    std::string line;
    std::stringstream parsed;
    unsigned par_level = 0;
    while (std::getline(m_strm, line)) {
        parsed.clear();
        m_actual_line++;
        for (auto ch : line) {
            if (ch == '#')
                break;
            if (ch == '{') par_level++;
            if (par_level == 0) parsed << ch;
            if (ch == '}') par_level--;
        }
        std::copy(std::istream_iterator<int>(parsed), std::istream_iterator<int>(), std::back_inserter(ret_vec));
        if (!ret_vec.empty()) break;
    }

    return ret_vec;
}

GraphBAReader::GraphBAReader(std::string &path) : m_path{path}, m_strm{path.c_str()}, m_actual_line(0), m_noofgraphs(0),
                                                  m_lastread(0) {

    // read the number of graphs in the file
    std::vector<int> tmp = next_line();
    if (tmp.size() == 1)
        m_noofgraphs = static_cast<size_t>(tmp[0]);
    else {
        if (m_strm && m_strm.is_open()) {
            std::stringstream ss;
            ss << "Syntax error at line " << m_actual_line << ".";
            m_strm.close();
            throw std::runtime_error(ss.str());
        }
        throw std::runtime_error("Error opening file. \'" + m_path + "\'");
    }


}

GraphBAReader::~GraphBAReader() {
    if (m_strm && m_strm.is_open())
        m_strm.close();
}

Graph GraphBAReader::read() {
    std::vector<int> buffer = {};
//    undirected_graph gph{};
//    Graph newGraph;

    std::vector<Vertex > vertices;
    std::vector<Undirected_edge> edges;

    int graphOrder;

    // read the graph number
    buffer = next_line();
    if (buffer.size() == 1) {
        m_lastread = static_cast<size_t >(buffer[0]);
    } else {
        if (m_strm && m_strm.is_open()) {
            std::stringstream ss;
            ss << "Syntax error at line " << m_actual_line << ".";
            m_strm.close();
            throw std::runtime_error(ss.str());
        }
    }
    // order of the graph
    buffer = next_line();
    if (buffer.size() == 1 && buffer[0] > 0) {
        graphOrder = buffer[0];
//        for (Graph::vertex_type v = 0; v < buffer[0]; v++) {
//            Vertex newVert(v);
//            vertices.push_back(newVert);
//            // newGraph.add_vertex(v);
//        }
    } else {
        if (m_strm && m_strm.is_open()) {
            std::stringstream ss;
            ss << "Syntax error at line " << m_actual_line << ".";
            m_strm.close();
            throw std::runtime_error(ss.str());
        }
    }
    // add edges and vertices
    int actual_vertex = 0;
    while (actual_vertex < graphOrder) {
        buffer = next_line();

        if (not(buffer.empty())) {
            Vertex newVert(actual_vertex);
            for (auto u : buffer) {
                if (u > actual_vertex){
                    Undirected_edge newEdge(actual_vertex, u);
                    edges.push_back(newEdge);
//                    gph.add_edge(actual_vertex, u);
                }
                newVert.addNeighbor(u);
            }
            actual_vertex++;
            vertices.push_back(newVert);
        }
    }

    Graph newGraph(vertices, edges);
    return newGraph;
}



GraphBAWriter::~GraphBAWriter() {
    save();
    if (file_handle != nullptr) {
        std::fclose(file_handle);
        file_handle = nullptr;
    }
}

// TODO - maybe later if needed
/*
void GraphBAWriter::write(Graph const &graph) {
    std::stringstream ss{};
    if (graph.order() == 0)
        return;
    ++m_no_written;

    // ss is empty
    ss << m_no_written;  // graph no.
    m_buffer.push(ss.str());

    for (auto const &elt : m_graph_remarks) { // graph remarks
        std::stringstream().swap(ss);
        ss << "{ " << elt << " }";
        m_buffer.push(ss.str());
    }
    m_graph_remarks.clear();

    std::stringstream().swap(ss);
    ss << graph.order() << ""; // graph order
    m_buffer.push(ss.str());

    // we have to normalise the vertex set
    auto vset = graph.vertices(); // set of vertices
    std::vector<undirected_graph::vertex_type> vertices(vset.begin(), vset.end());
    auto const &beg_it = vertices.begin();
    auto const &end_it = vertices.end();
    // the neighbours
    for (auto u : vertices) {
        std::stringstream().swap(ss);
        auto neigbours = graph.neigbours(u);
        std::string separator;
        for (auto v : neigbours) {
            auto vit = std::find(beg_it, end_it, v);

            assert(vit != end_it); // this is a program logic error

            ss << separator << std::distance(beg_it, vit);
            separator = " ";

        }
        m_buffer.push(ss.str());
    }

    if (m_no_written == m_filebound) {
        save();
        ++m_no_partfile;
    }
}
*/


void GraphBAWriter::save() {
    if (not(m_buffer.empty())) {
        if (file_handle == nullptr) {
            create_filename();
            file_handle = std::fopen(m_path.c_str(), "w");
            if ((file_handle == nullptr) or (std::ferror(file_handle) != 0))
                throw std::runtime_error("Cannot open BA file.");
        }

        for (auto const &elt : m_file_remarks) { // store file remarks
            std::fprintf(file_handle, "{ %s }\n", elt.c_str());
        }

        // number of graphs
        std::fprintf(file_handle, "%zu\n", m_no_written);


        while (!m_buffer.empty()) {
            ++m_actual_line;
            std::fprintf(file_handle, "%s\n", m_buffer.front().c_str());
            m_buffer.pop();
        }
        std::fflush(file_handle);

    }
}


void GraphBAWriter::create_filename() { // velmi neobratna logika
    std::stringstream ss;

    ss << m_orig_path;
    if (m_no_partfile > 0) {
        ss.width(5);
        ss.fill('0');
        ss << "." << m_no_partfile;
    }
    m_path = ss.str();
}
