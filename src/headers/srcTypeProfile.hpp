#include <unordered_map>
#include <string>

struct FunctionProfile{
	public:
		std::string name;
		std::string returnType;
		std::string namespacename;
};
struct NameProfile{
	public:
		int linenumber;
		short int category;
		std::string type;
		std::string name;
		std::string namespacename;
		bool classMember;
};
struct FunctionProfileHash {
public:
  std::size_t operator()(const FunctionProfile& x) const
  {
    return std::hash<std::string>()(x.namespacename + x.name); //TODO: This doesn't seem like it should work well
  }
};

typedef std::unordered_map<FunctionProfile, NameProfile, FunctionProfileHash> TypeNameMap;
