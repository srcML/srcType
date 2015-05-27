/**
 * @file element_count.hpp
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

#include <srcTypeHandler.hpp>
#include <srcSAXController.hpp>
#include <iostream> 
/**
 * main
 * @param argc number of arguments
 * @param argv the provided arguments (array of C strings)
 * 
 * Invoke srcSAX handler to count element occurences and print out the resulting element counts.
 */ 

 std::string OutputQuotedString(std::string str){
  return "\""+str+"\"";
 }
 std::string ConstructNameProfileCall(int linenumber, short int category, std::string type, std::string name, std::string namespacename, bool classMember){
    return "NameProfile("+OutputQuotedString(std::to_string(linenumber))+","+OutputQuotedString(std::to_string(category))+","+OutputQuotedString(type)+","+OutputQuotedString(name)+","+OutputQuotedString(namespacename)+","+OutputQuotedString(std::to_string(classMember))+")";
 }
 std::string ConstructFunctionProfileCall(std::string name, std::string fnNamespace, std::string returnType, std::string returnTypeNamespace){
    return "FunctionProfile("+OutputQuotedString(name)+","+OutputQuotedString(fnNamespace)+","+OutputQuotedString(returnType)+","+OutputQuotedString(returnTypeNamespace)+",";
 }
 template <typename T>
 std::string SerializeMapToString(T mp){
    std::string str;
    str=str+"{";
    for(typename T::const_iterator fvmIt = mp.begin(); fvmIt != mp.end(); ++fvmIt){
      str=str+"{"+OutputQuotedString(fvmIt->first)+","+SerializeMapToString(fvmIt)+"},"+"\n";
    }
    str.erase(str.size()-2);
    str=str+"};";
    return str;
 }
std::string SerializeMapToString(VarTypeMap::const_iterator pr){
  return ConstructNameProfileCall(pr->second.linenumber,pr->second.category,pr->second.type,pr->second.name,pr->second.namespacename,pr->second.classMember);
}
std::string SerializeMapToString(FunctionVarMap::const_iterator pr){
  std::string str = ConstructFunctionProfileCall(pr->second.name,pr->second.fnNamespace,pr->second.returnType,pr->second.returnTypeNamespace);
  str = str+"{";
  for(VarTypeMap::const_iterator p = pr->second.vtMap.begin(); p!=pr->second.vtMap.end(); ++p){
      str = str+"\n{"+OutputQuotedString(p->first)+","+SerializeMapToString(p)+"},";
  }
  str.erase(str.size()-1);
  str=str+"})";
  return str;
}

 void OutputDict(const srcTypeHandler& handler){
  std::string str;
    //auto fileNameIt = handler.sysDict.fileTable.find(ffvmIt->first);
    //if(fileNameIt != handler.sysDict.fileTable.end())
    for(FunctionVarMap::const_iterator fvmIt = handler.tDict.fvMap.begin(); fvmIt != handler.tDict.fvMap.end(); ++fvmIt){
      
      //std::cerr<<fvmIt->second.name+","<<std::endl;
      for(VarTypeMap::const_iterator vmIt = fvmIt->second.vtMap.begin(); vmIt != fvmIt->second.vtMap.end(); ++vmIt){
        std::cerr<<vmIt->second.name+","<<std::endl;
      }
      std::cerr<<")";
      std::cerr<<"-------------------------"<<std::endl;
    }
  }
void OutputHppFile(std::string hpp, std::string mapdat){
}
/*
  Type Resolution tool
  Def Use Tool as separate thing (same as type res?)
  methods
  statement #
  Consider output to srcML
  */
std::unordered_map<std::string, std::function<void()>> srcTypeHandler::process_map;
std::unordered_map<std::string, std::function<void()>> srcTypeHandler::process_map2;
int main(int argc, char * argv[]) {

  if(argc < 2) {
    std::cerr << "Useage: element_count input_file.xml\n";
    exit(1);
  }
  srcSAXController control(argv[1]);
  srcTypeHandler handler;
  control.parse(&handler);
  std::string serializedMap = "FunctionVarMap srcTypeMap = "+SerializeMapToString(handler.tDict.fvMap);
  std::cerr<<hppfile;
  std::cerr<<serializedMap;
  return 0;
}