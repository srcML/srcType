/**
 * @file srcSliceHandler.hpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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

#ifndef INCLUDED_SRCTYPEHANDLER_HPP
#define INCLUDED_SRCTYPEHANDLER_HPP

#include <srcSAXHandler.hpp>
#include <srcTypeProfile.hpp>
#include <functional>
#include <iostream>

class srcTypeHandler : public srcSAXHandler {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    typedef std::pair<std::string, unsigned int> NameLineNumberPair;
    public:
    enum ParserState {decl, expr, param, decl_stmt, expr_stmt, parameter_list, 
        argument_list, argument_list_template, call, templates, ctrlflow, endflow, 
        name, function, functiondecl, constructor, constructordecl, destructordecl, destructor,
        argument, index, block, type, init, op, literal, modifier, member_list, classn,
        preproc, whileloop, forloop, ifcond, nonterminal, empty, 
        MAXENUMVALUE = empty};
    
    //TypeNameMap tnMap;
    std::vector<unsigned short int> triggerField;

    NameLineNumberPair currentDecl;
    NameLineNumberPair currentDeclType;
    NameLineNumberPair currentParam;
    NameLineNumberPair currentParamType;
    NameLineNumberPair currentFunctionBody;
    NameLineNumberPair currentFunctionReturnType;

    NameProfile currentNameProfile;
    FunctionProfile currentFunctionProfile;
    
    TypeDictionary tDict;
    VarTypeMap::iterator vtmIt;
    FunctionVarMap::iterator fvmIt;
    
    bool attributeFound;

    static std::unordered_map< std::string, std::function<void()>> process_map;
    static std::unordered_map< std::string, std::function<void()>> process_map2;
    
    void GetTypeName();
    void GetDeclStmtName();
    void GetDeclStmtNamespace();

    void GetFunctionReturnType();
    void GetFunctionReturnTypeNamespace();
    void GetFunctionName();
    void GetFunctionNameResolution();
    
    void GetParamName();
    void GetParamType();
    void GetParamTypeNamespace();

    srcTypeHandler(){
            //fvmIt = tDict.fvMap.insert(std::make_pair("Global", FunctionProfile())).first;
            triggerField = std::vector<unsigned short int>(MAXENUMVALUE, 0);
            attributeFound = false;
            process_map = {
                {"decl_stmt", [this](){
                    ++triggerField[decl_stmt];
                } },
                { "expr_stmt", [this](){
                    ++triggerField[expr_stmt];
                } },
                { "parameter_list", [this](){
                    ++triggerField[parameter_list];
                } },
                { "if", [this](){
                    ++triggerField[ifcond];
                } },
                { "for", [this](){
                    ++triggerField[forloop];
                } },
                { "while", [this](){
                    ++triggerField[whileloop];
                } },
                { "template", [this](){
                    ++triggerField[templates];
                } },
                { "argument_list", [this](){
                    //So template arguments can be caught for types; attribute found is true if the argument list is for templates
                    if(!attributeFound){
                        ++triggerField[argument_list];
                    }else{
                        ++triggerField[argument_list_template];                        
                    }
                } },
                { "call", [this](){
                    ++triggerField[call];
                } },
                { "function", [this](){
                    ++triggerField[function];
                } },
                { "constructor", [this](){
                    ++triggerField[constructor];
                } },
                { "function_decl", [this](){
                    ++triggerField[functiondecl];
                } },
                { "destructor_decl", [this](){
                    ++triggerField[destructordecl];
                } },
                { "constructor_decl", [this](){
                    ++triggerField[constructordecl];
                } },
                { "class", [this](){
                    ++triggerField[classn];
                } },
                { "destructor", [this](){
                    ++triggerField[destructor];
                } },
                { "parameter", [this](){
                    ++triggerField[param];
                } },                
                { "member_list", [this](){
                    ++triggerField[member_list];
                } },
                { "index", [this](){
                    ++triggerField[index];
                } },
                { "operator", [this](){
                    /*Need to do some processing at the beginning of operator so that name resolution (::) in type names and function names can be properly parsed
                    without the need to do any string processing. Basically, when we see ::, save the namespace and throw out the :: by simply ignoring it*/
                    ++triggerField[op];
                    if(triggerField[parameter_list] && triggerField[param] && triggerField[decl] && triggerField[type] && !(triggerField[block] || triggerField[templates])){
                        GetParamTypeNamespace();
                    }
                    if((triggerField[type] && triggerField[decl_stmt] && !triggerField[modifier])){
                        GetDeclStmtNamespace();
                        //GetFunctionMetadata();
                    }
                    //Get Function name
                    if(triggerField[function] && triggerField[type] && !(triggerField[block] || triggerField[argument_list] || triggerField[templates] || triggerField[parameter_list])){
                        GetFunctionReturnTypeNamespace(); //split into functions for param, return and paramtype or something
                    }
                    if(triggerField[function] && !(triggerField[block] || triggerField[templates] || triggerField[parameter_list] || triggerField[type] || triggerField[argument_list])){
                        GetFunctionNameResolution();
                    }
                } },
                { "block", [this](){ 
                    if(triggerField[function] & !triggerField[block]){
                        fvmIt = tDict.fvMap.insert(std::make_pair(currentFunctionBody.first, FunctionProfile())).first;
                    }
                    ++triggerField[block];
                } },
                { "init", [this](){
                    ++triggerField[init];
                } },
                { "argument", [this](){
                    ++triggerField[argument];
                } },
                { "literal", [this](){
                    ++triggerField[literal];
                } },
                { "modifier", [this](){
                    ++triggerField[modifier];
                } },
                { "decl", [this](){
                    ++triggerField[decl]; 
                } },
                { "type", [this](){
                    ++triggerField[type]; 
                } },                
                { "expr", [this](){
                    ++triggerField[expr];
                } },
                { "name", [this](){
                    ++triggerField[name];
                } },
            };
            process_map2 = {
                {"decl_stmt", [this](){
                    //vtmIt = fvmIt->second.vtMap.insert(std::make_pair(currentNameProfile.name, currentNameProfile)).first;
                    if(!currentNameProfile.name.empty()){
                        vtmIt = currentFunctionProfile.vtMap.insert(std::make_pair(currentNameProfile.name, currentNameProfile)).first;
                    }
                    currentNameProfile.clear();
                    --triggerField[decl_stmt];
                } },             
                { "expr_stmt", [this](){
                    --triggerField[expr_stmt];
                } },            
                { "parameter_list", [this](){
                    --triggerField[parameter_list];
                } },            
                { "if", [this](){
                    --triggerField[ifcond];
                } },            
                { "for", [this](){
                    --triggerField[forloop];
                } },            
                { "while", [this](){
                    --triggerField[whileloop];
                } },
                { "template", [this](){
                    --triggerField[templates];
                } },            
                { "argument_list", [this](){
                    //So template arguments can be caught for types; attribute found is true if the argument list is for templates
                    if(!attributeFound){
                        --triggerField[argument_list];
                    }else{
                        attributeFound = false;
                        --triggerField[argument_list_template];                        
                    }
                } },            
                { "call", [this](){
                    --triggerField[call];
                } },            
                { "function", [this](){
                    //Get Function name at the end of a function. Means we can't assume we have it stored until function closes.
                    if(triggerField[function]){
                        GetFunctionName(); //split into functions for param, return and paramtype or something
                    }
                    fvmIt->second = currentFunctionProfile;
                    currentFunctionProfile.clear();
                    --triggerField[function];
                } },
                { "constructor", [this](){
                    --triggerField[constructor];
                } },            
                { "destructor", [this](){
                    --triggerField[destructor];
                } },
                { "function_decl", [this](){
                    --triggerField[functiondecl];
                } },
                { "constructor_decl", [this](){
                    --triggerField[constructordecl];
                } },
                { "destructor_decl", [this](){
                    --triggerField[destructordecl];
                } },            
                { "class", [this](){
                    --triggerField[classn];
                } },
                { "parameter", [this](){
                    /*Similar to the processing done at the end of decl; we want all of the data in the param tag so wait until it closes*/
                    if(triggerField[parameter_list] && triggerField[param] && !(triggerField[type] || triggerField[block] || triggerField[templates])){
                        GetParamName();
                    }
                    if(!currentNameProfile.name.empty()){
                        vtmIt = currentFunctionProfile.vtMap.insert(std::make_pair(currentNameProfile.name, currentNameProfile)).first;
                    }
                    currentNameProfile.clear();
                    --triggerField[param];
                } },    
                { "member_list", [this](){
                    --triggerField[member_list];
                } },    
                { "index", [this](){
                    --triggerField[index];
                } },    
                { "operator", [this](){
                    --triggerField[op];
                } },
                { "block", [this](){ 
                    --triggerField[block];
                } },    
                { "init", [this](){
                    --triggerField[init];
                } },    
                { "argument", [this](){
                    --triggerField[argument];
                } },    
                { "literal", [this](){
                    --triggerField[literal];
                } },    
                { "modifier", [this](){
                    --triggerField[modifier];
                } },    
                { "decl", [this](){
                    /*At the end of decl we've definitely seen the name. Have to constrain a bit hard to make sure nothing else comes in but this
                    allows the function to only be called once.*/
                    if(triggerField[decl_stmt]){
                        GetDeclStmtName();
                    }
                    --triggerField[decl]; 
                } },    
                { "type", [this](){
                    /*At the end of a type tag in a decl, parameter, or function we can pick up type*/
                    if((triggerField[type] && triggerField[decl_stmt] && !triggerField[modifier])){
                        GetTypeName();
                    }
                    if(triggerField[type] && triggerField[parameter_list] && triggerField[param] && triggerField[decl] && !(triggerField[block] || triggerField[templates])){
                        GetParamType();
                    }
                    if(triggerField[function] && triggerField[type] && !(triggerField[block] || triggerField[argument_list] || triggerField[templates] || triggerField[parameter_list])){
                        GetFunctionReturnType();
                    }
                    --triggerField[type];
                } },    
                { "expr", [this](){
                    --triggerField[expr];
                } },    
                { "name", [this](){
                    if(triggerField[decl_stmt] && (triggerField[name] || triggerField[op]) && 
                        triggerField[decl] && !(triggerField[index] || triggerField[preproc] || triggerField[type] || triggerField[init])){
                        //GetDeclStmtName();
                    }
                    --triggerField[name];
                } },
            };
    }

    /*
    virtual void startDocument() {}
    virtual void endDocument() {}
    */

    /**
     * startRoot
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param nb_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param nb_attributes the number of attributes on the tag
     * @param nb_defaulted the number of defaulted attributes
     * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
     *
     * SAX handler function for start of the root element.
     * Counts the root unit (if an archive, to avoid double count with startUnit).
     * Overide for desired behaviour.
     */
    virtual void startRoot(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {
        
        //push_element(localname, prefix);
    }
    /**
     * startUnit
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param nb_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param nb_attributes the number of attributes on the tag
     * @param nb_defaulted the number of defaulted attributes
     * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
     *
     * SAX handler function for start of an unit.
     * Counts each unit tag (= filecount non-archive, = filecount + 1 if archive).
     * Overide for desired behaviour.
     */
    virtual void startUnit(const char * localname, const char * prefix, const char * URI,
                           int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                           const struct srcsax_attribute * attributes) {

    }
    /**
     * startElementNs
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param nb_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param nb_attributes the number of attributes on the tag
     * @param nb_defaulted the number of defaulted attributes
     * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
     *
     * SAX handler function for start of an element.
     * Overide for desired behaviour.
    */
    virtual void startElement(const char * localname, const char * prefix, const char * URI,
                                int num_namespaces, const struct srcsax_namespace * namespaces, int num_attributes,
                                const struct srcsax_attribute * attributes) {
            std::string lname(localname);
            std::unordered_map<std::string, std::function<void()>>::const_iterator process = process_map.find(lname);
            if (process != process_map.end()){
                if(num_attributes && std::string(attributes[0].value) == "template"){
                    attributeFound = true;
                }
                process->second();
            }else{
                ++triggerField[nonterminal];
            }

    }
    /**
     * charactersUnit
     * @param ch the characers
     * @param len number of characters
     *
     * SAX handler function for character handling within a unit.
     * Overide for desired behaviour.
     */
    virtual void charactersUnit(const char * ch, int len) {
        if(triggerField[decl] && triggerField[decl_stmt] && (triggerField[name] || triggerField[op]) && !(triggerField[argument_list] 
            || triggerField[index] || triggerField[preproc] || triggerField[type] || triggerField[init])) {
            currentDecl.first.append(ch,len);
        }
        if((triggerField[type] && triggerField[decl_stmt] && !(triggerField[argument_list_template] || triggerField[modifier] || triggerField[op]))){
            currentDeclType.first.append(ch,len);
        }
        if(((triggerField[function] || triggerField[functiondecl]) && triggerField[name]  && triggerField[parameter_list] && triggerField[param])
            && !(triggerField[type] || triggerField[templates] || triggerField[argument_list])){
            currentParam.first.append(ch, len);
        }
        if(((triggerField[function] || triggerField[functiondecl]) && triggerField[name]  && triggerField[parameter_list] && triggerField[param]) && triggerField[type]
         && !(triggerField[templates] || triggerField[op] || triggerField[argument_list_template])){
            currentParamType.first.append(ch, len);
        }
        if((triggerField[function] && triggerField[name]) 
         && !(triggerField[argument_list] || triggerField[argument_list_template] || triggerField[block] || triggerField[type]
         || triggerField[parameter_list] || triggerField[index] || triggerField[preproc] || triggerField[op])){
            
            currentFunctionBody.first.append(ch, len);
        }
        if(triggerField[function] && triggerField[type] 
            && !(triggerField[op] || triggerField[block] || triggerField[argument_list] || triggerField[argument_list_template] || triggerField[templates] || triggerField[parameter_list])){
            currentFunctionReturnType.first.append(ch, len);
        }
    }

    // end elements may need to be used if you want to collect only on per file basis or some other granularity.
    virtual void endRoot(const char * localname, const char * prefix, const char * URI) {

    }
    virtual void endUnit(const char * localname, const char * prefix, const char * URI) {

    }

    virtual void endElement(const char * localname, const char * prefix, const char * URI) {
        std::string lname(localname);
        std::unordered_map<std::string, std::function<void()>>::const_iterator process2 = process_map2.find(lname);            
        if (process2 != process_map2.end()) {
            process2->second();
        }else{
            --triggerField[nonterminal]; 
        }
    }
#pragma GCC diagnostic pop

};

#endif