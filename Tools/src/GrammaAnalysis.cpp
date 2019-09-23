#include "GrammaAnalysis.hpp"

std::ostream &operator<<(std::ostream &os, const Production &p) {
    os << p.left << " -> ";
    for (auto right : p.rights) {
        os << right << " ";
    }
    return os;
}
