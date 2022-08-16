
class CreateWindowFailed : public std::exception {
	public:
		char* what() { return "Failed to create window."; }
};