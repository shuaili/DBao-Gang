#include "ParserSax.h"

#include <fstream>

std::ofstream output;

void htmlcxx::HTML::ParserSax::parse(const std::string &html, std::string &filename)
{
//	std::cerr << "Parsing string" << std::endl;
		std::string inputfile_name(filename);
		std::string outputfile_name = inputfile_name+".an";
		output.open(outputfile_name.c_str());

	parse(html.c_str(), html.c_str() + html.length());
}
