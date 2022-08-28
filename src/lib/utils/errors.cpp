
class CreateWindowFailed : public std::exception {
	public:
		char* what() { return (char*)"Failed to create window."; }
};

class CheckGladFailed : public std::exception {
	public:
		char* what() { return (char*)"Failed check glad."; }
};