#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <cctype>

// Convert PascalCase → snake_case
std::string to_snake_case(const std::string& s) {
    std::string out;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (std::isupper(c) && i > 0) out += '_';
        out += std::tolower(c);
    }
    return out;
}

// Generate include guard
std::string include_guard(const std::string& filename) {
    std::string g;
    for (char c : filename) {
        g += std::isalnum(c) ? std::toupper(c) : '_';
    }
    g += "_H";
    return g;
}

// Trim helper
std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" ");
    size_t b = s.find_last_not_of(" ");
    return s.substr(a, b - a + 1);
}

// Write AST
void defineAst(const std::string& includeDir,
               const std::string& srcDir,
               const std::string& baseName,
               const std::vector<std::string>& types)
{
    std::string filename = to_snake_case(baseName);
    std::ofstream h(includeDir + "/" + filename + ".h");
    std::ofstream cpp(srcDir + "/" + filename + ".cpp");

    if (!h || !cpp) throw std::runtime_error("Could not open output files");

    // ================= HEADER =================
    std::string guard = include_guard(filename);
    h << "#ifndef " << guard << "\n#define " << guard << "\n\n";

    h << "#include <any>\n";
    h << "#include <memory>\n";
    h << "#include \"token.h\"\n";
    if (baseName != "Expr") {
        h << "#include \"expr.h\"\n";
    }
    h << "\n";

    // Forward declarations
    for (auto& t : types) {
        h << "class " << trim(t.substr(0, t.find(':'))) << ";\n";
    }
    h << "\n";

    // Base class + Visitor
    h << "class " << baseName << " {\npublic:\n";
    h << "    class Visitor {\n    public:\n";
    h << "        virtual ~Visitor() = default;\n";

    for (auto& t : types) {
        std::string name = trim(t.substr(0, t.find(':')));
        h << "        virtual std::any visit"
          << name << baseName
          << "(" << name << "* "
          << (baseName == "Expr" ? "expr" : "stmt")
          << ") = 0;\n";
    }

    h << "    };\n\n";
    h << "    virtual ~" << baseName << "() = default;\n";
    h << "    virtual std::any accept(Visitor& visitor) = 0;\n";
    h << "};\n\n";

    // Subclasses
    for (auto& t : types) {
        auto colon = t.find(':');
        std::string className = trim(t.substr(0, colon));
        std::string fields = trim(t.substr(colon + 1));

        h << "class " << className << " : public " << baseName << " {\npublic:\n";

        std::stringstream fs(fields);
        std::string field;
        while (std::getline(fs, field, ',')) {
            std::stringstream f(trim(field));
            std::string type, name;
            f >> type >> name;
            h << "    " << type << " " << name << ";\n";
        }

        h << "\n    explicit " << className << "(" << fields << ");\n";
        h << "    std::any accept(Visitor& visitor) override;\n";
        h << "};\n\n";
    }

    h << "#endif // " << guard << "\n";

    // ================= CPP =================
    cpp << "#include \"" << filename << ".h\"\n\n";

    for (auto& t : types) {
        auto colon = t.find(':');
        std::string className = trim(t.substr(0, colon));
        std::string fields = trim(t.substr(colon + 1));

        cpp << className << "::" << className << "(" << fields << ")\n    : ";

        std::stringstream fs(fields);
        std::string field;
        bool first = true;
        while (std::getline(fs, field, ',')) {
            std::stringstream f(trim(field));
            std::string type, name;
            f >> type >> name;

            if (!first) cpp << ", ";
            first = false;

            if (type.find("std::unique_ptr") != std::string::npos)
                cpp << name << "(std::move(" << name << "))";
            else
                cpp << name << "(" << name << ")";
        }
        cpp << " {}\n\n";

        cpp << "std::any " << className
            << "::accept(Visitor& visitor) {\n";
        cpp << "    return visitor.visit"
            << className << baseName << "(this);\n";
        cpp << "}\n\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: generate_ast <include_dir> <src_dir>\n";
        return 64;
    }

    std::string include_dir = argv[1];
    std::string src_dir = argv[2];

    defineAst(include_dir, src_dir, "Expr", {
        "Assign     : Token name, std::unique_ptr<Expr> value",
        "Binary     : std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right",
        "Grouping   : std::unique_ptr<Expr> expr",
        "Literal    : std::any value",
        "Unary      : Token op, std::unique_ptr<Expr> right",
        "Variable   : Token name"
    });

    defineAst(include_dir, src_dir, "Stmt", {
        "Expression : std::unique_ptr<Expr> expr",
        "Print      : std::unique_ptr<Expr> expr",
        "Var        : Token name, std::unique_ptr<Expr> initializer"
    });

    return 0;
}
