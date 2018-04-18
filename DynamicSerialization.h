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
