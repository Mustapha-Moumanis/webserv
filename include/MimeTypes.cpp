#include "MimeTypes.hpp"

std::string MimeTypes::getType(const std::string &extension) {
	int i = -1;

	while (++i <= 32) {
		if (extension == types[i].fileExtension)
			return types[i].mimeType;
	}
	return "";
}

std::string MimeTypes::getExtension(const std::string &type) {
	int i = -1;

	while (++i <= 32) {
		if (type == types[i].mimeType)
		return types[i].fileExtension;
	}
	return "";
}

MimeTypes::entry MimeTypes::types[32] =  {
	{"text", "text/plain"},
	{"txt", "text/plain"},
	{"htm", "text/html"},
	{"html", "text/html"},
	{"css", "text/css"},
	{"js", "application/javascript"},
	{"yml", "text/yaml"},
	{"json", "application/json"},
	{"jpg", "image/jpeg"},
	{"jpeg", "image/jpeg"},
	{"jpe", "image/jpeg"},
	{"png", "image/png"},
	{"apng", "image/apng"},
	{"svg", "image/svg+xml"},
	{"pdf", "application/pdf"},
	{"gif", "image/gif"},
	{"*mp3", "audio/mp3"},
	{"mp3", "audio/mpeg"},
	{"mp4", "video/mp4"},
	{"weba", "audio/webm"},
	{"*rtf", "text/rtf"},
	{"*xml", "text/xml"},
	{"xml", "application/xml"},
	{"xht", "application/xhtml+xml"},
	{"xhtml", "application/xhtml+xml"},
	{"atom", "application/atom+xml"},
	{"csv", "text/csv"},
	{"markdown", "text/markdown"},
	{"md", "text/markdown"},
	{"rss", "application/rss+xml"},
	{"zip", "application/zip"},
	{"", "application/x-www-form-urlencoded"},
};