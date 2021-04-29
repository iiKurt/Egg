#include <iostream>
#include <fstream>

#pragma pack(1)
struct Header {
    char id[15];
    int length;
    int count;
};
#pragma pack()

int main() {
  Header h = {"alalalala", 5, 10};

  std::fstream fh;
  fh.open("test.bin", std::fstream::out | std::fstream::binary);
  fh.write((char*)&h, sizeof(Header));
  fh.close();

  //////////

  fh.open("test.bin", std::fstream::in | std::fstream::binary);

  Header h2;

  fh.read((char*)&h2, sizeof(h2));

  fh.close();

  std::cout << h2.id << " " << h2.length << " " << h2.count << std::endl;
}
