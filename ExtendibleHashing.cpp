#include "ExtendibleHashing.hpp"
#include <cmath>
#include <memory>
#include <vector>
#include <iostream>
using namespace std;

ExtendibleHashing::ExtendibleHashing(int globalDepth, int maxSizeBucket) {
  this->globalDepth = globalDepth;
  this->maxSizeBucket = maxSizeBucket;
  this->size = pow(2,globalDepth); 
  this->indices=new Indice[size];
  this->buckets.emplace_back(make_unique<Bucket>(globalDepth,maxSizeBucket));
  indices[0].refBucket = buckets.back().get();

  this->buckets.emplace_back(make_unique<Bucket>(globalDepth,maxSizeBucket));
  indices[1].refBucket = buckets.back().get();
  this->numBuckets= 2;

}

ExtendibleHashing::~ExtendibleHashing() {
  delete[] indices;
}

int ExtendibleHashing::getIndexToValue(int value, int depth) {
  int mascara = pow(2,depth) - 1;
  return value & mascara;
}

void ExtendibleHashing::insert(int value) {
  int index = getIndexToValue(value,globalDepth);
  if (!indices[index].refBucket->add(value)) {
    Bucket * overflow = indices[index].refBucket;
    overflow->increaseDepth();

    if (overflow->getDepthLocal() >= globalDepth) dividir();

    buckets.emplace_back(make_unique<Bucket>(overflow->getDepthLocal(),maxSizeBucket));
    Bucket * newBucket = buckets.back().get();
    this->numBuckets++;

    recalculate(index, overflow, newBucket);

    int nextIndex = getNextIndex(index);
    indices[nextIndex].refBucket = newBucket;
    insert(value);
  }
}

void ExtendibleHashing::dividir() {
  this->globalDepth++;
  int newSize = pow(2, globalDepth);
  Indice * temp = new Indice[newSize];
  for (int i = 0; i < size; i++) {
    temp[i] = indices[i];
    temp[i+size] = indices[i];
  }
  this->size = newSize;
  delete [] indices;
  indices = temp;

}

void ExtendibleHashing::recalculate(int index, Bucket * current, Bucket * next) {
  for (int i = 0; i < maxSizeBucket; i++) {
    int value = current->getValue(i);
    if (!Bucket::isBitZero(value,current->getDepthLocal())) {
      next->add(value);
      current->deleteValue(value);
    }
  }
}

int ExtendibleHashing::getNextIndex(int current) {
  int aux = current;
  aux |= 1 << (globalDepth-1);
  return aux;
}

void ExtendibleHashing::printBuckets() {
  for (const auto & b: buckets) {
    cout<<"\tBucket depth : "<<b->getDepthLocal()<<" / elementos -> ";
    for (int i = 0; i< b->getCounterValues(); i++) {
      cout<<b->getValue(i)<<"\t";
    }
    cout<<endl;
  }
}
int ExtendibleHashing::getNumBuckets() {
  return this->numBuckets;
}

int ExtendibleHashing::getSize() {
  return this->size;
}

int ExtendibleHashing::find(int value) {
  int index = getIndexToValue(value, globalDepth);
  Bucket * current = indices[index].refBucket;

  if (current->getCounterValues() > 0) {
    for (int i = 0; i < current->getCounterValues(); i++) {
      if (current->getValue(i) == value) {
        return index;
      }
    }
    int nextBucket = (index*2)+1;
    if (nextBucket<size) {
      current = indices[nextBucket].refBucket;
      if (current->getCounterValues() > 0) {
        for (int i = 0; i < current->getCounterValues(); i++) {
          if (current->getValue(i) == value) {
            return nextBucket;
          }
        }
      }
    }
  };
  return -1;
}

bool ExtendibleHashing::deleteValue(int value) {
  int indexResult = find(value);
  if (indexResult >= 0) {
    Bucket * current = indices[indexResult].refBucket;
    current->deleteValue(value);
    return true;
  }
  return false;
}

void ExtendibleHashing::printindices() {
  for (int i = 0; i< size; i++) {
    Bucket * current = indices[i] .refBucket;
    cout<<"indice "<<i<<" :: depthLocal del bucket "<<current->getDepthLocal()<<""<<" / primer elemento del "<<current->getValue(0)<<endl;
  }
}







//*Bucket-----------------------------------------------------------
bool Bucket::add(int value) {
  if (count < maxSize) {
    values[count] = value;
    count++;
    return true;
  }
  else {
    return false;
  }
}

Bucket::Bucket(int localDepth, int maxSize) {
  this->localDepth = localDepth;
  this->maxSize = maxSize;
  this->values = new int[maxSize]();
  this->count = 0;
}
Bucket::~Bucket() {
  delete [] values;
}

void Bucket::increaseDepth() {
  this->localDepth++;
}

int Bucket::getDepthLocal() {
  return this->localDepth;
}

bool Bucket::isBitZero(int value, int indexBit) {
  int mascara = 1 << (indexBit-1);
  return (value & mascara) == 0;
}

void Bucket::deleteValue(int value) {
  for (int i = 0; i < count; i++) {
    if (values[i] == value) {
      values[i] = values[count-1];
      count--;
      return ;
    }
  }
}

int Bucket::getValue(int index) {
  return values[index];
}

int Bucket::getCounterValues() {
  return this->count;
}
