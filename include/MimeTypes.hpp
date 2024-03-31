#ifndef MIMETYPES_HPP
#define MIMETYPES_HPP
#include <cstring>
#include <string>

class MimeTypes {
  public:
	static std::string getType(const std::string &extension);
	static std::string getExtension(const std::string &type);

  private:
	struct entry {
		std::string fileExtension;
		std::string mimeType;
	};

	static MimeTypes::entry types[32];
};

#endif