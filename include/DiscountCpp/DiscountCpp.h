#ifndef DISCOUNTCPP
#define DISCOUNTCPP

#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
			if(fseek(mFilePtr, 0, SEEK_END)) return -1;
			long size = ftell(mFilePtr);
			if(size < 0) return -1;
			if(fseek(mFilePtr, 0, SEEK_SET)) return -1;
			return size;
		}

		size_t filePtrContentsToCString(char** mCString, FILE* mFilePtr, bool& mOutError)
		{
			size_t read = 0;
			*mCString = NULL;

			if(mOutError) mOutError = 1;

			do
			{
				if(!mFilePtr) break;

				long size = getFilePtrSize(mFilePtr);
				if(size < 0) break;

				if(mOutError) mOutError = 0;

				*mCString = static_cast<char*>(malloc((size_t)size + 1));
				if(!*mCString) break;

				read = fread(*mCString, 1, (size_t)size, mFilePtr);
				(*mCString)[read] = 0;
				*mCString = static_cast<char*>(realloc(*mCString, read + 1));

				if(mOutError) mOutError = (size != (long)read);
			}
			while(0);

			return read;
		}

		std::string getFilePtrContents(FILE* mFile)
		{
			char* result; bool error;
			filePtrContentsToCString(&result, mFile, error);
			if(error) throw std::runtime_error("[DiscountCpp internal error] Error reading file ptr contents");
			return result;
		}
	}

	class Source
	{
		private:
			std::string source;

		public:
			inline void readFromFile(const std::string& mPath) { std::ifstream ifs; ifs.open(mPath); source = std::string{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()}; ifs.close(); }
			inline void readFromString(const std::string& mSource) { source = mSource; }
			inline MMIOT* getMMIOT() const { return mkd_string(source.c_str(), strlen(source.c_str()), 0); }
			inline const std::string& getSource() const { return source; }
	};

	class Document
	{
		private:
			MMIOT* mmiot;

		public:
			Document(const Source& mSource) : mmiot{mSource.getMMIOT()} { }
			~Document() { mkd_cleanup(mmiot); }

			void writeHTMLToFile(const std::string& mPath)
			{
				FILE* file{fopen(mPath.c_str(), "w")};
				markdown(mmiot, file, 0);
				fclose(file);
			}
			void writeHTMLToString(std::string& mString)
			{
				FILE* file{tmpfile()};
				markdown(mmiot, file, 0);
				mString = Internal::getFilePtrContents(file);
				fclose(file);
			}
	};

	inline std::string getHTMLFromMarkdownString(const std::string& mMarkdownString)
	{
		std::string result;
		Source src; src.readFromString(mMarkdownString);
		Document{src}.writeHTMLToString(result);
		return result;
	}

	inline std::string getHTMLFromMarkdownFile(const std::string& mMarkdownFilePath)
	{
		std::string result;
		Source src; src.readFromFile(mMarkdownFilePath);
		Document{src}.writeHTMLToString(result);
		return result;
	}

	inline void writeHTMLFileFromMarkdownString(const std::string& mHTMLFilePath, const std::string& mMarkdownString)
	{
		std::ofstream ofs{mHTMLFilePath};
		ofs << getHTMLFromMarkdownString(mMarkdownString);
		ofs.flush(); ofs.close();
	}

	inline void writeHTMLFileFromMarkdownFile(const std::string& mHTMLFilePath, const std::string& mMarkdownFilePath)
	{
		std::ofstream ofs{mHTMLFilePath};
		ofs << getHTMLFromMarkdownFile(mMarkdownFilePath);
		ofs.flush(); ofs.close();
	}
}

#endif
