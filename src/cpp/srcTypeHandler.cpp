#include <srcTypeHandler.hpp>

void srcTypeHandler::GetParamTypeNamespace(){
    currentNameProfile.namespacename = currentParamType.first;
    std::cerr<<"param type namespace: "<<currentNameProfile.namespacename<<std::endl;
    currentParamType.first.clear();
}
void srcTypeHandler::GetParamType(){
    //Get param types
    currentNameProfile.type = currentParamType.first;
    std::cerr<<"param type: "<<currentNameProfile.type<<std::endl;
    currentParamType.first.clear();
}
void srcTypeHandler::GetParamName(){
    //Get Param names
    currentNameProfile.name = currentParam.first;
    std::cerr<<"param name: "<<currentParam.first<<std::endl;
    currentParam.first.clear();
}
void srcTypeHandler::GetFunctionReturnTypeNamespace(){
    std::cerr<<"function namespace: "<<currentFunctionReturnType.first<<std::endl;
    currentFunctionProfile.returnTypeNamespace = currentFunctionReturnType.first;
    currentFunctionReturnType.first.clear();
}
void srcTypeHandler::GetFunctionReturnType(){
    std::cerr<<"function Type: "<<currentFunctionReturnType.first<<std::endl;
    currentFunctionProfile.returnType = currentFunctionReturnType.first;
    currentFunctionReturnType.first.clear();
}
void srcTypeHandler::GetFunctionNameResolution(){
    std::cerr<<"function Name resultion: "<<currentFunctionBody.first<<std::endl;
    currentFunctionProfile.fnNamespace = currentFunctionBody.first;
    currentFunctionBody.first.clear();
}
void srcTypeHandler::GetFunctionName(){
    //get function name
    std::cerr<<"function Name: "<<currentFunctionBody.first<<std::endl;
    currentFunctionProfile.name = currentFunctionBody.first;
    currentFunctionBody.first.clear();
}
void srcTypeHandler::GetDeclStmtNamespace(){
    std::cerr<<"decl namespace: "<<currentDeclType.first<<std::endl;
    currentDeclType.first.clear();
}
void srcTypeHandler::GetTypeName(){
    std::cerr<<"decl type: "<<currentDeclType.first<<std::endl;
    currentDeclType.first.clear();
}
void srcTypeHandler::GetDeclStmtName(){
    std::cerr<<"decl name: "<<currentDecl.first<<std::endl;
    currentDecl.first.clear();
}
