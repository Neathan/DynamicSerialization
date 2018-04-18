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

}

#endif