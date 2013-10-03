#ifndef DISCOUNTCPP
#define DISCOUNTCPP

#include <string>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
extern "C"
{
	#include <mkdio.h>
}

namespace discountcpp
{
	namespace Internal
	{
		long getFilePtrSize(FILE* mFilePtr)
		{
			if(fseek(mFilePtr, 0, SEEK_END)) throw std::runtime_error("[DiscountCpp internal error] Error reading file ptr size");
			auto size(ftell(mFilePtr));
			if(size < 0 || fseek(mFilePtr, 0, SEEK_SET)) throw std::runtime_error("[DiscountCpp internal error] Error reading file ptr size");
			return size;
		}

		std::size_t filePtrContentsToCString(char** mCString, FILE* mFilePtr)
		{
			std::size_t read{0};
			*mCString = nullptr;

			if(!mFilePtr) return read;

			long size{getFilePtrSize(mFilePtr)};
			if(size < 0) return read;

			*mCString = static_cast<char*>(malloc((std::size_t)size + 1));
			if(!*mCString) return read;

			read = fread(*mCString, 1, (std::size_t)size, mFilePtr);
			(*mCString)[read] = 0;
			*mCString = static_cast<char*>(realloc(*mCString, read + 1));

			if(size != static_cast<long>(read)) throw std::runtime_error("[DiscountCpp internal error] Error reading file ptr contents");

			return read;
		}

		inline std::string getFilePtrContents(FILE* mFilePtr) { char* result; filePtrContentsToCString(&result, mFilePtr); return result; }
		inline std::string readFile(const std::string& mPath)
		{
			std::ifstream ifs{mPath};
			return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
		}
	}

	class Document;

	class Source
	{
		friend class Document;

		private:
			std::string source;
			MMIOT* mmiot{nullptr};

			inline void refreshMMIOT()
			{
				if(mmiot != nullptr) mkd_cleanup(mmiot);
				const auto& cstr(source.c_str());
				mmiot = mkd_string(cstr, strlen(cstr), 0);
			}
			inline MMIOT* getMMIOT() const
			{
				if(mmiot == nullptr) throw std::runtime_error("[DiscountCpp internal error] Source mmiot is null");
				return mmiot;
			}

		public:
			~Source() { if(mmiot != nullptr) mkd_cleanup(mmiot); }

			inline static Source fromFile(const std::string& mPath)		{ Source result; result.readFromFile(mPath); return result; }
			inline static Source fromString(const std::string& mStr)	{ Source result; result.readFromString(mStr); return result; }

			inline void readFromFile(const std::string& mPath)		{ source = Internal::readFile(mPath); refreshMMIOT(); }
			inline void readFromString(const std::string& mSource)	{ source = mSource; refreshMMIOT(); }
			inline const std::string& getSource() const				{ return source; }
	};

	class Document
	{
		private:
			const Source& source;

		public:
			Document(const Source& mSource) : source(mSource) { }

			void writeHTMLToFile(const std::string& mPath)
			{
				FILE* file{fopen(mPath.c_str(), "w")};
				markdown(source.getMMIOT(), file, 0);
				fclose(file);
			}
			void writeHTMLToString(std::string& mStr)
			{
				FILE* file{tmpfile()};
				markdown(source.getMMIOT(), file, 0);
				mStr = Internal::getFilePtrContents(file);
				fclose(file);
			}
	};

	namespace Internal
	{
		inline std::string getHTMLFromMarkdownSource(const Source& mSource)				{ std::string result; Document{mSource}.writeHTMLToString(result); return result; }
		inline void writeToFile(const std::string& mPath, const std::string& mContents)	{ std::ofstream ofs{mPath}; ofs << mContents; ofs.flush(); ofs.close(); }
	}

	inline std::string getHTMLFromMarkdownString(const std::string& mMarkdownString)	{ return Internal::getHTMLFromMarkdownSource(Source::fromString(mMarkdownString)); }
	inline std::string getHTMLFromMarkdownFile(const std::string& mMarkdownFilePath)	{ return Internal::getHTMLFromMarkdownSource(Source::fromFile(mMarkdownFilePath)); }
	inline void writeHTMLFileFromMarkdownString(const std::string& mHTMLFilePath, const std::string& mMarkdownString)	{ Internal::writeToFile(mHTMLFilePath, getHTMLFromMarkdownString(mMarkdownString)); }
	inline void writeHTMLFileFromMarkdownFile(const std::string& mHTMLFilePath, const std::string& mMarkdownFilePath)	{ Internal::writeToFile(mHTMLFilePath, getHTMLFromMarkdownFile(mMarkdownFilePath)); }
}

#endif
