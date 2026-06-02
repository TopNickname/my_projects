#pragma once
#include "VectorN.h"
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace std_plus {

    // ============================================================
    // Общие детали реализации 
    // ============================================================
    namespace detail {

        // MurmurHash2 – используется и HashTable, и HashSet
        inline uint32_t murmur_hash2(const void* key_ptr, int len, uint32_t seed) {
            const uint32_t m = 0x5bd1e995;
            const int r = 24;
            uint32_t h = seed ^ len;
            const unsigned char* data = static_cast<const unsigned char*>(key_ptr);

            while (len >= 4) {
                uint32_t k;
                std::memcpy(&k, data, 4);
                k *= m;
                k ^= k >> r;
                k *= m;
                h *= m;
                h ^= k;
                data += 4;
                len -= 4;
            }

            switch (len) {
            case 3: h ^= data[2] << 16;
            case 2: h ^= data[1] << 8;
            case 1: h ^= data[0];
                h *= m;
            }

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;
            return h;
        }

        // Единая точка хеширования значения
        template<typename T>
        uint32_t hash_value(const T& value, uint32_t seed = 0) {
            if constexpr (std::is_same_v<T, std::string>) {
                return murmur_hash2(value.data(),
                    static_cast<int>(value.size() * sizeof(char)),
                    seed);
            }
            else {
                static_assert(std::is_trivially_copyable_v<T>,
                    "HashTable/HashSet: type must be std::string or trivially copyable");
                return murmur_hash2(&value, sizeof(T), seed);
            }
        }

        // Общий итератор для обхода корзин
        template<typename Table, bool IsConst>
        class HashIterator {
        public:
            using table_type = std::conditional_t<IsConst, const Table, Table>;
            using value_type = typename Table::value_type;
            using reference = std::conditional_t<IsConst, const value_type&, value_type&>;
            using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;

        private:
            table_type* table;
            size_t bucket_idx;
            size_t elem_idx;

            void advance_to_next() {
                while (bucket_idx < table->data.size() &&
                    elem_idx >= table->data[bucket_idx].size()) {
                    ++bucket_idx;
                    elem_idx = 0;
                }
            }

        public:
            HashIterator(table_type* t, size_t b, size_t e)
                : table(t), bucket_idx(b), elem_idx(e) {
                if (bucket_idx < table->data.size() &&
                    elem_idx >= table->data[bucket_idx].size())
                    advance_to_next();
            }

            reference operator*() const {
                return table->data[bucket_idx][elem_idx];
            }
            pointer operator->() const { return &(operator*()); }

            HashIterator& operator++() {
                ++elem_idx;
                advance_to_next();
                return *this;
            }
            HashIterator operator++(int) {
                HashIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            bool operator==(const HashIterator& other) const {
                return table == other.table &&
                    bucket_idx == other.bucket_idx &&
                    elem_idx == other.elem_idx;
            }
            bool operator!=(const HashIterator& other) const {
                return !(*this == other);
            }
        };

        // Вспомогательный rehash – параметризуется функтором получения ключа
        template<typename BucketVector, typename GetKey>
        void rehash_impl(BucketVector& data, size_t new_bucket_count, GetKey&& get_key) {
            BucketVector new_data(new_bucket_count);
            for (size_t b = 0; b < data.size(); ++b) {
                for (size_t e = 0; e < data[b].size(); ++e) {
                    const auto& elem = data[b][e];
                    size_t hash = detail::hash_value(get_key(elem), 0);
                    size_t idx = hash % new_bucket_count;
                    new_data[idx].push_back(elem);
                }
            }
            data.swap(new_data);
        }
    } // namespace detail

    // ============================================================
    // HashTable
    // ============================================================
    template<typename key, typename T>
    class HashTable {
        // Дружественный итератор
        template<typename, bool> friend class detail::HashIterator;

    public:
        using value_type = std::pair<key, T>;

    private:
        VectorN<VectorN<value_type>> data;
        size_t count;

        void rehash(size_t new_bucket_count) {
            detail::rehash_impl(data, new_bucket_count,
                [](const value_type& pair) -> const key& { return pair.first; });
        }

    public:
        static uint32_t HashFunction(const key& k, uint32_t seed = 0) {
            return detail::hash_value(k, seed);
        }

        HashTable() : data(8), count(0) {}
        explicit HashTable(size_t bucket_count) : data(bucket_count), count(0) {
            if (bucket_count == 0)
                throw std::invalid_argument("bucket_count must be > 0");
        }

        size_t size()        const noexcept { return count; }
        size_t bucket_count() const noexcept { return data.size(); }
        bool empty()         const noexcept { return count == 0; }

        void add(const key& k, const T& v) {
            if (count >= data.size())
                rehash(data.size() * 2);

            size_t hash = HashFunction(k, 0);
            size_t idx = hash % data.size();
            VectorN<value_type>& bucket = data[idx];
            for (auto& pair : bucket) {
                if (pair.first == k) {
                    pair.second = v;
                    return;
                }
            }
            bucket.push_back(value_type(k, v));
            ++count;
        }

        bool contains(const key& k) const {
            size_t hash = HashFunction(k, 0);
            size_t idx = hash % data.size();
            for (const auto& pair : data[idx])
                if (pair.first == k) return true;
            return false;
        }

        T& operator[](const key& k) {
            size_t hash = HashFunction(k, 0);
            size_t idx = hash % data.size();
            auto& bucket = data[idx];
            for (auto& pair : bucket)
                if (pair.first == k) return pair.second;

            if (count >= data.size()) {
                rehash(data.size() * 2);
                idx = hash % data.size();
                auto& new_bucket = data[idx];
                new_bucket.push_back(value_type(k, T{}));
                ++count;
                return new_bucket.back().second;
            }
            else {
                bucket.push_back(value_type(k, T{}));
                ++count;
                return bucket.back().second;
            }
        }

        const T& at(const key& k) const {
            size_t hash = HashFunction(k, 0);
            size_t idx = hash % data.size();
            for (const auto& pair : data[idx])
                if (pair.first == k) return pair.second;
            throw std::out_of_range("HashTable::at: key not found");
        }

        bool erase(const key& k) {
            size_t hash = HashFunction(k, 0);
            size_t idx = hash % data.size();
            auto& bucket = data[idx];
            for (size_t i = 0; i < bucket.size(); ++i) {
                if (bucket[i].first == k) {
                    bucket.erase(i, 1);
                    --count;
                    return true;
                }
            }
            return false;
        }

        void clear() {
            for (auto& bucket : data) bucket.clear();
            count = 0;
        }

        using iterator = detail::HashIterator<HashTable, false>;
        using const_iterator = detail::HashIterator<HashTable, true>;

        iterator begin() { return empty() ? end() : iterator(this, 0, 0); }
        iterator end() { return iterator(this, data.size(), 0); }
        const_iterator begin() const { return empty() ? end() : const_iterator(this, 0, 0); }
        const_iterator end()   const { return const_iterator(this, data.size(), 0); }
        const_iterator cbegin() const { return begin(); }
        const_iterator cend()   const { return end(); }
    };

    // ============================================================
    // HashSet
    // ============================================================
    template<typename T>
    class HashSet {
        // Дружественный итератор
        template<typename, bool> friend class detail::HashIterator;

    public:
        using value_type = T;

    private:
        VectorN<VectorN<T>> data;
        size_t count;

        void rehash(size_t new_bucket_count) {
            detail::rehash_impl(data, new_bucket_count,
                [](const T& elem) -> const T& { return elem; });
        }

    public:
        static uint32_t HashFunction(const T& value, uint32_t seed = 0) {
            return detail::hash_value(value, seed);
        }

        HashSet() : data(8), count(0) {}
        explicit HashSet(size_t bucket_count) : data(bucket_count), count(0) {
            if (bucket_count == 0)
                throw std::invalid_argument("bucket_count must be > 0");
        }

        size_t size()        const noexcept { return count; }
        size_t bucket_count() const noexcept { return data.size(); }
        bool empty()         const noexcept { return count == 0; }

        void clear() {
            for (auto& bucket : data) bucket.clear();
            count = 0;
        }

        bool contains(const T& value) const {
            size_t hash = HashFunction(value, 0);
            size_t idx = hash % data.size();
            for (const auto& elem : data[idx])
                if (elem == value) return true;
            return false;
        }

        using const_iterator = detail::HashIterator<HashSet, true>;
        using iterator = const_iterator;

        const_iterator find(const T& value) const {
            size_t hash = HashFunction(value, 0);
            size_t idx = hash % data.size();
            for (size_t i = 0; i < data[idx].size(); ++i)
                if (data[idx][i] == value)
                    return const_iterator(this, idx, i);
            return end();
        }

        std::pair<iterator, bool> insert(const T& value) {
            size_t hash = HashFunction(value, 0);
            size_t idx = hash % data.size();
            auto& bucket = data[idx];
            for (size_t i = 0; i < bucket.size(); ++i)
                if (bucket[i] == value)
                    return { const_iterator(this, idx, i), false };

            if (count >= data.size())
                rehash(data.size() * 2);

            idx = hash % data.size();
            auto& new_bucket = data[idx];
            new_bucket.push_back(value);
            ++count;
            return { const_iterator(this, idx, new_bucket.size() - 1), true };
        }

        void add(const T& value) { insert(value); }

        bool erase(const T& value) {
            size_t hash = HashFunction(value, 0);
            size_t idx = hash % data.size();
            auto& bucket = data[idx];
            for (size_t i = 0; i < bucket.size(); ++i) {
                if (bucket[i] == value) {
                    bucket.erase(i, 1);
                    --count;
                    return true;
                }
            }
            return false;
        }

        iterator begin() { return empty() ? end() : const_iterator(this, 0, 0); }
        iterator end() { return const_iterator(this, data.size(), 0); }
        const_iterator begin() const { return cbegin(); }
        const_iterator end()   const { return cend(); }
        const_iterator cbegin() const { return empty() ? end() : const_iterator(this, 0, 0); }
        const_iterator cend()   const { return const_iterator(this, data.size(), 0); }
    };

} // namespace std_plus