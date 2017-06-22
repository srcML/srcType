/**
 * @file srcType.hpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef SRCTYPE_HPP
#define SRCTYPE_HPP

#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
#include <stdexcept>
#include <iostream>

namespace srcTypeNS{
    class srcType{
        public:
            unsigned int currentline;
            std::string currentfilename;
            std::string currentfunctionname;
            srcTypeData data;
            srcType();
            srcType(const char*, const char*);
            srcType(std::string, const char*);
            srcType(FILE*, const char*);
            srcType(int, const char*);
            void ReadArchiveFile(std::string);
            bool IsPrimitive(std::string type){
                return cppPrimTypes.find(type) != cppPrimTypes.end();
            }
            bool SetContext(std::string file, std::string function){
                currentfilename = file;
                currentfunctionname = function;
                return true;
            }
            //Assumes context was set
            std::vector<DeclTypePolicy::DeclTypeData> FindVariable(std::string varName) const{
                auto it = data.variableMap.find(currentfilename + currentfunctionname + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Could not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName + "\n");
                }
                return std::vector<DeclTypePolicy::DeclTypeData>();
            }

            //Does not use context
            std::vector<DeclTypePolicy::DeclTypeData> FindVariable(std::string varName, std::string functionName, std::string fileName) const{
                auto it = data.variableMap.find(fileName + functionName + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Could not find variable with key: file: " + fileName+" func: "+functionName+" Id: "+varName + "\n");
                }
                return std::vector<DeclTypePolicy::DeclTypeData>();
            }

            //Does not use context
            std::vector<ParamTypePolicy::ParamData> FindParam(std::string varName, std::string functionName) const{
                auto it = data.paramMap.find(functionName + varName);
                if(it != data.paramMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Could not find parameter with key: " + functionName+" Name: "+varName + "\n");
                }
                return std::vector<ParamTypePolicy::ParamData>();
            }
            

            std::vector<FunctionSignaturePolicy::SignatureData> FindFunction(std::string funcName, int numParams) {
                std::vector<FunctionSignaturePolicy::SignatureData> resultVec;
                auto it = data.functionMap.find(funcName);
                if(it != data.functionMap.end()){
                    for(auto func : it->second){
                        if(func.parameters.size() == numParams){
                            resultVec.push_back(func);
                        }
                    }
                    return resultVec;
                }else{
                    throw std::runtime_error("Could not find function with key: " + funcName + "\n");
                }
                return std::vector<FunctionSignaturePolicy::SignatureData>();
            }

            std::vector<FunctionSignaturePolicy::SignatureData> FindFunction(std::string funcName, const std::vector<DeclTypePolicy::DeclTypeData>& callParams) {
                std::vector<FunctionSignaturePolicy::SignatureData> resultVec;
                auto it = data.functionMap.find(funcName);
                std::string callparamtypes, currentparamtypes;
                if(it != data.functionMap.end()){
                    for(auto func : it->second){
                        if(func.parameters.size() == callParams.size()){
                            for(unsigned int i = 0; i< func.parameters.size(); ++i){
                                callparamtypes += callParams.at(i).nameoftype;
                                currentparamtypes+=func.parameters.at(i).nameoftype;
                            }
                            if(callparamtypes == currentparamtypes){
                                resultVec.push_back(func);
                            }
                            callparamtypes.clear();
                            currentparamtypes.clear();
                        }
                    }
                    return resultVec;
                }else{
                    throw std::runtime_error("Could not find function with key: " + funcName + "\n");
                }
                return std::vector<FunctionSignaturePolicy::SignatureData>();
            }
    };
}
#endif