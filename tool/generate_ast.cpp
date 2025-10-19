#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

class GenerateAst {
public:
    static void main(int argc, char* argv[]) {
        if (argc != 2) {
            std::cerr << "Usage: generate_ast <output directory>" << std::endl;
            std::exit(64);
        }
        
        std::string outputDir = argv[1];
        
        defineAst(outputDir, "expr", {
            "Binary : Expr* left, Token operator, Expr* right",
            "Grouping : Expr* expression", 
            "Literal : std::any value",
            "Unary : Token operator, Expr* right"
        });
    }

private:
    static void defineAst(const std::string& outputDir, 
                         const std::string& baseName, 
                         const std::vector<std::string>& types) {
        std::string path = outputDir + "/" + baseName + ".h";
        std::ofstream writer(path);
        
        if (!writer) {
            std::cerr << "Could not open file: " << path << std::endl;
            std::exit(1);
        }
        
        writer << "#ifndef " << toUpperCase(baseName) << "_H" << std::endl;
        writer << "#define " << toUpperCase(baseName) << "_H" << std::endl;
        writer << std::endl;
        writer << "#include <any>" << std::endl;
        writer << "#include <memory>" << std::endl;
        writer << "#include \"token.h\"" << std::endl;
        writer << std::endl;
        writer << "namespace lox {" << std::endl;
        writer << std::endl;
        std::string className = toPascalCase(baseName);
        writer << "class " << className << " {" << std::endl;
        writer << "public:" << std::endl;
        writer << "    virtual ~" << className << "() = default;" << std::endl;
        writer << std::endl;
        
        // Define visitor interface
        defineVisitor(writer, className, types);
        
        // The base accept() method
        writer << "    template<typename R>" << std::endl;
        writer << "    virtual R accept(Visitor<R>& visitor) = 0;" << std::endl;
        writer << "};" << std::endl;
        writer << std::endl;
        
        // Generate each subclass
        for (const auto& type : types) {
            size_t colonPos = type.find(':');
            std::string className = trim(type.substr(0, colonPos));
            std::string fields = trim(type.substr(colonPos + 1));
            defineType(writer, toPascalCase(baseName), className, fields);
        }
        
        writer << "} // namespace lox" << std::endl;
        writer << std::endl;
        writer << "#endif // " << toUpperCase(baseName) << "_H" << std::endl;
        
        writer.close();
    }
    
    static void defineVisitor(std::ofstream& writer, 
                              const std::string& baseName, 
                              const std::vector<std::string>& types) {
        writer << "    template<typename R>" << std::endl;
        writer << "    class Visitor {" << std::endl;
        writer << "    public:" << std::endl;
        writer << "        virtual ~Visitor() = default;" << std::endl;
        
        for (const auto& type : types) {
            size_t colonPos = type.find(':');
            std::string typeName = trim(type.substr(0, colonPos));
            std::string paramName = toLowerCase(baseName);
            writer << "        virtual R visit" << typeName << baseName 
                   << "(" << typeName << "* " << paramName << ") = 0;" << std::endl;
        }
        
        writer << "    };" << std::endl;
        writer << std::endl;
    }
    
    static void defineType(std::ofstream& writer, 
                           const std::string& baseName,
                           const std::string& className, 
                           const std::string& fieldList) {
        writer << "class " << className << " : public " << baseName << " {" << std::endl;
        writer << "public:" << std::endl;
        
        // Constructor
        writer << "    " << className << "(" << fieldList << ")";
        
        // Constructor initialization list
        std::vector<std::string> fields = split(fieldList, ", ");
        if (!fields.empty()) {
            writer << " : ";
            for (size_t i = 0; i < fields.size(); ++i) {
                std::string fieldName = getFieldName(fields[i]);
                writer << fieldName << "(" << fieldName << ")";
                if (i < fields.size() - 1) {
                    writer << ", ";
                }
            }
        }
        writer << " {}" << std::endl;
        writer << std::endl;
        
        // Fields
        for (const auto& field : fields) {
            writer << "    " << field << ";" << std::endl;
        }
        
        // Visitor pattern
        writer << std::endl;
        writer << "    template<typename R>" << std::endl;
        writer << "    R accept(Visitor<R>& visitor) override {" << std::endl;
        writer << "        return visitor.visit" << className << baseName << "(this);" << std::endl;
        writer << "    }" << std::endl;
        writer << "};" << std::endl;
        writer << std::endl;
    }
    
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
        size_t lastSpace = field.find_last_of(' ');
        return field.substr(lastSpace + 1);
    }
    
    static std::string toUpperCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::toupper(c);
        }
        return result;
    }
    
    static std::string toPascalCase(const std::string& str) {
        if (str.empty()) return str;
        std::string result = str;
        result[0] = std::toupper(result[0]);
        return result;
    }
    
    static std::string toLowerCase(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(c);
        }
        return result;
    }
};

int main(int argc, char* argv[]) {
    GenerateAst::main(argc, argv);
    return 0;
}