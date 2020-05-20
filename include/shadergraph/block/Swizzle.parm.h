#ifndef PARAM_INFO
#error "You must define PARAM_INFO macro before include this file"
#endif

PARAM_INFO(Red,   shadergraph::block::Swizzle::Channel, r, m_r, (shadergraph::block::Swizzle::Channel::Red))
PARAM_INFO(Green, shadergraph::block::Swizzle::Channel, g, m_g, (shadergraph::block::Swizzle::Channel::Green))
PARAM_INFO(Blue,  shadergraph::block::Swizzle::Channel, b, m_b, (shadergraph::block::Swizzle::Channel::Blue))
PARAM_INFO(Alpha, shadergraph::block::Swizzle::Channel, a, m_a, (shadergraph::block::Swizzle::Channel::Alpha))
