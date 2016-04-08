#ifndef OBSERVER_PTR_H
#define OBSERVER_PTR_H

#include <memory>

template<typename T>
struct nop_deleter
{
	void operator()(T*) const { }
};

template<typename T>
using observer_ptr = std::unique_ptr<T, struct nop_deleter>;

#endif