#include "tritset.h"

ostream &operator<<(ostream &out, Trit &trit) {
    switch (trit) {
        case Trit::True :
            out << 1;
            break;
        case Trit::False :
           out << 0;
            break;
        default:
            out << "?";
    }
    return out;
};

ostream &operator<<(ostream &out, Tritset &set) {
    for (auto s : set) {
        out << s;
    }
    return out;
};

Tritset::Tritset(uint s) {
    trueAmount = 0;
    falseAmount = 0;
    unknownAmount = 0;
    lastImportantTrit = 0;
    isChecked = false;
    size = s;
    if (s % TRITS_IN_UINT == 0)
        trits.resize(s / TRITS_IN_UINT, 0);
    else
        trits.resize(s / TRITS_IN_UINT + 1, 0);
}

Tritset_P Tritset::operator[](uint nmb) {
    if (nmb >= size) {
        return Tritset_P(*this, nmb, trits.front(), TRITS_IN_UINT + 1 + nmb);
    }
    return Tritset_P(*this, nmb, trits[nmb / TRITS_IN_UINT], nmb % TRITS_IN_UINT);
}

uint Tritset::capacity() const {
    return size;
}

void Tritset::shrink() {
    check();
    uint newSize;
    if (lastImportantTrit % TRITS_IN_UINT == 0)
        newSize = lastImportantTrit / TRITS_IN_UINT;
    else
        newSize = lastImportantTrit / TRITS_IN_UINT + 1;
    size = lastImportantTrit + 1;
    trits.resize(newSize);
    isChecked = false;
}

size_t Tritset::cardinality(Trit value) {
    check();
    switch (value) {
        case Trit::True:
            return trueAmount;
        case Trit::False:
            return falseAmount;
        default:
            return unknownAmount;
    }
}

void Tritset::trim(size_t lastIndex) {
    check();
    uint newSize;
    if (lastImportantTrit % TRITS_IN_UINT == 0)
        newSize = lastImportantTrit / TRITS_IN_UINT;
    else
        newSize = lastImportantTrit / TRITS_IN_UINT + 1;
    uint temp = trits.size();
    trits.resize(newSize);
    trits.resize(temp, 0);
    for (uint i = 0; i < TRITS_IN_UINT - lastIndex % TRITS_IN_UINT; i++)
        (*this)[lastIndex + i] = Trit::Unknown;
}

size_t Tritset::length() const {
    uint length = capacity();
    uint lit = 0;
    for (uint i = 0; i < length; i++) {
        if ((*this)[i] != Trit::Unknown) {
            lit = i;
        }
    }
    return lit + 1;
}

Tritset Tritset::operator&(const Tritset &set) const {
    uint biggestLength = (size > set.size) ? size : set.size;
    Tritset returnSet(biggestLength);
    uint mask1, mask2;
    for (uint i = 0; i < biggestLength; i++) {
        mask1 = 3;
        mask2 = 3;
        mask1 = mask1 & (trits[i / TRITS_IN_UINT] >> (2 * (i % TRITS_IN_UINT)));
        mask2 = mask2 & (set.trits[i / TRITS_IN_UINT] >> (2 * (i % TRITS_IN_UINT)));
        if (mask1 == static_cast<uint>(Trit::False) || mask2 == static_cast<uint>(Trit::False))
            returnSet[i] = Trit::False;
        else if (mask1 == static_cast<uint>(Trit::True) && mask2 == static_cast<uint>(Trit::True))
            returnSet[i] = Trit::True;
        else returnSet[i] = Trit::Unknown;
    }
    return returnSet;
};

Tritset Tritset::operator|(const Tritset &set) const {
    uint biggestLength = (size > set.size) ? size : set.size;
    Tritset returnSet(biggestLength);
    uint mask1, mask2;
    for (uint i = 0; i < biggestLength; i++) {
        mask1 = 3;
        mask2 = 3;
        mask1 = mask1 & (trits[i / TRITS_IN_UINT] >> (2 * (i % TRITS_IN_UINT)));
        mask2 = mask2 & (set.trits[i / TRITS_IN_UINT] >> (2 * (i % TRITS_IN_UINT)));
        if (mask1 == static_cast<uint>(Trit::True) || mask2 == static_cast<uint>(Trit::True)) returnSet[i] = Trit::True;
        else if (mask1 == static_cast<uint>(Trit::False) && mask2 == static_cast<uint>(Trit::False))
            returnSet[i] = Trit::False;
        else returnSet[i] = Trit::Unknown;
    }
    return returnSet;
}

