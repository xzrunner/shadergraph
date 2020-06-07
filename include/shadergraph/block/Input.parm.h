#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Type, shadergraph::VarType, type, m_type, ())
PARAM_INFO(Name, std::string,          name, m_name, ())
