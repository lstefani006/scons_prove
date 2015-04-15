#include "c.hpp"

void Context::push(DeclFun *v)
{ 
	st.push(v); 
}
void Context::push(DeclClass *v) 
{ 
	st.push(v); 
}
	
void Context::add(DeclVar *v){

}
	
DeclVar * Context::findVar(const std::string &id)
{
	return nullptr;
}
