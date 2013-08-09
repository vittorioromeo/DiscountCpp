#DiscountCpp

C++11 wrapper for the markdown library "Discount"

##Global functions (fire and forget)

Easy-to-use functions that convert Markdown strings/files to HTML strings/files.

```cpp
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
```

Example usage:

```cpp
std::string writeBlogHTML()
{
	std::string resultPage;

	for(const auto& markdownEntryPath : getBlogMarkdownEntryPaths())	
		resultPage += discountcpp::getHTMLFromMarkdownFile(markdownEntryPath);
		
	return resultPage;
}
```

##Objects

```cpp
namespace discountcpp
{
	class Source; // Markdown source 
	class Document; // Markdown document
}
```

`discountcpp::Source` can read Markdown code from strings or files.

`discountcpp::Document` must be constructed with a `discountcpp::Source`, and can generate HTML strings or files.

Example usage:
```cpp
std::string writeBlogHTML()
{
	std::string resultPage;

	for(const auto& markdownEntryPath : getBlogMarkdownEntryPaths())	
	{
		discountcpp::Source source;
		source.readFromFile(markdownEntryPath);

		// Using the same document to output both to string and both to file
		discountcpp::Document document{source};
		string outputHTML;
		document.writeHTMLToString(outputHTML);
		document.writeHTMLToFile(markdownEntryPath + ".tmp.html");

		resultPage += outputHTML;
	}
		
	return resultPage;
}
```