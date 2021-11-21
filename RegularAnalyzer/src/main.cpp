#include "regular.hpp"

int main()
{
	Regular regular1("a.b.#|c(3:mephi|msu|mipt)(mephi|msu)\\3");
	Regular regular2("abcd(1:mephi)hg\\1");
	Regular regular4("a{0,}");
	Regular regular5("a(e){0,}b|c(e){0,}b"); // example for minimization

	Regular sub_1("abc|def");
	Regular sub_2("abc");

	Regular inv_1("m+|e+p+(h|i)");

	try {
		/*regular4.compile();
		Regular regular6(regular4.getRE());
		std::cout << regular4.getRE() << std::endl;
		regular6.compile();*/

		/*Regular regular11("mephi|^");
		Regular regular12("^");
		Regular sub_res("");
		sub_res.subtraction(regular11, regular12);
		std::vector<std::string> captures;
		std::vector<std::string> strings;
		std::string str = "mephi";
		sub_res.findAll(str, strings);
		for (size_t i = 0; i < strings.size(); i++) {
			std::cout << "String: " << strings[i] << std::endl;
		}*/

		/*Regular regular11("(1:(m(3:e))ph)i");
		std::string str = "mephi";
		std::vector<std::string> captures;
		std::vector<std::string> strings;
		regular11.compile();
		regular11.match(str, captures) ? std::cout << "Acceptable" : std::cout << "Not cceptable";
		std::cout << std::endl;
		for (size_t i = 0; i < captures.size(); i++) {
			std::cout << "Capture: " << captures[i] << std::endl;
		}*/

		/*Regular inv_res("");
		inv_res.inversion(inv_1);
		std::string str = "ippppee";
		inv_res.match(str) ? std::cout << "Acceptable" : std::cout << "Not cceptable";*/
		
		/*std::string str = "abcdmephihgmephidsfabcdmephihgmephiabcdmephihgmephi";
		std::vector<std::string> captures;
		std::vector<std::string> strings;
		regular2.compile();
		regular2.findAll(str, strings, captures);
		regular2.match(str, captures) ? std::cout << "Acceptable" : std::cout << "Not cceptable";
		std::cout << std::endl << std::endl;

		for (size_t i = 0; i < strings.size(); i++) {
			std::cout << "String: " << strings[i] << std::endl;
		}
		std::cout << std::endl;
		for (size_t i = 0; i < captures.size(); i++) {
			std::cout << "Capture: " << captures[i] << std::endl;
		}*/


		/*Regular sub_res("");
		sub_res.subtraction(sub_1, sub_2);*/

		/*Regular inv_res("");
		inv_res.inversion(inv_1);
		std::string str = "ecbacba";
		inv_res.match(str) ? std::cout << "Acceptable" : std::cout << "Not cceptable";*/
	}
	catch (const char* ex) {
		std::cerr << ex << std::endl;
	}

	return 0;
}
