#ifndef CHAINING_HASH_TABLE_H
#define CHAINING_HASH_TABLE_H

#include "hash_table_base.h"
#include <vector> // Zmieniono z <list> na <vector>

// Implementacja 1: Hash Table z metodą lancuchowa (chaining)
// Ale teraz z uzyciem std::vector zamiast std::list w kazdym "kubku"
class ChainingHashTable : public HashTableBase {
private:
    struct KeyValue {
        int key;
        int value;
        KeyValue(int k, int v) : key(k), value(v) {}
    };

    // Zmieniono std::list na std::vector w kazdym kubku
    std::vector<std::vector<KeyValue>> table;
    size_t table_size;
    size_t current_size;

    // Wspolczynnik obciazenia
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    void resize() {
        size_t old_size = table_size;
        auto old_table = std::move(table);

        table_size *= 2;
        table.clear();
        table.resize(table_size);
        current_size = 0;

        // Przepisz wszystkie elementy
        for (const auto& chain : old_table) {
            for (const auto& kv : chain) {
                insert(kv.key, kv.value);
            }
        }
    }

public:
    explicit ChainingHashTable(size_t initial_size = 16)
        : table_size(initial_size), current_size(0) {
        table.resize(table_size);
    }

    bool insert(int key, int value) override {
        // Sprawdz czy trzeba zwiekszyc rozmiar
        if (static_cast<double>(current_size) / table_size > MAX_LOAD_FACTOR) {
            resize();
        }

        size_t index = hash_function(key, table_size);
        auto& chain = table[index]; // Teraz to jest std::vector<KeyValue>

        // Sprawdz czy klucz juz istnieje
        for (auto& kv : chain) {
            if (kv.key == key) {
                kv.value = value; // Aktualizuj wartosc
                return true;
            }
        }

        // Dodaj nowy element do wektora
        chain.emplace_back(key, value);
        current_size++;
        return true;
    }


    bool remove(int key) override {
        size_t index = hash_function(key, table_size);
        auto& chain = table[index]; // Teraz to jest std::vector<KeyValue>

        // Szukaj elementu do usuniecia w wektorze
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) {
                // Usun element z wektora. erase() dla vectora moze byc kosztowne
                // (przenoszenie wszystkich elementow za usunietym).
                chain.erase(it);
                current_size--;
                return true;
            }
        }
        return false;
    }

    bool find(int key, int& value) override {
        size_t index = hash_function(key, table_size);
        const auto& chain = table[index]; // Teraz to jest std::vector<KeyValue>

        for (const auto& kv : chain) {
            if (kv.key == key) {
                value = kv.value;
                return true;
            }
        }
        return false;
    }

    void display() override {
        std::cout << "=== Chaining Hash Table (using std::vector for chains) ===" << std::endl; // Zmieniono nazwe dla jasnosci
        for (size_t i = 0; i < table_size; ++i) {
            std::cout << "Bucket " << i << ": ";
            for (const auto& kv : table[i]) {
                std::cout << "(" << kv.key << "," << kv.value << ") ";
            }
            std::cout << std::endl;
        }
        std::cout << "Size: " << current_size << "/" << table_size << std::endl;
    }

    size_t size() const override { return current_size; }

    void clear() override {
        for (auto& chain : table) {
            chain.clear(); // Wyczysc kazdy wektor
        }
        current_size = 0;
    }

    std::string get_name() const override {
        return "Chaining Hash Table (Vector)"; // Zmieniono nazwe dla jasnosci
    }
};

#endif // CHAINING_HASH_TABLE_H
