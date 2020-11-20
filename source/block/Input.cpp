#include "shadergraph/block/Input.h"
#include "shadergraph/ValueImpl.h"

namespace shadergraph
{
namespace block
{

void Input::SetVarType(const VarType& type) 
{
    m_var_type = type;

    ResetVariants();
}

void Input::SetVarName(const std::string& name)
{
    m_var_name = name;
    m_exports[0].var.type.name = name;

    ResetVariants();
}

void Input::ResetVariants()
{
	m_global_vars.clear();

	if (m_var_name.empty() || m_var_type == VarType::Invalid) {
		return;
	}

    auto unif = std::make_shared<UniformVal>();
    unif->var.type = m_var_type;
    unif->var.name = m_var_name;

    Variant var;
    var.type = VarType::Uniform;
    var.name = unif->var.name;
    var.val = unif;

    m_global_vars.push_back(var);
}

}
}