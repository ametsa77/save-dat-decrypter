#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <string>
#include "proton/Variant.hpp"
#include "proton/vardb.h"

class Decrypt {
public:
	void save_dat();
	std::string password();
	std::string get_id();
	unsigned int decrypt(byte* data, unsigned int size, int key);
	uint32 hash_str(const char* str, int32 len);
};