#DiscountCpp

C++11 wrapper for the markdown library "Discount"

##Global functions (fire and forget)

Easy-to-use functions that convert Markdown strings/files to HTML strings/files.

	namespace discountcpp
	{
		// Returns a string containing HTML code converted from an input string containing Markdown code
		std::string getHTMLFromMarkdownString(const std:string& mMarkdownString);

		// Returns a string containing HTML code converted from an input path to a Markdown file
		std::string getHTMLFromMarkdownFile(const std::string& mMarkdownFilePath);

		// Writes an HTML file to an input path from an input string containing Markdown code
		void writeHTMLFileFromMarkdownString(const std::string& mHTMLFilePath, const std::string& mMarkdownString);
		
		// Writes an HTML file to an input path from an input path to a Markdown file
		void writeHTMLFileFromMarkdownFile(const std::string& mHTMLFilePath, const std::string& mMarkdownFilePath);
	}

Example usage:

	std::string writeBlogHTML()
	{
		std::string resultPage;

		for(const auto& markdownEntryPath : getBlogMarkdownEntryPaths())	
			resultPage += discountcpp::getHTMLFromMarkdownFile(markdownEntryPath);
		
		return resultPage;
	}
