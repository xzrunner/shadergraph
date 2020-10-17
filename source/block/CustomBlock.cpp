#include "shadergraph/block/CustomBlock.h"

namespace shadergraph
{
namespace block
{

void CustomBlock::SetCode(const std::string& code)
{ 
    std::shared_ptr<dag::Node<Variant>> me = nullptr;

    // cache conns
    std::map<std::string, std::vector<dag::Node<Variant>::PortAddr>> in_conns, out_conns;

    for (auto& in : m_imports) 
    {
        for (auto& conn : in.conns) 
        {
            if (!me) {
                auto from = conn.node.lock();
                for (auto& f_conn : from->GetExports()[conn.idx].conns) {
                    auto to_node = f_conn.node.lock();
                    if (to_node.get() == this) {
                        me = to_node;
                    }
                }
            }

            auto itr = in_conns.find(in.var.type.name);
            if (itr == in_conns.end()) {
                in_conns.insert({ in.var.type.name, { conn } });
            } else {
                itr->second.push_back(conn);
            }
        }
    }
    for (auto& out : m_exports) 
    {
        for (auto& conn : out.conns)
        {
            if (!me) {
                auto to = conn.node.lock();
                for (auto& t_conn : to->GetImports()[conn.idx].conns) {
                    auto to_node = t_conn.node.lock();
                    if (to_node.get() == this) {
                        me = to_node;
                    }
                }
            }

            auto itr = out_conns.find(out.var.type.name);
            if (itr == out_conns.end()) {
                out_conns.insert({ out.var.type.name, { conn } });
            } else {
                itr->second.push_back(conn);
            }
        }
    }

	Parser(code); 

    // restore conns
    for (int i = 0, n = m_imports.size(); i < n; ++i)
    {
        auto itr = in_conns.find(m_imports[i].var.type.name);
        if (itr != in_conns.end()) {
            for (auto& conn : itr->second) {
                assert(me);
                dag::make_connecting<Variant>(conn, { me, i });
            }
        }
    }
    for (int i = 0, n = m_exports.size(); i < n; ++i)
    {
        auto itr = out_conns.find(m_exports[i].var.type.name);
        if (itr != out_conns.end()) {
            for (auto& conn : itr->second) {
                assert(me);
                dag::make_connecting<Variant>({ me, i }, conn);
            }
        }
    }
}

}
}