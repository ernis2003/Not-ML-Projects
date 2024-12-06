#include <iostream>
#include <string>

using namespace std;

class ReadFile {
	private:
		string filePath_;

	enum readedFile {
		Read,
		Unread
	};

	public:
		ReadFile(string filePath) {
			filePath_ = filePath;
		}
};