Tritset Tritset::operator!() {
    uint length = size;
    Tritset returnSet(length);
    uint mask1;
    for (uint i = 0; i < length; i++) {
        mask1 = 3;
        mask1 = mask1 & (trits[i / TRITS_IN_UINT] >> (2 * (i % TRITS_IN_UINT)));
        if (mask1 == static_cast<uint>(Trit::True)) returnSet[i] = Trit::False;
        else if (mask1 == static_cast<uint>(Trit::False)) returnSet[i] = Trit::True;
        else returnSet[i] = Trit::Unknown;
    }
    return returnSet;
}

Tritset &Tritset::operator=(const Tritset &set) {
    trueAmount = set.trueAmount;
    falseAmount = set.falseAmount;
    lastImportantTrit = set.lastImportantTrit;
    unknownAmount = set.unknownAmount;
    size = set.size;
    trits = set.trits;
    isChecked = set.isChecked;
    return *this;
}

void Tritset::check() {
    if (!isChecked) {
        uint length = capacity();
        trueAmount = 0;
        falseAmount = 0;
        lastImportantTrit = 0;
        uint unknownsBuffer = 0;
        for (uint i = 0; i < length; i++) {
            if ((*this)[i] == Trit::Unknown)
                unknownsBuffer++;
            else {
                if ((*this)[i] == Trit::True)
                    trueAmount++;
                else
                    falseAmount++;
                lastImportantTrit = i;
                unknownAmount += unknownsBuffer;
                unknownsBuffer = 0;
            }
        }
        isChecked = true;
    }
}

Trit Tritset::operator[](uint pos) const {
    uint n = pos / TRITS_IN_UINT;
    uint shift = pos % TRITS_IN_UINT;
    uint source = trits[n];
    source = 3 & (source >> (shift * 2));
    return static_cast<Trit>(source);
}

Tritset::Tritset(initializer_list<Trit> list) {
    size = list.size();
    trueAmount = 0;
    falseAmount = 0;
    unknownAmount = 0;
    if (size % TRITS_IN_UINT == 0)
        trits.resize(size / TRITS_IN_UINT, 0);
    else
        trits.resize(size / TRITS_IN_UINT + 1, 0);
    uint i = 0;
    for (auto &s: list) {
        (*this)[i] = s;
        i++;
    }
    isChecked = false;
    check();
}

Tritset::iterator Tritset::begin() {
    return Tritset::iterator(this, 0);
}

Tritset::iterator Tritset::end() {
    return Tritset::iterator(this, length());
};

Tritset_P &Tritset_P::operator=(const Trit &n) {
    if (shift / 2 > set.TRITS_IN_UINT) {
        if (n != Trit::Unknown) {
            uint newSize = shift / 2 - set.TRITS_IN_UINT - 1;
            set.trits.resize(newSize / set.TRITS_IN_UINT + 1, 0);
            set.size = newSize;
            newSize--;
            set.trits[newSize / set.TRITS_IN_UINT] =
                    set.trits[newSize / set.TRITS_IN_UINT] & (~(3 << ((newSize % set.TRITS_IN_UINT) * 2)));
            set.trits[newSize / set.TRITS_IN_UINT] =
                    set.trits[newSize / set.TRITS_IN_UINT] |
                    (static_cast<int> (n) << (newSize % set.TRITS_IN_UINT) * 2);
        }
    } else {
        pointer = pointer & (~(3 << shift));
        pointer = pointer | (static_cast<int> (n) << shift);
    }
    set.isChecked = false;
    return *this;
}

bool Tritset_P::operator==(Trit n) {
    uint mask = 3;
    mask = mask & (pointer >> shift);
    return (static_cast<int> (n) == mask);
}

Tritset_P::Tritset_P(Tritset &set, uint nmb, uint &pointer, uint shift) : set(set), nmb(nmb), pointer(pointer),
                                                                          shift(shift * 2) {}

Tritset_P::operator Trit() {
    uint mask = 3;
    mask = mask & (pointer >> shift);
    switch (mask) {
        default:
            return Trit::Unknown;
        case 1:
            return Trit::True;
        case 2:
            return Trit::False;

    }
}

Tritset::iterator::iterator(Tritset *set, uint ind) {
    parent = set;
    index = ind;
}

Tritset::iterator &Tritset::iterator::operator++() {
    index++;
    return (*this);
}

bool Tritset::iterator::operator!=(const Tritset::iterator &it) {
    return it.index != index;
}

Trit Tritset::iterator::operator*() {
    Trit a = (*parent)[index];
    return a;
}
