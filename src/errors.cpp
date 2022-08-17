
class CreateWindowFailed : public std::exception {
	public:
		char* what() { return "Failed to create window."; }
};

class CheckGladFailed : public std::exception {
	public:
		char* what() { return "Failed check glad."; }
};

class CompileShaderFailed : public std::exception {
	public:
		char* what() { return "Failed shader compilation."; }
};

class LinkShaderProgramFailed : public std::exception {
	public:
		char* what() { return "Failed linking shader program."; }
};