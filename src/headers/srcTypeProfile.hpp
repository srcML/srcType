/**
 * @file srcTypeProfile.hpp
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
#ifndef INCLUDED_SRCTYPEPROFILE_HPP
#define INCLUDED_SRCTYPEPROFILE_HPP

#include <string>
#include <iostream>
#include <exception>
#include <unordered_map>
#include <unordered_set>
namespace srcTypeNS{    
    enum PrimTypes{INT, UINT, SIINT, SHINT, SISINT, USISINT, LINT, SLINT, USILINT, SILINT, LDOUBLE, WCHAR, CHAR, UCHAR, SCHAR, VOID, FLOAT, BOOL, DOUBLE, SIZE_T};
    enum VarCategory {primitive=0, userdefined=1};
    
    static const std::unordered_map<std::string, PrimTypes> cppPrimTypes{
    {"int", INT},
    {"unsigned int", UINT},
    {"signed int", SIINT},
    {"short int", SHINT},
    {"signed short int", SISINT},
    {"unsigned short int", USISINT},
    {"long int", LINT},
    {"signed long int", SILINT},
    {"unsigned long int", USILINT},
    {"float", FLOAT},
    {"double", DOUBLE},
    {"long double", LDOUBLE},
    {"wchar_t", WCHAR},
    {"char", CHAR},
    {"unsigned char", UCHAR},
    {"signed char", SCHAR},
    {"void", VOID},
    {"float", FLOAT},
    {"bool", BOOL},
    {"double", DOUBLE},
    {"size_t", SIZE_T}};
};
    static const std::unordered_set<std::string> numberConversionTypes{
    "int",
    "unsigned int",
    "signed int",
    "short int",
    "signed short int",
    "unsigned short int",
    "long int",
    "signed long int",
    "unsigned long int",
    "float",
    "double",
    "long double",
    "wchar_t",
    "char",
    "unsigned char",
    "signed char",
    "void",
    "float",
    "bool",
    "double",
    "size_t"};

    static const std::unordered_set<std::string> stringConversionTypes{
    "char",
    "unsigned char",
    "signed char"};
#endif