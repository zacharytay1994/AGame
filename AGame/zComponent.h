#pragma once

#include <memory>
#include <vector>
#include <cstddef>

struct ComponentDescription {
	const	uint32_t	_size;
	mutable	int			_bit;
};

namespace ComponentDescriptionDetails {
	template <typename T_COMPONENT>
	ComponentDescription MakeComponentDescription() {
		return ComponentDescription{ sizeof(T_COMPONENT),-1 };
	}
	// to prevent vs compiler from creating multiple instances of a comp desc
	template <typename T_COMPONENT>
	struct BugFix {
		inline static ComponentDescription component_description 
			= MakeComponentDescription<T_COMPONENT>();
	};
}

template <typename T_COMPONENT>
inline static ComponentDescription& component_description_v
= ComponentDescriptionDetails::BugFix<T_COMPONENT>::component_description;

struct ComponentDescription_DB {
	template <typename T>
	void RegisterComponent() {
		component_description_v<T>._bit = _bit_generator++;
		_database.push_back(&component_description_v<T>);
	}
	std::vector<const ComponentDescription*>	_database;
	uint32_t									_bit_generator = 0;
};