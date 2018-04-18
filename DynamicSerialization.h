/*

MIT License

Copyright (c) 2018 Neathan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef _DYNAMIC_SERIALIZATION_H_
#define _DYNAMIC_SERIALIZATION_H_

namespace DS {

	template<typename Struct, typename Var>
	void setVariable(Struct s, unsigned char& offset, Var var) {
		*(Var*)((unsigned char*)s + sizeof(*s) - offset - sizeof(Var)) = var;
		offset += sizeof(Var);
	}

	template<typename Struct, typename Var>
	bool createStruct(Struct s, unsigned char& offset, Var var) {
		setVariable<Struct, Var>(s, offset, var);
		return true;
	}

	template<typename Struct, typename Var, typename... Vars>
	bool createStruct(Struct s, unsigned char& offset, Var var, Vars... vars) {
		while(!createStruct(s, offset, vars...)) {
			return false;
		}
		setVariable<Struct, Var>(s, offset, var);
		return true;
	}

	template<typename Struct, typename Var, typename... Vars>
	void createStruct(Struct s, Var var, Vars... vars) {
		unsigned char offset = 0;
		createStruct(s, offset, var, vars...);
	}


	
	template<typename Struct>
	char* serialize(Struct s) {
		size_t size = sizeof(*s);
		char* data = (char*)malloc(size);
		memcpy(data, s, size);
		return data;
	}

	template<typename Struct>
	void deserialize(Struct dest, char* data) {
		memcpy(dest, data, sizeof(*dest));
	}


}

#ifndef DS_NO_FILE
#include <fstream>

namespace DS {

	template<typename Struct>
	void serializeToFile(Struct s, const char* fileName) {
		char* data = serialize(s);
		std::fstream stream;
		stream.open(fileName, std::ios::out | std::ios::binary);
		stream.write(data, sizeof(*s));
		stream.close();
	}

	template<typename Struct>
	void deserializeFromFile(Struct dest, const char* fileName) {
		char* data = new char;
		std::fstream stream;
		stream.open(fileName, std::ios::in | std::ios::binary);
		stream.read(data, sizeof(*dest));
		stream.close();
		deserialize(dest, data);
	}

}

#endif

#endif
