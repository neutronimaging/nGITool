#ifndef __ARRAYBUFFER_H
#define __ARRAYBUFFER_H

#include <base/core/sharedbuffer.h>
template <class T>
class ArrayBuffer
{
public:
	ArrayBuffer(size_t N) : data(N), cnt(0), mSize(N) {};

	size_t push_back(T val) { data.GetDataPtr()[cnt++]=val; return cnt;}
	size_t size() {return cnt;}
	void reset() { cnt=0; }

	T* dataptr() {return data.GetDataPtr();}
	void copy(ArrayBuffer<T> *b) {
		memcpy(data.GetDataPtr(),b->dataptr(),b->size()*sizeof(T));
		cnt=b->size();
	}
	bool empty() {return cnt==0;}
protected:
	kipl::base::core::buffer<T> data;
	size_t cnt;
	size_t mSize;
};
#endif
