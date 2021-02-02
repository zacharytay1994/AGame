//#include "zSystem.h"
//#include "zComponent.h"
//#include "zecs.h"
//
//zSystem::zSystem(const ComponentMask& mask)
//	:
//	_mask(mask)
//{
//}
//
//void zSystem::Update(const float& dt)
//{
//	std::unordered_map<ComponentMask, zArchetype>& pool = zComponentManager::Instance().GetPool();
//	for (auto& i : pool) {
//		// if mask match in archetype
//		if ((i.first & _mask) == i.first) {
//			for (zChunk& c : i.second._chunks) {
//				char* temp = c._data.get();
//				TypeDetails td = zECS::Instance().GetTypeDetails("TestComponent");
//			}
//		}
//	}
//}
