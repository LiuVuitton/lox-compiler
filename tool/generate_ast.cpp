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

// Generate a valid include guard macro
std::string include_guard(const std::string& filename) {
    std::string guard = to_snake_case(filename);
    for (auto &c : guard) if (!isalnum(c)) c = '_';
    for (auto &c : guard) c = std::toupper(c);
    guard += "_H";
    return guard;
}

// Map Java "Object" to C++ std::any
std::string map_type(const std::string& t) {
    if (t == "Object") return "std::any";
    return t;
}

// Forward declarations for header
void writeForwardDeclarations(std::ofstream& writer, const std::vector<std::string>& types) {
    for (const auto& t : types) {
        std::string name = t.substr(0, t.find(':'));
        size_t s = name.find_first_not_of(" ");
        size_t e = name.find_last_not_of(" ");
        writer << "class " << name.substr(s, e - s + 1) << ";\n";
    }
    writer << "\n";
}

// Write visitor interface to header
void defineVisitor(std::ofstream& writer,
                   const std::string& baseName,
                   const std::vector<std::string>& types)
{
    writer << "class " << baseName << "Visitor {\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "Visitor() = default;\n\n";

    for (const auto& type : types) {
        std::string typeName = type.substr(0, type.find(':'));
        size_t s = typeName.find_first_not_of(" ");
        size_t e = typeName.find_last_not_of(" ");
        typeName = typeName.substr(s, e - s + 1);

        writer << "    virtual std::any visit" << typeName
               << "(const class " << typeName << "& expr) = 0;\n";
    }

    writer << "};\n\n";
}

// Write class declarations to header
void defineClassHeader(std::ofstream& header,
                       const std::string& baseName,
                       const std::string& className,
                       const std::string& fieldList)
{
    header << "class " << className << " : public " << baseName << " {\n";
    header << "public:\n";
    header << "    " << className << "(" << fieldList << ");\n";
    header << "    std::any accept(" << baseName << "Visitor& visitor) const override;\n\n";

    // Fields
    std::stringstream ss(fieldList);
    std::string item;
    while (std::getline(ss, item, ',')) {
        size_t st = item.find_first_not_of(" ");
        size_t en = item.find_last_not_of(" ");
        std::string trimmed = item.substr(st, en - st + 1);
        std::stringstream fs(trimmed);
        std::string type, name;
        fs >> type >> name;
        header << "    const " << map_type(type) << " " << name << ";\n";
    }

    header << "};\n\n";
}

// Write class implementations to CPP
void defineClassImpl(std::ofstream& cpp,
                     const std::string& baseName,
                     const std::string& className,
                     const std::string& fieldList)
{
    cpp << "#include <expr.h>\n\n";

    // Constructor
    cpp << className << "::" << className << "(" << fieldList << ")";
    std::vector<std::string> fields_raw;
    std::stringstream ss(fieldList);
    std::string item;
    while (std::getline(ss, item, ',')) {
        size_t st = item.find_first_not_of(" ");
        size_t en = item.find_last_not_of(" ");
        fields_raw.push_back(item.substr(st, en - st + 1));
    }

    if (!fields_raw.empty()) {
        cpp << " : ";
        for (size_t i = 0; i < fields_raw.size(); i++) {
            std::stringstream fs(fields_raw[i]);
            std::string type, name;
            fs >> type >> name;
            cpp << name << "(" << name << ")";
            if (i + 1 < fields_raw.size()) cpp << ", ";
        }
    }
    cpp << " {}\n\n";

    // accept()
    cpp << "std::any " << className << "::accept(" << baseName << "Visitor& visitor) const {\n";
    cpp << "    return visitor.visit" << className << "(*this);\n";
    cpp << "}\n\n";
}

// Generate header + CPP for AST
void defineAst(const std::string& includeDir,
               const std::string& srcDir,
               const std::string& baseName,
               const std::vector<std::string>& types)
{
    std::string filename = to_snake_case(baseName);
    std::string headerPath = includeDir + "/" + filename + ".h";
    std::string cppPath = srcDir + "/" + filename + ".cpp";

    std::ofstream header(headerPath);
    std::ofstream cpp(cppPath);
    if (!header.is_open() || !cpp.is_open()) {
        throw std::runtime_error("Could not open output files");
    }

    // ---- Header ----
    std::string guard = include_guard(filename);
    header << "#ifndef " << guard << "\n";
    header << "#define " << guard << "\n\n";

    header << "#include <memory>\n";
    header << "#include <string>\n";
    header << "#include <vector>\n";
    header << "#include <any>\n";
    header << "#include <token.h>\n\n";

    writeForwardDeclarations(header, types);
    defineVisitor(header, baseName, types);

    // Base class
    header << "class " << baseName << " {\n";
    header << "public:\n";
    header << "    virtual ~" << baseName << "() = default;\n\n";
    header << "    virtual std::any accept(" << baseName << "Visitor& visitor) const = 0;\n";
    header << "};\n\n";

    // ---- Subclasses ----
    for (auto t : types) {
        size_t colon = t.find(':');
        std::string className = t.substr(0, colon);
        std::string fields = t.substr(colon + 1);

        auto trim = [](std::string s) {
            size_t a = s.find_first_not_of(" ");
            size_t b = s.find_last_not_of(" ");
            return s.substr(a, b - a + 1);
        };

        std::string trimmedName = trim(className);
        std::string trimmedFields = trim(fields);

        defineClassHeader(header, baseName, trimmedName, trimmedFields);
        defineClassImpl(cpp, baseName, trimmedName, trimmedFields);
    }

    header << "#endif // " << guard << "\n";
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cout << "Usage: generate_ast <include_dir> <src_dir>\n";
        return 64;
    }

    std::string include_dir = argv[1];
    std::string src_dir = argv[2];

    defineAst(include_dir, src_dir, "Expr", {
        "Binary   : std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right",
        "Grouping : std::shared_ptr<Expr> expression",
        "Literal  : std::any value",
        "Unary    : Token op, std::shared_ptr<Expr> right"
    });

    return 0;
}
