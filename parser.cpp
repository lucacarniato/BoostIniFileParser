/* 
A ini file parser based on boost libraries. It creates a tree structure and parses the ini file.
prop_get templated function does the work of extracting the numbers(integer, float or double) from the strings
using c++11 regular expression functionality.
*/

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>
#include <type_traits>
#include <regex>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

struct Getter
{
   template<class T, class Ptree>
   static void prop_get(Ptree &pt, const string & chapter, const string & key, vector<T> & results) 
   {
      string  query{ chapter + '.' + key };
      string resultString = pt.get<string>(query);
      regex rex;
      if(std::is_same<T, int>::value)
         rex = "(\\+|-)?[[:digit:]]+";
      if (std::is_same<T, float>::value)
         rex = "((\\ + | -) ? [[:digit:]] + )(\\.(([[:digit:]] + ) ? )) ? ";
      if (std::is_same<T, double>::value)
         rex = "((\\ + | -) ? [[:digit:]] + )(\\.(([[:digit:]] + ) ? )) ? ((e | E)((\\ + | -) ? )[[:digit:]] + ) ? ";
      smatch match;
      while (regex_search(resultString, match, rex))
      {
         T number = boost::lexical_cast<T>(match[0]);
         results.push_back(number);
         resultString = match.suffix().str();
      }
   }
};


int main()
{
   boost::property_tree::ptree pt;
   string file = "test.ini";
   boost::property_tree::read_ini(file, pt);
   
   vector<int> intResults;
   Getter::prop_get(pt, "Person1", "Numbers", intResults);
   cout << "Person1 ";
   for (const auto& n : intResults) cout << " "<<n;
   cout << endl;
   intResults.clear();

   Getter::prop_get(pt, "Person2", "Numbers", intResults);
   cout << "Person2 ";
   for (const auto& n : intResults) cout << " " << n;
   cout << endl;

   getchar();
   return 0;
}