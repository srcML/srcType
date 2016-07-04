#include <srcTypeHandler.hpp>
namespace srcTypeNS{
    void srcTypeHandler::GetTypedefNamePair(){
        //std::cerr<<ActualNameTNamePair.first<< " "<<lineNum<<std::endl;
    }
    void srcTypeHandler::GetParamTypeNamespace(){
        currentVariableProfile.varNamespace = currentParamType.first;
        //std::cerr<<"param type namespace: "<<currentVariableProfile.namespacename<<std::endl;
        currentVariableProfile.linenumber = lineNum;
        currentParamType.first.clear();
    }
    void srcTypeHandler::GetParamType(){
        //Get param types
        currentVariableProfile.type = currentParamType.first;
        //std::cerr<<"param type: "<<currentVariableProfile.type<<std::endl;
        currentVariableProfile.linenumber = lineNum;
        if(cppPrimTypes.find(currentParamType.first) != cppPrimTypes.end()){
            currentVariableProfile.isPrimitive = true;
        }else{
            currentVariableProfile.isPrimitive = false;
        }
        currentParamType.first.clear();
    }
    void srcTypeHandler::GetParamName(){
        if(currentSpecifier == "const"){
            currentVariableProfile.isConst = true;
        }
        //Get Param names
        currentVariableProfile.name = currentParam.first;
        currentVariableProfile.linenumber = lineNum;
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
        if(currentSpecifier == "const"){
            currentFunctionProfile.hasConstReturn = true;
        }
        currentFunctionProfile.returnType = currentFunctionReturnType.first;
        if(cppPrimTypes.find(currentFunctionReturnType.first) != cppPrimTypes.end()){
            currentFunctionProfile.isPrimitive = true;
        }else{
            currentFunctionProfile.isPrimitive = false;
        }
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
        if(triggerField[classn]){
            currentFunctionProfile.isMethod = true;
        }
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
        currentVariableProfile.varNamespace = currentDeclType.first;
        //std::cerr<<"decl namespace: "<<currentDeclType.first<<std::endl;
        currentVariableProfile.linenumber = lineNum;
        currentDeclType.first.clear();
    }
    void srcTypeHandler::GetTypeName(){
        if(currentVariableProfile.type.empty()){
            currentVariableProfile.type = currentDeclType.first;
        }
        //std::cerr<<"decl type: "<<currentDeclType.first<<std::endl;
        currentVariableProfile.linenumber = lineNum;
        if(cppPrimTypes.find(currentVariableProfile.type) != cppPrimTypes.end()){
            currentVariableProfile.isPrimitive = true;
        }else{
            currentVariableProfile.isPrimitive = false;
        }
        currentDeclType.first.clear();
    }
    void srcTypeHandler::GetDeclStmtName(){
        if(currentSpecifier == "const"){
            currentVariableProfile.isConst = true;
        }
        currentVariableProfile.name = currentDecl.first;
        currentVariableProfile.linenumber = lineNum;
        currentDecl.first.clear();
    }
    void srcTypeHandler::GetClassLevelTypeName(){
        classNameProfile.type = currentDeclType.first;
        if(cppPrimTypes.find(currentDeclType.first) != cppPrimTypes.end()){
            classNameProfile.isPrimitive = true;
        }else{
            classNameProfile.isPrimitive = false;
        }
        currentDeclType.first.clear();
    }
    void srcTypeHandler::GetClassLevelDeclName(){
        classNameProfile.name = currentDecl.first;
        classNameProfile.linenumber = lineNum;
        classNameProfile.classMember = true;
        if(triggerField[modifier]){
            classNameProfile.isAlias =  true;
        }
        if(cvmIt != tDict->fvMap.end()){  
            cvmIt->second.vtMap.insert(std::make_pair(currentDecl.first+std::to_string(lineNum), classNameProfile));
        }
        classNameProfile.clear();
        currentDecl.first.clear();
    }

}