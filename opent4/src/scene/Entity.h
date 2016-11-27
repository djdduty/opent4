#ifndef ENTITY_H
#define ENTITY_H

#include "util/array.h"
#include <deque>
#include <cassert>

namespace opent4
{
    namespace scene {
        const uint32_t ENTITY_INDEX_BITS = 24;
        const uint32_t ENTITY_INDEX_MASK = (1<<ENTITY_INDEX_BITS)-1;

        const uint32_t ENTITY_GENERATION_BITS = 8;
        const uint32_t ENTITY_GENERATION_MASK = (1<<ENTITY_GENERATION_BITS)-1;

        class EntityManager;
        class Entity
        {
            public:
                uint32_t index() const { return id & ENTITY_INDEX_MASK; }
                uint32_t generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }

            protected:
                friend class EntityManager;
                uint32_t id;
        };

        class EntityManager
        {
            const uint32_t MINIMUM_FREE_INDICES = 1024;

            public:
                EntityManager()
                {
                    //_entities.reserve(512);
                }

                Entity create()
                {
                    uint32_t idx;
                    if(_free_indices.size() > MINIMUM_FREE_INDICES) {
                        idx = _free_indices.front();
                        _free_indices.pop_front();
                    } else {
                        _entities.push_back(0);
                        idx = _entities.size() - 1;
                        assert(idx < (1 << ENTITY_INDEX_BITS));
                    }

                    return _make_entity(idx, _entities[idx]);
                }

                bool alive(Entity e)
                {
                    if(e.index() > _entities.size()-1)
                        return false;
                    return _entities[e.index()] == e.generation();
                }

                void destroy(Entity e)
                {
                    uint32_t idx = e.index();
                    assert(idx < _entities.size());
                    ++_entities[idx];
                    _free_indices.push_back(idx);
                }

            protected:
                Entity _make_entity(uint32_t idx, uint8_t generation)
                {
                    Entity e;
                    e.id = ((uint32_t)generation << ENTITY_INDEX_BITS) | idx;
                    _entities[idx] = generation;
                    return e;
                }

                opent4::util::Array<uint8_t> _entities;
                std::deque<uint32_t> _free_indices;
        };
    }
}

#endif
