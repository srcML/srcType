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
            srcType(const char*, const bool);
            srcType(std::string, const bool);
            srcType(FILE*, const bool);
            srcType(int, const bool);
            void ReadArchiveFile(std::string);
            bool IsPrimitive(std::string type){
                return cppPrimTypes.find(type) != cppPrimTypes.end();
            }
            bool SetContext(std::string file, std::string function){
                currentfilename = file;
                currentfunctionname = function;
                return true;
            }
            //Allows side effects
            std::vector<DeclData>* FindIdentifierWrite(std::string varName, std::string functionName, std::string classname, std::string fileName){
                auto varit = data.variableMap.find(fileName + functionName + classname + varName);
                auto paramit = data.paramMap.find(functionName + varName);
                if(varit != data.variableMap.end()){
                    return &(varit->second);
                }else if (paramit != data.paramMap.end()){
                    return &(paramit->second);
                }else{
                    return nullptr;
                }
            }
            //Allows side effects
            std::vector<DeclData>* FindParameterWrite(std::string varName, unsigned int lineNumber){
                auto paramit = data.paramMap.find(varName + std::to_string(lineNumber));
                if (paramit != data.paramMap.end()){
                    return &(paramit->second);
                }else{
                    return nullptr;
                }
            }
            //Allows side effects
            std::vector<DeclData>* FindVariableWrite(std::string varName, unsigned int lineNumber){
                auto varit = data.variableMap.find(varName + std::to_string(lineNumber));
                if (varit != data.variableMap.end()){
                    return &(varit->second);
                }else{
                    return nullptr;
                }
            }

            std::vector<DeclData> FindIdentifier(std::string varName, std::string functionName, std::string classname, std::string fileName) const{
                auto varit = data.variableMap.find(fileName + functionName + classname + varName);
                auto paramit = data.paramMap.find(functionName + varName);
                if(varit != data.variableMap.end()){
                    return varit->second;
                }else if (paramit != data.paramMap.end()){
                    return paramit->second;
                }else{
                    return std::vector<DeclData>();
                }
            }
            //Assumes context was set
            std::vector<DeclData> FindVariable(std::string varName) const{
                auto it = data.variableMap.find(currentfilename + currentfunctionname + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Could not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName + "\n");
                }
                return std::vector<DeclData>();
            }

            //Does not use context
            std::vector<DeclData> FindVariable(std::string varName, std::string functionName, std::string fileName) const{
                auto it = data.variableMap.find(fileName + functionName + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Could not find variable with key: file: " + fileName+" func: "+functionName+" Id: "+varName + "\n");
                }
                return std::vector<DeclData>();
            }

            //Does not use context
            std::vector<DeclData> FindParam(std::string varName, std::string functionName) const{
                auto it = data.paramMap.find(functionName + varName);
                if(it != data.paramMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Could not find parameter with key: " + functionName+" Name: "+varName + "\n");
                }
                return std::vector<DeclData>();
            }
            

            std::vector<SignatureData> FindFunction(std::string funcName, int numParams) {
                std::vector<SignatureData> resultVec;
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
                return std::vector<SignatureData>();
            }
            std::vector<SignatureData>* FindFunctionWrite(std::string funcName, int numParams) {
                auto it = data.functionMap.find(funcName);
                if(it != data.functionMap.end()){
                    return &(it->second);
                }else{
                    throw std::runtime_error("Could not find function with key: " + funcName + "\n");
                }
                return nullptr;
            }
            typedef std::pair<double, SignatureData> FunctionProbabilityPair;
            std::vector<FunctionProbabilityPair> FindFunction(std::string funcName, const std::vector<std::string>& callParams) {
                std::vector<FunctionProbabilityPair> resultVec;
                auto it = data.functionMap.find(funcName);
                if(it != data.functionMap.end()){
                    //if(it->second.size() == 1){return it->second;}
                    for(auto func : it->second){
                        double  probability = 0, numParameters = 0, numTrue = 0;
                        if(func.parameters.size() == callParams.size()){
                            for(unsigned int i = 0; i< func.parameters.size(); ++i){
                                if(callParams.at(i) == func.parameters.at(i).nameOfType){
                                    ++numTrue;
                                }else if(stringConversionTypes.find(callParams.at(i)) != stringConversionTypes.end()){ //Current param is a string literal type
                                    if(stringConversionTypes.find(func.parameters.at(i).nameOfType) != stringConversionTypes.end()){
                                        ++numTrue; //Ok, the argument in the original definition is convertable to string, so these might match
                                    }
                                }else if (numberConversionTypes.find(callParams.at(i)) != numberConversionTypes.end()){ //Current param is a number literal type
                                    if(numberConversionTypes.find(func.parameters.at(i).nameOfType) != numberConversionTypes.end()){
                                        ++numTrue; //Ok, the argument in the original definition is convertable to some builtin number type, so these might match
                                    }
                                }
                                ++numParameters;
                                probability = numTrue/numParameters;
                            }
                            resultVec.push_back(std::make_pair(probability, func));
                        }
                    }
                    std::sort(resultVec.begin(), resultVec.end(),
                        [](FunctionProbabilityPair a, FunctionProbabilityPair b){
                            return a.first < b.first;
                        });
                    for(auto pair : resultVec){
                        std::cerr<<"Func name: "<<pair.second.name<<" probability: "<<pair.first<<std::endl;
                    }
                    return resultVec;
                }else{
                    throw std::runtime_error("Could not find function with key: " + funcName + "\n");
                }
                return std::vector<FunctionProbabilityPair>();
            }
    };
}
#endif