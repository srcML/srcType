#include <srcTypeHandler.hpp>
namespace srcTypeNS{
    void srcTypeHandler::GetTypedefNamePair(){
        //std::cerr<<ActualNameTNamePair.first<< " "<<lineNum<<std::endl;
    }
    void srcTypeHandler::GetParamTypeNamespace(){
        currentNameProfile.namespacename = currentParamType.first;
        //std::cerr<<"param type namespace: "<<currentNameProfile.namespacename<<std::endl;
        currentNameProfile.linenumber = lineNum;
        currentParamType.first.clear();
    }
    void srcTypeHandler::GetParamType(){
        //Get param types
        currentNameProfile.type = currentParamType.first;
        //std::cerr<<"param type: "<<currentNameProfile.type<<std::endl;
        currentNameProfile.linenumber = lineNum;
        if(cppPrimTypes.find(currentParamType.first) != cppPrimTypes.end()){
            currentNameProfile.category = primitive;
        }else{
            currentNameProfile.category = userdefined;
        }
        currentParamType.first.clear();
    }
    void srcTypeHandler::GetParamName(){
        if(currentSpecifier == "const"){
            currentNameProfile.isConst = true;
        }
        //Get Param names
        currentNameProfile.name = currentParam.first;
        currentNameProfile.linenumber = lineNum;
        //std::cerr<<"param name: "<<currentParam.first<<std::endl;
        currentParam.first.clear();
    }
    void srcTypeHandler::GetFunctionReturnTypeNamespace(){
        //std::cerr<<"function namespace: "<<currentFunctionReturnType.first<<std::endl;
        currentScopeProfile.returnTypeNamespace = currentFunctionReturnType.first;
        currentFunctionReturnType.first.clear();
    }
    void srcTypeHandler::GetFunctionReturnType(){
        //std::cerr<<"function Type: "<<currentFunctionReturnType.first<<std::endl;
        if(currentSpecifier == "const"){
            currentScopeProfile.hasConstReturn = true;
        }
        currentScopeProfile.returnType = currentFunctionReturnType.first;
        if(cppPrimTypes.find(currentFunctionReturnType.first) != cppPrimTypes.end()){
            currentScopeProfile.category = primitive;
        }else{
            currentScopeProfile.category = userdefined;
        }
        currentFunctionReturnType.first.clear();
    }
    void srcTypeHandler::GetFunctionNameResolution(){
        //std::cerr<<"function Name resultion: "<<currentFunctionBody.first<<std::endl;
        currentScopeProfile.fnNamespace = currentFunctionBody.first;
        currentFunctionBody.first.clear();
    }
    void srcTypeHandler::GetFunctionName(){
        //get function name
        //std::cerr<<"function Name: "<<currentFunctionBody.first<<std::endl;
        currentScopeProfile.name = currentFunctionBody.first;
        if(triggerField[classn]){
            currentScopeProfile.isMethod = true;
        }
        currentFunctionBody.first.clear();
    }
    void srcTypeHandler::GetConstructorNameResolution(){
        //std::cerr<<"function Name resultion: "<<currentFunctionBody.first<<std::endl;
        currentScopeProfile.fnNamespace = currentConstructor.first;
        currentConstructor.first.clear();
    }
    void srcTypeHandler::GetConstructorName(){
        //get function name
        //std::cerr<<"function Name: "<<currentFunctionBody.first<<std::endl;
        currentScopeProfile.name = currentConstructor.first;
        currentConstructor.first.clear();
    }
    void srcTypeHandler::GetDeclStmtNamespace(){
        currentNameProfile.namespacename = currentDeclType.first;
        //std::cerr<<"decl namespace: "<<currentDeclType.first<<std::endl;
        currentNameProfile.linenumber = lineNum;
        currentDeclType.first.clear();
    }
    void srcTypeHandler::GetTypeName(){
        if(currentNameProfile.type.empty()){
            currentNameProfile.type = currentDeclType.first;
        }
        //std::cerr<<"decl type: "<<currentDeclType.first<<std::endl;
        currentNameProfile.linenumber = lineNum;
        if(cppPrimTypes.find(currentNameProfile.type) != cppPrimTypes.end()){
            currentNameProfile.category = primitive;
        }else{
            currentNameProfile.category = userdefined;
        }
        currentDeclType.first.clear();
    }
    void srcTypeHandler::GetDeclStmtName(){
        if(currentSpecifier == "const"){
            currentNameProfile.isConst = true;
        }
        currentNameProfile.name = currentDecl.first;
        currentNameProfile.linenumber = lineNum;
        currentDecl.first.clear();
    }
    void srcTypeHandler::GetClassLevelTypeName(){
        classNameProfile.type = currentDeclType.first;
        if(cppPrimTypes.find(currentDeclType.first) != cppPrimTypes.end()){
            classNameProfile.category = primitive;
        }else{
            classNameProfile.category = userdefined;
        }
        currentDeclType.first.clear();
    }
    void srcTypeHandler::GetClassLevelDeclName(){
        classNameProfile.name = currentDecl.first;
        classNameProfile.linenumber = lineNum;
        classNameProfile.classMember = true;
        if(triggerField[modifier]){
            classNameProfile.alias =  true;
        }
        cvmIt->second.vtMap.insert(std::make_pair(currentDecl.first+std::to_string(lineNum), classNameProfile));
        classNameProfile.clear();
        currentDecl.first.clear();
    }

}