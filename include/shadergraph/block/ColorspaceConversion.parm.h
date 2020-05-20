#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(From, shadergraph::block::ColorspaceConversion::ColorType, from, m_from, (shadergraph::block::ColorspaceConversion::ColorType::RGB))
PARAM_INFO(To,   shadergraph::block::ColorspaceConversion::ColorType, to,   m_to,   (shadergraph::block::ColorspaceConversion::ColorType::RGB))
