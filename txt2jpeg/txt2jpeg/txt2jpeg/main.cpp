#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
	ifstream in("D:\\GraduationProject\\QTtest\\SerialPort\\SerialPort\\build-SerialPort-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug\\imagedata.txt");
	ofstream out("out.jpg",ios::binary);
	string temp;
	bool triger=false;
	unsigned char input_data=0;
	char* buffer = new char[500000];
	int counter=0;

	while (getline(in, temp)) {
		if (triger == true) {
			//out << temp;
			input_data = 0;
			if (temp.length() == 1) {
				if (temp[0] <= 'f'&&temp[0] >= 'a') {
					input_data = input_data + (temp[0] - 'a' + 10);
				}
				else if (temp[0] <= '9'&&temp[0] >= '0') {
					input_data = input_data + (temp[0] - '0');
				}
			}
			else if (temp.length() == 2) {
				if (temp[0] <= 'f'&&temp[0] >= 'a') {
					input_data = input_data + (temp[0] - 'a' + 10) * 16;
				}
				else if (temp[0] <= '9'&&temp[0] >= '0') {
					input_data = input_data + (temp[0] - '0') * 16;
				}
				if (temp[1] <= 'f'&&temp[1] >= 'a') {
					input_data = input_data + (temp[1] - 'a' + 10);
				}
				else if (temp[1] <= '9'&&temp[1] >= '0') {
					input_data = input_data + (temp[1] - '0');
				}
			}
			buffer[counter] = input_data;
			counter++;
		}
		if (temp == "IDS\r")	triger = true;
	}
	out.write(buffer, counter);
	out.close();
	return 0;
}