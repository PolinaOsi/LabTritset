#ifndef TRITSET_TRITSET_H
#define TRITSET_TRITSET_H

#include <iostream>
#include <vector>
#include <initializer_list>


typedef unsigned int uint;

enum class Trit {
    Unknown, True, False
};

class Tritset_P {
private:
    class Tritset &set;

    uint nmb;
    uint &pointer;
    uint shift;
public:
    Tritset_P(Tritset &, uint, uint &, uint);

    Tritset_P &operator=(const Trit &);

    bool operator==(Trit);

    operator Trit();
};


class Tritset {
private:
    uint trueAmount;
    uint falseAmount;
    uint lastImportantTrit;
    uint unknownAmount;
    bool isChecked;
    const uint TRITS_IN_UINT = sizeof(uint) * 4;
    uint size;
    std::vector<uint> trits;
public:
    class iterator {
    private:
        Tritset *parent;
        uint index;
    public:
        iterator(Tritset *, uint);

        iterator &operator++();

        bool operator!=(const iterator &); //cmp indexes
        Trit operator*();
    };

    iterator begin(); //return iterator this, 0

    iterator end(); //return iterator this, length()

    friend class Tritset_P;

    Tritset &operator=(const Tritset &);

    Tritset(std::initializer_list<Trit>);

    Tritset &operator|=(const Tritset &);

    Tritset &operator&=(const Tritset &);

    Tritset operator&(const Tritset &) const;

    Tritset operator|(const Tritset &) const;

    Tritset operator!();

    void check();

    void shrink();

    size_t cardinality(Trit);

    size_t length() const;

    void trim(size_t);

    explicit Tritset(uint);

    uint capacity() const;

    Trit operator[](uint) const;

    Tritset_P operator[](uint);

};

std::ostream &operator<<(std::ostream &, Trit &);

std::ostream &operator<<(std::ostream &, Tritset &);
int testes();
#endif //TRITSET_TRITSET_H
