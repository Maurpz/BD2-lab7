#include <memory>
#include <vector>
using namespace std;
#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H


class Bucket {
  private:
    int localDepth;
    int maxSize;
    int count;
    int * values;
  public:
    Bucket(int localDepth, int maxSize);
    ~Bucket();
    bool add(int value);
    void increaseDepth();
    int getDepthLocal();
    static bool isBitZero(int value, int indexBit);
    void deleteValue(int value);
    int getValue(int index);
    int getIndexByDepth();
    int getCounterValues();

};



struct Indice {
  Bucket * refBucket;
};





class ExtendibleHashing {
  private:
    int globalDepth;
    int size;

    int maxSizeBucket;
    vector<unique_ptr<Bucket>> buckets;
    int numBuckets;

    Indice * indices;
  public:
    ExtendibleHashing(int globalDepth, int maxSizeBucket);
    ~ExtendibleHashing();

    void insert(int value);
    int find(int value);
    bool deleteValue(int value);

    void dividir();
    void recalculate(int index, Bucket * current, Bucket * next);

    static int getIndexToValue(int value, int depth);
    int getNextIndex(int current);
    void printBuckets();
    int getNumBuckets();
    int getSize();
    void printindices();
};
#endif