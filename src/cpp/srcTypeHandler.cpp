#include <srcTypeHandler.hpp>

void srcTypeHandler::GetParamTypeNamespace(){
    currentNameProfile.namespacename = currentParamType.first;
    //std::cerr<<"param type namespace: "<<currentNameProfile.namespacename<<std::endl;
    currentNameProfile.linenumber = currentParamType.second;
    currentParamType.first.clear();
}
void srcTypeHandler::GetParamType(){
    //Get param types
    currentNameProfile.type = currentParamType.first;
    //std::cerr<<"param type: "<<currentNameProfile.type<<std::endl;
    currentNameProfile.linenumber = currentParamType.second;
    if(cppPrimTypes.find(currentParamType.first) != cppPrimTypes.end()){
        currentNameProfile.category = primitive;
    }else{
        currentNameProfile.category = userdefined;
    }
    currentParamType.first.clear();
}
void srcTypeHandler::GetParamName(){
    //Get Param names
    currentNameProfile.name = currentParam.first;
    currentNameProfile.linenumber = currentParam.second;
    //std::cerr<<"param name: "<<currentParam.first<<std::endl;
    currentParam.first.clear();
}
void srcTypeHandler::GetFunctionReturnTypeNamespace(){
    //std::cerr<<"function namespace: "<<currentFunctionReturnType.first<<std::endl;
    currentFunctionProfile.returnTypeNamespace = currentFunctionReturnType.first;
    currentFunctionReturnType.first.clear();
}
void srcTypeHandler::GetFunctionReturnType(){
    //std::cerr<<"function Type: "<<currentFunctionReturnType.first<<std::endl;
    currentFunctionProfile.returnType = currentFunctionReturnType.first;
    currentFunctionReturnType.first.clear();
}
void srcTypeHandler::GetFunctionNameResolution(){
    //std::cerr<<"function Name resultion: "<<currentFunctionBody.first<<std::endl;
    currentFunctionProfile.fnNamespace = currentFunctionBody.first;
    currentFunctionBody.first.clear();
}
void srcTypeHandler::GetFunctionName(){
    //get function name
    //std::cerr<<"function Name: "<<currentFunctionBody.first<<std::endl;
    currentFunctionProfile.name = currentFunctionBody.first;
    currentFunctionBody.first.clear();
}
void srcTypeHandler::GetConstructorNameResolution(){
    //std::cerr<<"function Name resultion: "<<currentFunctionBody.first<<std::endl;
    currentFunctionProfile.fnNamespace = currentConstructor.first;
    currentConstructor.first.clear();
}
void srcTypeHandler::GetConstructorName(){
    //get function name
    //std::cerr<<"function Name: "<<currentFunctionBody.first<<std::endl;
    currentFunctionProfile.name = currentConstructor.first;
    currentConstructor.first.clear();
}
void srcTypeHandler::GetDeclStmtNamespace(){
    currentNameProfile.namespacename = currentDeclType.first;
    //std::cerr<<"decl namespace: "<<currentDeclType.first<<std::endl;
    currentNameProfile.linenumber = currentDeclType.second;
    currentDeclType.first.clear();
}
void srcTypeHandler::GetTypeName(){
    currentNameProfile.type = currentDeclType.first;
    //std::cerr<<"decl type: "<<currentDeclType.first<<std::endl;
    currentNameProfile.linenumber = currentDeclType.second;
    if(cppPrimTypes.find(currentDeclType.first) != cppPrimTypes.end()){
        currentNameProfile.category = primitive;
    }else{
        currentNameProfile.category = userdefined;
    }
    currentDeclType.first.clear();
}
void srcTypeHandler::GetDeclStmtName(){
    currentNameProfile.name = currentDecl.first;
    //std::cerr<<"decl name: "<<currentDecl.first<<std::endl;
    currentNameProfile.linenumber = currentDecl.second;
    currentDecl.first.clear();
}