#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <set>

struct E {
    int s;
    int t;
    int w;
    bool g;
};

std::string cl(const std::string& x) {
    std::string r = "";
    for (char c : x) {
        if (!std::isspace(c)) {
            r += c;
        }
    }
    return r;
}

std::vector<std::vector<int>> pa(const std::string& s) {
    std::vector<std::vector<int>> r;
    std::vector<int> cr;
    std::string cn = "";
    int d = 0;

    for (size_t i = 0; i < s.length(); ++i) {
        char c = s[i];
        if (c == '[') {
            d++;
            if (d == 2) {
                cr.clear();
            }
        } else if (c == ']') {
            if (d == 2) {
                if (!cn.empty()) {
                    cr.push_back(std::stoi(cn));
                    cn.clear();
                }
                r.push_back(cr);
            }
            d--;
        } else if (c == ',') {
            if (d == 2) {
                if (!cn.empty()) {
                    cr.push_back(std::stoi(cn));
                    cn.clear();
                }
            }
        } else if (std::isdigit(c) || c == '-') {
            cn += c;
        }
    }
    return r;
}

std::string es(const std::string& x) {
    std::string r = "";
    for (char c : x) {
        if (c == '"') r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else r += c;
    }
    return r;
}

void er(const std::string& m) {
    std::cout << "{\n  \"error\": \"" << es(m) << "\"\n}\n";
    std::exit(1);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        er("Usage: a <f> <r>");
    }

    std::string f = argv[1];
    std::string r = argv[2];
    std::string c = cl(r);

    if (c.empty() || c[0] != '[' || c[c.length() - 1] != ']') {
        er("Invalid format: Input must be enclosed in brackets like [[...]]");
    }

    std::vector<std::vector<int>> d;
    try {
        d = pa(c);
    } catch (const std::exception& e) {
        er("Parsing error: " + std::string(e.what()));
    }

    if (d.empty()) {
        er("Empty structure.");
    }

    if (f == "auto") {
        bool sq = true;
        size_t n = d.size();
        for (const auto& row : d) {
            if (row.size() != n) {
                sq = false;
                break;
            }
        }

        if (sq && n > 0) {
            f = "matrix";
        } else {
            bool ae = true;
            for (const auto& row : d) {
                if (row.size() != 2 && row.size() != 3) {
                    ae = false;
                    break;
                }
            }
            if (ae) {
                f = "edges";
            } else {
                f = "adj_list";
            }
        }
    }

    std::set<int> un;
    std::vector<E> ed;

    if (f == "edges") {
        for (const auto& row : d) {
            if (row.size() < 2 || row.size() > 3) {
                er("Edge row size error: " + std::to_string(row.size()));
            }
            int u = row[0];
            int v = row[1];
            int w = (row.size() == 3) ? row[2] : 1;
            bool g = (row.size() == 3);

            ed.push_back({u, v, w, g});
            un.insert(u);
            un.insert(v);
        }
    } else if (f == "adj_list") {
        int n = d.size();
        for (int u = 0; u < n; ++u) {
            un.insert(u);
            for (int v : d[u]) {
                ed.push_back({u, v, 1, false});
                un.insert(v);
            }
        }
    } else if (f == "matrix") {
        int n = d.size();
        for (int i = 0; i < n; ++i) {
            un.insert(i);
            if (d[i].size() != (size_t)n) {
                er("Matrix not square. Row " + std::to_string(i) + " size " + std::to_string(d[i].size()));
            }
            for (int j = 0; j < n; ++j) {
                int w = d[i][j];
                if (w != 0) {
                    ed.push_back({i, j, w, (w != 1)});
                }
            }
        }
    } else {
        er("Unknown format: " + f);
    }

    std::cout << "{\n";
    std::cout << "  \"format_detected\": \"" << f << "\",\n";
    std::cout << "  \"num_nodes\": " << un.size() << ",\n";
    
    std::cout << "  \"nodes\": [";
    bool fst = true;
    for (int node : un) {
        if (!fst) std::cout << ", ";
        std::cout << node;
        fst = false;
    }
    std::cout << "],\n";

    std::cout << "  \"edges\": [\n";
    for (size_t i = 0; i < ed.size(); ++i) {
        std::cout << "    {\n";
        std::cout << "      \"source\": " << ed[i].s << ",\n";
        std::cout << "      \"target\": " << ed[i].t << ",\n";
        std::cout << "      \"weight\": " << ed[i].w << ",\n";
        std::cout << "      \"weighted\": " << (ed[i].g ? "true" : "false") << "\n";
        std::cout << "    }" << (i + 1 == ed.size() ? "" : ",") << "\n";
    }
    std::cout << "  ]\n";
    std::cout << "}\n";

    return 0;
}
