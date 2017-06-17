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
                    throw std::runtime_error("Coud not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName + "\n");
                }
            }

            //Does not use context
            std::vector<DeclTypePolicy::DeclTypeData> FindVariable(std::string varName, std::string functionName, std::string fileName) const{
                auto it = data.variableMap.find(fileName + functionName + varName);
                if(it != data.variableMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Coud not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName + "\n");
                }
            }

            //Does not use context
            std::vector<ParamTypePolicy::ParamData> FindParam(std::string varName, std::string functionName, std::string fileName) const{
                auto it = data.paramMap.find(fileName + functionName + varName);
                if(it != data.paramMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Coud not find variable with key: " + currentfilename+" "+currentfunctionname+" "+varName + "\n");
                }
            }

            std::vector<FunctionSignaturePolicy::SignatureData> FindFunction(std::string funcName) {
                auto it = data.functionMap.find(funcName);
                std::cerr<<"Looking for: "<<funcName<<std::endl;
                if(it != data.functionMap.end()){
                    return it->second;
                }else{
                    throw std::runtime_error("Coud not find function with key: " + funcName + "\n");
                }
                return std::vector<FunctionSignaturePolicy::SignatureData>();
            }
    };
}
#endif