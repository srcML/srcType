#include <cassert>
#include <srcml.h>
#include <srcSAXHandler.hpp>
#include <SideEffectPolicy.hpp>
#include <srcSAXEventDispatcher.hpp>
inline char* TrimFromEnd(char *s, size_t len){
    for (int i = len - 1; i > 0; --i){
        if (s[i] != '>'){
            s[i] = 0;
        }else{
            return s;
        }
    }
    return nullptr;
}
std::string StringToSrcML(std::string str){
	struct srcml_archive* archive;
	struct srcml_unit* unit;
	size_t size = 0;

	char *ch = 0;

	archive = srcml_archive_create();
	srcml_archive_enable_option(archive, SRCML_OPTION_POSITION);
	srcml_archive_write_open_memory(archive, &ch, &size);

	unit = srcml_unit_create(archive);
	srcml_unit_set_language(unit, SRCML_LANGUAGE_CXX);
	srcml_unit_set_filename(unit, "testsrcType.cpp");

	srcml_unit_parse_memory(unit, str.c_str(), str.size());
	srcml_archive_write_unit(archive, unit);
	
	srcml_unit_free(unit);
	srcml_archive_close(archive);
	srcml_archive_free(archive);
	TrimFromEnd(ch, size);
	return std::string(ch);
}

class TestSideEffect : public srcSAXEventDispatch::PolicyDispatcher, public srcSAXEventDispatch::PolicyListener{
    public:
        ~TestSideEffect(){}
        TestSideEffect(std::initializer_list<srcSAXEventDispatch::PolicyListener *> listeners = {}) : srcSAXEventDispatch::PolicyDispatcher(listeners){}
        void Notify(const PolicyDispatcher * policy, const srcSAXEventDispatch::srcSAXEventContext & ctx) override {}

		void RunTestVar(std::vector<DeclData> testvec){
			auto id = testvec.at(0);
			if(id.nameOfIdentifier == "j"){
				assert(id.hasSideEffect == true);
			}
			if(id.nameOfIdentifier == "y"){
				assert(id.hasSideEffect == true);
			}
			if(id.nameOfIdentifier == "i"){
				assert(id.hasSideEffect == true);
			}
			if(id.nameOfIdentifier == "k"){
				assert(id.hasSideEffect == false);
			}
			if(id.nameOfIdentifier == "y2"){
				assert(id.hasSideEffect == true);
			}
			if(id.nameOfIdentifier == "u"){
				assert(id.hasSideEffect == false);
			}
			if(id.nameOfIdentifier == "x"){
				assert(id.hasSideEffect == false);
			}
			if(id.nameOfIdentifier == "q"){
				assert(id.hasSideEffect == false);
			}
		}
		void RunTestFunc(std::vector<SignatureData> testvec){
			auto id = testvec.at(0);
			if(id.name == "foo2"){
				assert(id.hasSideEffect == false);
			}
			if(id.name == "bar"){
				assert(id.hasSideEffect == true);
			}
			if(id.name == "foobar"){
				assert(id.hasSideEffect == false);
			}
			if(id.name == "barfoo"){
				assert(id.hasSideEffect == true);
			}
			if(id.name == "barfoo2"){
				assert(id.hasSideEffect == true);
			}
		}
    protected:
        void * DataInner() const override {
            return (void*)0; //To silence the warning
        }
    private:
};

int main(int argc, char** filename){
	std::string codestr =
	"class object{\n\
		int x;\n\
		Foo y;\n\
		Obj blee;\n\
		std::string foo(int i, double j, const obj* r, Object q){\n\
			object y2 = x;\n\
			r = 0;\n\
			object u;\n\
			j = 0;\n\
			float k;\n\
			y = k - 1;\n\
			i = j + k;\n\
			foo(abc+doreme);\n\
			return y;\n\
		}\n\
		Object barfoo2(){blee = 3;}\n\
	};\n\
    int foo2(int ab) const{return ab;}\n\
    int bar(int pop){pop = 0; return pop;}\n\
    Object foobar(int wop)const{return wop;}\n\
    Object barfoo(){obj a = 5; return a;}";

	std::string srcmlstr = StringToSrcML(codestr);
	auto dictionary = new srcTypeNS::srcType(srcmlstr, false);
	std::cerr<<srcmlstr<<std::endl;

	TestSideEffect setest;

    SideEffectPolicy* sepolicy = new SideEffectPolicy(dictionary);
    srcSAXController control(srcmlstr);
    srcSAXEventDispatch::srcSAXEventDispatcher<> handler {sepolicy};
    control.parse(&handler); //Start parsing

    for(auto data : dictionary->data.variableMap){
    	setest.RunTestVar(data.second);
    }
    for(auto data : dictionary->data.paramMap){
    	setest.RunTestVar(data.second);
    }
    for(auto data : dictionary->data.functionMap){
    	setest.RunTestFunc(data.second);
    }
}