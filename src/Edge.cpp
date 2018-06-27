//
// Created by Jan Karabas on 23/05/17.
//

#include "Edge.h"

// TODO - could be better documented ...

Undirected_edge::Undirected_edge(vertex_type const &from,
                                 vertex_type const &to) noexcept
        : m_from(from), m_to(to) {
    if (from > to)
        std::swap(m_from, m_to);
};

Undirected_edge &Undirected_edge::
operator=(Undirected_edge const &other) noexcept {
    if (this != &other) {
        this->m_from = other.m_from;
        this->m_to = other.m_to;
    }
    return *this;
}

bool Undirected_edge::operator==(Undirected_edge const &other) const noexcept {
    return this == &other or (m_from == other.m_from and m_to == other.m_to) or
           (m_from == other.m_to and m_to == other.m_from);
}

bool Undirected_edge::operator<(Undirected_edge const &other) const noexcept {
    return m_from == other.m_from ? m_to < other.m_to
                                  : m_from < other.m_from;
}

bool Undirected_edge::contains(vertex_type const &vertex) const noexcept {
    return vertex == m_from or vertex == m_to;
}

Undirected_edge::vertex_type
Undirected_edge::traverse(vertex_type const &vertex) const noexcept {
    if (vertex == m_from)
        return m_to;
    if (vertex == m_to)
        return m_from;
    return vertex;
}

std::vector<Undirected_edge::vertex_type> Undirected_edge::vertices() const {
    std::vector<vertex_type> ret{};

    if (m_from != std::numeric_limits<vertex_type>::max())
        ret.push_back(m_from);
    if (m_to != m_from and m_to != std::numeric_limits<vertex_type>::max())
        ret.push_back(m_to);

    return ret;
}

std::ostream &operator<<(std::ostream &strm, Undirected_edge const &edge) {
    strm << "{" << edge.m_from << ", " << edge.m_to << "}";
    return strm;
}

bool Undirected_edge::is_independent_of(Undirected_edge const &other) const {
    return m_from == other.m_from or m_to == other.m_to or m_from == other.m_to or
           m_to == other.m_from;
}
