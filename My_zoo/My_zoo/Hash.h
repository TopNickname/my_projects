#pragma once
#include "VectorN.h"
#include <string>


namespace std_plus {

	template<typename key,typename T>
	class HashTable {
		VectorN < /*<-  тут реально нужен тот пробел(Ќ≈ ”ƒјЋя“№)*/std::pair<key, VectorN<T>>> data;
        size_t size;
		static uint32_t HashFunction(key k, uint32_t seed);
        static uint32_t murmur_hash2(const void* key, int len, uint32_t seed);
        static uint32_t hash_pod(const T& obj, uint32_t seed);
        void resize(size_t new_size);

        class iterator {
            T* data;
            size_t i,j;
        public:
            iterator(T* _data, size_t _i = 0, size_t _j=0 ) : data(_data), i(_i), j(_j) {};
            iterator& operator++();
            iterator& operator--();
            
            bool operator==(iterator x);
            bool operator!=(iterator x);
            T operator*();
        };
    public:
        HashTable();
        HashTable(size_t _size);
        
        void add();
	};

    inline uint32_t murmur_hash2(const void* key, int len, uint32_t seed=0)
    {
        // 'm' и 'r' Ч константы микшировани€
        const uint32_t m = 0x5bd1e995;
        const int r = 24;

        uint32_t h = seed ^ len; // инициализаци€: seed xor длина

        const unsigned char* data = (const unsigned char*)key;

        // ќбрабатываем основное тело ключа блоками по 4 байта
        while (len >= 4)
        {
            // «агружаем 4 байта как little-endian uint32_t
            uint32_t k;
            memcpy(&k, data, 4); // (или можно собрать вручную через сдвиги)

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            len -= 4;
        }

        // ќбрабатываем хвост (оставшиес€ 1-3 байта)
        switch (len)
        {
        case 3: h ^= data[2] << 16;
        case 2: h ^= data[1] << 8;
        case 1: h ^= data[0];
            h *= m;
        }

        // ‘инальное перемешивание
        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }

    template<typename T>
    uint32_t hash_pod(const T& obj, uint32_t seed=0) {
        if constexpr (std::is_trivially_copyable_v<T>)
            return murmur_hash2(&obj, sizeof(T), seed);
        else {
            throw "error";
        }
    }

    template<typename key>
    inline static uint32_t HashFunction(key k, uint32_t seed=0) {
        return hash_pod(k, seed);
    }


    template<>
    inline static uint32_t HashFunction<std::string>(std::string k, uint32_t seed) {
        const void* tmp = k.data();
        return murmur_hash2(tmp, k.size()*sizeof(char), seed);
    }

    
}


