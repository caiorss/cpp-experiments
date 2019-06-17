#include <iostream>
#include <iomanip>

#include <tinyxml2.h>

#define ENABLE_ASSERT

#ifdef ENABLE_ASSERT
#define M_ASSERT(expr) \
      { \
         if(!(expr)){ \
            std::cerr << "ASSERTION FAILURE: \n"; \
            std::cerr << " => Condition: " << #expr << "\n"; \
            std::cerr << " =>  Function: " << __FUNCTION__ << "\n"; \
            std::cerr << __FILE__ << ":" << __LINE__ << ":" << "\n"; \
            std::terminate(); \
         } \
      }
#else
#define M_ASSERT(expr)
#endif

using tinyxml2::XMLText;
using tinyxml2::XMLElement;
using tinyxml2::XMLNode;

extern const char* exchangeRatesXML;

int main()
{

    std::cout << " [INFO] Running TinyXMl2 " << std::endl;

    tinyxml2::XMLDocument doc;

    if(doc.Parse( exchangeRatesXML) != tinyxml2::XML_SUCCESS)
    {
        std::cout << " [ERROR] Failed to parse XML" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << " [OK] XML parsed successfully" << std::endl;


    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);

    std::cout << "Value: doc.FirstChild()->Value() = " << doc.FirstChild()->Value() << std::endl;

    XMLElement* elem = doc.FirstChildElement("gesmes:Envelope");
    M_ASSERT(elem != nullptr);
    if(elem){
        std::cout << " Element found. OK " << std::endl;
        std::cout << " =>> Element Name = " << elem->Name() << std::endl;

    }

    XMLElement* elem1 = elem->FirstChildElement("Cube");
    M_ASSERT(elem1 != nullptr);

    std::cout << " =>> Found Node Name: " << elem1->ToElement()->Name() << "\n";

    XMLElement* elem2 = elem1->FirstChildElement("Cube");
    M_ASSERT(elem2 != nullptr);

    const char* time = elem2->Attribute("time");
    M_ASSERT(time != nullptr);

    // XML node with: <Cube time = 'xxxx-xx-xx'>
    std::cout << " => Time = " << time << "\n\n";
    std::cout << std::fixed << std::setprecision(3);

    std::cout << " ===== Exchange rates in Euros ====" << std::endl;

    for(XMLElement* e = elem2->FirstChildElement("Cube")
             ; e != nullptr; e = e->NextSiblingElement("Cube") )
    {
        std::cout << std::setw(10) << e->Attribute("currency")
                  << std::setw(15) << std::stod(e->Attribute("rate"))
                  << std::endl;
    }


    return doc.ErrorID();
}

// Source: https://www.ecb.europa.eu/stats/eurofxref/eurofxref-daily.xml
const char* exchangeRatesXML = R"(
   <?xml version="1.0" encoding="UTF-8"?>
   <gesmes:Envelope 
                xmlns:gesmes="http://www.gesmes.org/xml/2002-08-01" 
                xmlns="http://www.ecb.int/vocabulary/2002-08-01/eurofxref">

	   <gesmes:subject>Reference rates</gesmes:subject>
	   <gesmes:Sender>
		   <gesmes:name>European Central Bank</gesmes:name>
	   </gesmes:Sender>
	   <Cube>
		   <Cube time='2019-06-14'>
			   <Cube currency='USD' rate='1.1265'/>
			   <Cube currency='JPY' rate='121.90'/>
			   <Cube currency='BGN' rate='1.9558'/>
			   <Cube currency='CZK' rate='25.540'/>
			   <Cube currency='DKK' rate='7.4676'/>
			   <Cube currency='GBP' rate='0.89093'/>
			   <Cube currency='HUF' rate='321.53'/>
			   <Cube currency='PLN' rate='4.2534'/>
			   <Cube currency='RON' rate='4.7233'/>
			   <Cube currency='SEK' rate='10.6390'/>
			   <Cube currency='CHF' rate='1.1211'/>
			   <Cube currency='ISK' rate='141.50'/>
			   <Cube currency='NOK' rate='9.7728'/>
			   <Cube currency='HRK' rate='7.4105'/>
			   <Cube currency='RUB' rate='72.3880'/>
			   <Cube currency='TRY' rate='6.6427'/>
			   <Cube currency='AUD' rate='1.6324'/>
			   <Cube currency='BRL' rate='4.3423'/>
			   <Cube currency='CAD' rate='1.5018'/>
			   <Cube currency='CNY' rate='7.7997'/>
			   <Cube currency='HKD' rate='8.8170'/>
			   <Cube currency='IDR' rate='16128.10'/>
			   <Cube currency='ILS' rate='4.0518'/>
			   <Cube currency='INR' rate='78.6080'/>
			   <Cube currency='KRW' rate='1333.60'/>
			   <Cube currency='MXN' rate='21.6073'/>
			   <Cube currency='MYR' rate='4.6981'/>
			   <Cube currency='NZD' rate='1.7241'/>
			   <Cube currency='PHP' rate='58.539'/>
			   <Cube currency='SGD' rate='1.5403'/>
			   <Cube currency='THB' rate='35.101'/>
			   <Cube currency='ZAR' rate='16.6529'/>
		   </Cube>
	   </Cube>
   </gesmes:Envelope>
)";
