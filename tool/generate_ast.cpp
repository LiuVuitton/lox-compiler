#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cctype>

class GenerateAst {
public:
    static void main(int argc, char* argv[]) {
        if (argc != 2) {
            std::cerr << "Usage: generate_ast <output directory>" << std::endl;
            std::exit(64);
        }

        std::string output_dir = argv[1];

        std::vector<std::string> expr_types = {
            "Binary : std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right",
            "Grouping : std::unique_ptr<Expr> expr",
            "Literal : std::any value",
            "Unary : Token op, std::unique_ptr<Expr> right"
        };

        defineAst(output_dir, "Expr", expr_types);
    }

private:
    static void defineAst(const std::string& output_dir,
                          const std::string& base_name,
                          const std::vector<std::string>& types) {
        std::string path = output_dir + "/" + toLowerCase(base_name) + ".h";
        std::ofstream writer(path);

        if (!writer) {
            std::cerr << "Could not open file: " << path << std::endl;
            std::exit(1);
        }

        writer << "#ifndef " << toUpperCase(base_name) << "_H\n";
        writer << "#define " << toUpperCase(base_name) << "_H\n\n";

        writer << "#include <any>\n";
        writer << "#include <memory>\n";
        writer << "#include \"token.h\"\n\n";

        writer << "namespace lox {\n\n";

        // Forward declarations
        for (const auto& type : types) {
            size_t colon_pos = type.find(':');
            std::string class_name = trim(type.substr(0, colon_pos));
            writer << "class " << class_name << ";\n";
        }
        writer << "\n";

        // Base class
        writer << "class " << base_name << " {\n";
        writer << "public:\n";
        defineVisitor(writer, base_name, types);
        writer << "\n";
        writer << "    virtual ~" << base_name << "() = default;\n";
        writer << "    virtual std::any accept(Visitor& visitor) = 0;\n";
        writer << "};\n\n";

        // Subclasses
        writer << "// ---------- Expression types ----------\n";
        for (const auto& type : types) {
            size_t colon_pos = type.find(':');
            std::string class_name = trim(type.substr(0, colon_pos));
            std::string fields = trim(type.substr(colon_pos + 1));
            defineType(writer, base_name, class_name, fields);
        }

        writer << "} // namespace lox\n\n";
        writer << "#endif // " << toUpperCase(base_name) << "_H\n";

        writer.close();
    }

    static void defineVisitor(std::ofstream& writer,
                              const std::string& base_name,
                              const std::vector<std::string>& types) {
        writer << "    class Visitor {\n";
        writer << "    public:\n";
        writer << "        virtual ~Visitor() = default;\n";

        for (const auto& type : types) {
            size_t colon_pos = type.find(':');
            std::string type_name = trim(type.substr(0, colon_pos));
            std::string method_name = type_name + "Expr";
            writer << "        virtual std::any visit" << method_name
                   << "(class " << type_name << "* expr) = 0;\n";
        }

        writer << "    };\n";
    }

    static void defineType(std::ofstream& writer,
                           const std::string& base_name,
                           const std::string& class_name,
                           const std::string& field_list) {
        writer << "class " << class_name << " : public " << base_name << " {\n";
        writer << "public:\n";

        // Parse fields and types
        std::vector<std::string> field_decls = split(field_list, ", ");
        
        // Write field declarations
        for (const auto& field : field_decls) {
            writer << "    " << field << ";\n";
        }
        writer << "\n";

        // Constructor
        writer << "    " << class_name << "(";
        for (size_t i = 0; i < field_decls.size(); ++i) {
            writer << field_decls[i];
            if (i < field_decls.size() - 1) writer << ", ";
        }
        writer << ")";

        // Constructor initializer list
        if (!field_decls.empty()) {
            writer << "\n        : ";
            for (size_t i = 0; i < field_decls.size(); ++i) {
                std::string field_name = getFieldName(field_decls[i]);
                
                // Check if it's a unique_ptr and use std::move
                std::string field_type = getFieldType(field_decls[i]);
                if (field_type.find("std::unique_ptr") != std::string::npos) {
                    writer << field_name << "(std::move(" << field_name << "))";
                } else {
                    writer << field_name << "(" << field_name << ")";
                }
                
                if (i < field_decls.size() - 1) writer << ", ";
            }
        }
        writer << " {}\n\n";

        // Visitor accept method
        writer << "    std::any accept(Visitor& visitor) override {\n";
        writer << "        return visitor.visit" << class_name << "Expr(this);\n";
        writer << "    }\n";

        writer << "};\n\n";
    }

    // ----------------- Utility functions -----------------

    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(' ');
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }

    static std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> tokens;
        size_t start = 0;
        size_t end = str.find(delimiter);

        while (end != std::string::npos) {
            tokens.push_back(trim(str.substr(start, end - start)));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }
        tokens.push_back(trim(str.substr(start)));
        return tokens;
    }

    static std::string getFieldName(const std::string& field) {
        size_t last_space = field.find_last_of(' ');
        return field.substr(last_space + 1);
    }

    static std::string getFieldType(const std::string& field) {
        size_t last_space = field.find_last_of(' ');
        return trim(field.substr(0, last_space));
    }

    static std::string toUpperCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) c = std::toupper(c);
        return result;
    }

    static std::string toLowerCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) c = std::tolower(c);
        return result;
    }
};

// ----------------- Main -----------------

int main(int argc, char* argv[]) {
    GenerateAst::main(argc, argv);
    return 0;
}
