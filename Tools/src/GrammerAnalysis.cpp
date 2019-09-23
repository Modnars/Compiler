#include "GrammerAnalysis.hpp"

std::ostream &operator<<(std::ostream &os, const Production &p) {
    os << p.left << " -> ";
    for (auto right : p.rights) {
        os << right << " ";
    }
    return os;
}

std::vector<std::string> split(const std::string &str, const std::string &delim) { 
    std::vector<std::string> res;
    if("" == str) return res;
    std::string strs = str + delim; 
    size_t pos;
    size_t size = strs.size();
    for (int i = 0; i < size; ++i) {
        pos = strs.find(delim, i); 
        if(pos < size) { 
            std::string s = strs.substr(i, pos - i); 
            res.push_back(s); 
            i = pos + delim.size() - 1;
        }
    }
    return res;	
}

std::string &trim(std::string &s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

/* Class Production's implementation. */

/**
 * Set the Production's null production contents.
 * @param null The null production's expression.
 */
void Production::setNull(const std::string &null) {
    _null = null;
}

/**
 * Get the Production's null production contents.
 * @return the null production's expression.
 */
std::string Production::null() {
    return _null;
}

/* Initialize the Production's null production contents with "$" */
std::string Production::_null = "$";
