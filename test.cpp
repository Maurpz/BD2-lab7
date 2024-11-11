#include "ExtendibleHashing.cpp"
string isTrue(int index) {
  if (index >= 0) {
    return "true";
  }
  return "false";
}

int main () {
  ExtendibleHashing h1(1,3);
  cout<<"inserts..."<<endl;
  h1.insert(28);
  h1.insert(4);
  h1.insert(19);
  h1.insert(1);
  h1.insert(22);
  h1.insert(16);
  h1.insert(12);
  h1.insert(0);
  h1.insert(5);
  h1.insert(7);

  cout<<endl;
  h1.printBuckets();
  cout<<endl;
  cout<<"el tamaño del array de indices es :"<<h1.getSize()<<endl;
  cout<<"Existen "<<h1.getNumBuckets()<<" buckets "<<endl;
  cout<<"Find(88): "<<isTrue(h1.find(88))<<endl;
  cout<<"Find(19): "<<isTrue(h1.find(19))<<endl;
  cout<<"Find(0): "<<isTrue(h1.find(0))<<endl;
  cout<<"delete(28) :"<<isTrue(h1.deleteValue(28))<<endl;;
  cout<<endl;
  h1.printBuckets();
  cout<<endl;

  h1.printindices();

  return 0;
}