#ifndef UTIL_ARRAY_H
#define UTIL_ARRAY_H

#include <cassert>
#include <memory>
#include <memory.h>

// Declaration
namespace opent4
{
    namespace util
    {
        template<typename T> struct Array
        {
            Array() : _size(0), _capacity(0), _data(0) {}
            ~Array() { if(_data) free(_data); }
            Array(const Array &other)
            {
                const uint32_t n = other._size;
                set_capacity(n);
                memcpy(_data, other._data, sizeof(T)*n);
                _size = n;
            }

            Array<T> &operator=(const Array &other)
            {
                const uint32_t n = other._size;
                resize(n);
                memcpy(_data, other._data, sizeof(T)*n);
                return *this;
            }

            T &operator[](uint32_t i)
            {
                assert(i < _size);
                return _data[i];
            }

            const T &operator[](uint32_t i) const
            {
                return _data[i];
            }

            uint32_t size()     { return _size; }
            bool any()          { return _size != 0; }
            bool empty()        { return !any(); }

            T* begin()          { return _data; }
            //const T* begin()    { return _data; }
            T* end()            { return _data + _size; }
            //const T* end()      { return _data + _size; }

            T& front()          { return _data[0]; }
            //const T& front()    { return _data[0]; }
            T& back()           { return _data[_size-1]; }
            //const T& back()     { return _data[_size-1]; }

            void resize(uint32_t new_size)
            {
                if(new_size > _capacity)
                    grow(new_size);
                _size = new_size;
            }

            void reserve(uint32_t new_capacity)
            {
                if(new_capacity > _capacity)
                    set_capacity(new_capacity);
            }

            void set_capacity(uint32_t new_capacity)
            {
                if(new_capacity == _capacity)
                    return;

                if(new_capacity < _size)
                    resize(new_capacity);

                T *new_data = 0;
                if(new_capacity > 0) {
                    new_data = (T*)malloc(sizeof(T)*new_capacity);
                    memcpy(new_data, _data, sizeof(T)*_size);
                }
                if(_data)
                    free(_data);
                _data = new_data;
                _capacity = new_capacity;
            }

            void grow(uint32_t min_capacity=0)
            {
                uint32_t new_capacity = _capacity*2+8;
                if(new_capacity < min_capacity)
                    new_capacity = min_capacity;
                set_capacity(new_capacity);
            }

            void clear() { resize(0); }
            void trim() { set_capacity(_size); }

            void push_back(const T &item)
            {
                if(_size+1 > _capacity)
                    grow();
                _data[_size++] = item;
            }

            void pop_back()
            {
                _size--;
            }

            uint32_t _size;
            uint32_t _capacity;
            T *_data;
        };
    }
}

#endif
