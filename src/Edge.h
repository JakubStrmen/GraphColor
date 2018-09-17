//
// Created by jakub on 6/27/18.
//

#ifndef UNDIRECTED_EDGE_HPP
#define UNDIRECTED_EDGE_HPP

#include <limits>
#include <ostream>
#include <vector>

/// this is an undirected edge
class Undirected_edge {
public:
    using vertex_type = int;

    Undirected_edge() noexcept
            : m_from(std::numeric_limits<vertex_type>::max()),
              m_to(std::numeric_limits<vertex_type>::max()) {}

    Undirected_edge(vertex_type const &, vertex_type const &) noexcept;
    Undirected_edge(Undirected_edge const &) = default;

    Undirected_edge &operator=(Undirected_edge const &) noexcept;

    bool operator==(Undirected_edge const &) const noexcept;
    bool operator!=(Undirected_edge const &other) const noexcept {
        return !(*this == other);
    }
    bool operator<(Undirected_edge const &other) const noexcept;

    bool contains(vertex_type const &) const noexcept;
    vertex_type traverse(vertex_type const &) const noexcept;
    std::vector<vertex_type> vertices() const;
    vertex_type from() const { return m_from; }
    vertex_type to() const { return m_to; }
    friend std::ostream &operator<<(std::ostream &, Undirected_edge const &);
    bool is_independent_of(Undirected_edge const &) const;

    void setColour(int newColour);
    int getColour();
    bool coloured();

    std::string toString();


private:
    vertex_type m_from, m_to;
    int _colour;
    bool _coloured;

    void init();
};

#endif // UNDIRECTED_EDGE_HPP

