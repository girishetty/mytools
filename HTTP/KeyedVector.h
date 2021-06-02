/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_KEYED_VECTOR_H
#define ANDROID_KEYED_VECTOR_H

#include <assert.h>
#include <stdint.h>
#include <sys/types.h>
#include <vector>
#include <utility>

using namespace std;


template <typename KEY, typename VALUE> class KeyedVector {
public:
    typedef KEY    key_type;
    typedef VALUE  value_type;

    inline                  KeyedVector();

    //empty the vector
    inline void clear()  { mVector.clear(); }
    //returns number of items in the vector
    inline size_t size() const { return mVector.size(); }
    //returns whether or not the vector is empty
    inline bool isEmpty() const { return mVector.empty(); }
    //returns how many items can be stored without reallocating the backing store
    inline size_t capacity() const { return mVector.capacity(); }

    const VALUE& valueAt(size_t index) const;
    const KEY& keyAt(size_t index) const;
    const VALUE& operator[] (size_t index) const;
    void add(const KEY& key, const VALUE& item);

private:
    vector< pair<KEY, VALUE> > mVector;
};

template<typename KEY, typename VALUE> inline
KeyedVector<KEY,VALUE>::KeyedVector()
{
}

template<typename KEY, typename VALUE> inline
const VALUE& KeyedVector<KEY,VALUE>::valueAt(size_t index) const {
    return mVector.at(index).second;
}

template<typename KEY, typename VALUE> inline
const VALUE& KeyedVector<KEY,VALUE>::operator[] (size_t index) const {
    return valueAt(index);
}

template<typename KEY, typename VALUE> inline
const KEY& KeyedVector<KEY,VALUE>::keyAt(size_t index) const {
    return mVector.at(index).first;
}

template<typename KEY, typename VALUE> inline
void KeyedVector<KEY,VALUE>::add(const KEY& key, const VALUE& value) {
    return mVector.push_back( pair<KEY,VALUE>(key, value) );
}

#endif // ANDROID_KEYED_VECTOR_H
