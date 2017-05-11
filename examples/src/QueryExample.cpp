/**
 * @file QueryExample.cpp
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
#include <string>
#include <srcType.hpp>
#include <cassert>
#include <iostream>

int main(int argc, char** argv){
	srcTypeNS::srcType typeDict;
	typeDict.ReadArchiveFile(argv[1]);
	typeDict.SetContext("code.cpp", "Foo");
	{
		auto nameprofile = typeDict.FindVariable("coo");
		std::cerr<<"Name: "<<nameprofile.front().nameofidentifier<<" Type: "<< nameprofile.front().nameoftype<<std::endl;
		assert(nameprofile.front().nameoftype == "Object" 
				&& typeDict.IsPrimitive(nameprofile.front().nameoftype) == false 
				&& nameprofile.front().namespaces.front() == "std");
	}
	{
		auto nameprofile = typeDict.FindVariable("ke_e4e");
		std::cerr<<"Name: "<<nameprofile.front().nameofidentifier<<" Type: "<< nameprofile.front().nameoftype<<std::endl;
	    assert(nameprofile.front().nameoftype == "Object" && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false && nameprofile.front().isConst && nameprofile.front().namespaces.front() == "std");
	}
	{
		auto nameprofile = typeDict.FindVariable("caa34");
		std::cerr<<"Name: "<<nameprofile.front().nameofidentifier<<" Type: "<< nameprofile.front().nameoftype<<std::endl;
		assert(nameprofile.front().nameoftype == "Object" && typeDict.IsPrimitive(nameprofile.front().nameoftype) == false && nameprofile.front().isConst && nameprofile.front().namespaces.front() == "std");
	}
}