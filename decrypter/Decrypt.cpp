#include "Decrypt.hpp"

std::string Decrypt::get_id()
{
	DWORD dwDiskSerial;
	if (!GetVolumeInformation(L"C:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL) &&
		!GetVolumeInformation(L"D:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL) &&
		!GetVolumeInformation(L"E:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL) &&
		!GetVolumeInformation(L"F:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL) &&
		!GetVolumeInformation(L"G:\\", NULL, 0, &dwDiskSerial, NULL, NULL, NULL, NULL)) return "";
	return std::to_string(dwDiskSerial);
}

unsigned int Decrypt::decrypt(byte* data, unsigned int size, int key)
{
	unsigned int checksum = 0;
	for (unsigned int i = 0; i < size; i++)
	{
		checksum += data[i] + key + i;
		data[i] -= (2 + key + i);
	}
	return checksum;
}

uint32 Decrypt::hash_str(const char* str, int32 len)
{
	if (!str) return 0;

	const unsigned char* n = reinterpret_cast<const unsigned char*>(str);
	
	uint32 acc = 0x55555555;

	for (int32 i = 0; i < len; i++) acc = (acc >> 27) + (acc << 5) + *n++;
	return acc;
}


std::string Decrypt::password() {
	VariantDB data{};
	bool exist = false;
	std::string path = "save.dat";
	if (data.Load(path, &exist) == 1) {
		auto password = data.GetVarIfExists("tankid_password");
		if (password) {
			auto varstr = password->get_h();
			auto size = varstr.length();
			auto pass = new uint8_t[size];
			memcpy(pass, varstr.data(), size);
			auto device_id = get_id();
			auto output = decrypt(pass, size, hash_str(device_id.c_str(), device_id.length()));
			auto pass_str = (string)(char*)(&*(DWORD**)pass);
			delete[] pass;
			pass_str.resize(size);
			return pass_str.c_str();
		}
	}
	else std::cout << "Save.dat not found!" << std::endl;
	return "";
}

void Decrypt::save_dat() {
	VariantDB data{};
	bool exist = false;
	std::string path = "save.dat";
	if (data.Load(path, &exist) == 1) {
		auto username = data.GetVarIfExists("tankid_name");
		auto supporter = data.GetVarIfExists("isPayingUser");
		auto world = data.GetVarIfExists("lastworld");
		if (username) std::cout << "GrowID: " << username->get_h().c_str() << std::endl;
		if (!Decrypt::password().empty()) std::cout << "Password: " << Decrypt::password() << std::endl;
		if (supporter) std::cout << "Supporter: " << supporter->get_h().c_str() << std::endl;
		if (world) std::cout << "Last World: " << world->get_h().c_str() << "\n" << std::endl;
	}
	else std::cout << "Save.dat not found!" << std::endl;
}