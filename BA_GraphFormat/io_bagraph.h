//
// Created by jakub on 9/7/18.
//
// Borrowed and modified from:
// Created by Jan Karabas on 12/01/2017.
//

#ifndef GRAPHCOLOR_IO_BAGRAPH_H
#define GRAPHCOLOR_IO_BAGRAPH_H



#include <fstream>
#include <queue>
#include "../src/Graph.h"


class GraphBAReader {
public:
    GraphBAReader(std::string &);

    ~GraphBAReader();

    bool has_next() const {
        return bool(m_strm) && m_strm.good() && m_strm.is_open() && (m_lastread < m_noofgraphs);
    };

    Graph read();

    size_t number_of_graphs() const { return m_noofgraphs; }

    size_t last_read() const { return m_lastread; }

    size_t actual_line() const { return m_actual_line; }

private:
    std::string m_path;
    std::ifstream m_strm;
    size_t m_actual_line;
    size_t m_noofgraphs;
    size_t m_lastread;

    std::vector<int> next_line();
};

class GraphBAWriter {
public:
    explicit GraphBAWriter(char const *filename, size_t file_bound = 30000) :
            m_filebound{file_bound}, m_no_written{0}, m_actual_line{0},
            file_handle{nullptr}, m_orig_path{filename}, m_path{},
            m_no_partfile{0} {}

    GraphBAWriter(GraphBAWriter const &) = delete;

    ~GraphBAWriter();

    std::size_t no_written() const { return m_no_written; }

    std::size_t actual_line() const { return m_actual_line; } //?

    bool bad() const {
        return (file_handle != nullptr) and (std::ferror(file_handle) != 0);
    }

    void flush() {}; // just the implementation of the interface

    void write(Graph const &);

    void write(Graph const &graph, std::vector<std::string> const &msgs) {
        for (auto elt : msgs) add_graph_comment(elt);
        write(graph);
    }

    void add_graph_comment(std::string const &msg) {
        if (not(msg.empty())) m_graph_remarks.push_back(msg);
    }

    void add_file_comment(std::string const &msg) {
        if (not(msg.empty())) m_file_remarks.push_back(msg);
    }

    void add_file_comment(char const *msg) {
        add_file_comment(std::string(msg));
    }

    void clear_graph_comments() {
        m_graph_remarks.clear();
    }

    void clear_file_comments() {
        m_file_remarks.clear();
    }


private:
//    std::string m_path;
//    std::ifstream m_strm;
//    size_t m_lastwritten;
    size_t m_filebound;
    size_t m_no_written;
    size_t m_actual_line;
    unsigned int m_no_partfile;

    FILE *file_handle;
    std::string m_orig_path;
    std::string m_path;

    std::queue<std::string> m_buffer;
    std::vector<std::string> m_file_remarks;
    std::vector<std::string> m_graph_remarks;

//
//    //void dump();
//
//    std::string m_tmppath;
//    std::ifstream m_tmpstrm;
//    size_t m_noofgraphs;
//    size_t m_actualwritten;
    void create_filename();

    void save();
};

#endif //GRAPHCOLOR_IO_BAGRAPH_H
