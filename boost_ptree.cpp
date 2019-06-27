#include <iostream>
#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace bp = boost::property_tree;


extern const char* exchangeRatesXML;

int main()
{
    // Use a string stream instead of a file stream.
    // std::ifstream("/path/to/data.xml")
    std::stringstream ss(exchangeRatesXML);

    bp::ptree tree;
    bp::read_xml(ss, tree);

    /* Value of Node N -1 = tree.get<NODE TYPE>("<Node Path>")
     * Node Path = Node0.Node1.Node2.Node3 .... Node N-1
     *
     * XML:
     *  <Node0
     *    <Node1
     *      <Node2 ... <Node N -1 > Value of Node N - 1 </Node N-1> </ ...>
     *
     */
    auto subject = tree.get<std::string>("gesmes:Envelope.gesmes:subject");
    std::cout << "Subject = " << subject << std::endl;

    auto sender_name = tree.get<std::string>("gesmes:Envelope.gesmes:Sender.gesmes:name");
    std::cout << "Sende_name = " << sender_name << std::endl;

    auto time = tree.get<std::string>("gesmes:Envelope.Cube.Cube.<xmlattr>.time");
    std::cout << "time = " << time << std::endl;

#if 0
    for(bp::ptree::value_type const & x : tree.get_child("gesmes:Envelope.Cube.Cube.Cube.<xmlattr>.currency"))
    {
     //   auto currency = x.second.get_child("Cube.<xmlattr>.currency");
                        // second.get<std::string>("Cube");
        std::cout << " Currency = " << x.first << std::endl;
    }
#endif

    std::cout << " ======= Second Loop =============" << std::endl;

    for(auto const & x : tree.get_child("gesmes:Envelope.Cube.Cube"))
    {

        //   auto currency = x.second.get_child("Cube.<xmlattr>.currency");
        // second.get<std::string>("Cube");
        std::cout << " XML tag = " << x.first  << x.second.data() << std::endl;
//        std::cout << " Currency = " << x.second.get<std::string>("<xmlattr>.currency") << std::endl;
    }


    return 0;
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
                           <FXRate currency='USD' rate='1.1265'/>
                           <FXRate currency='JPY' rate='121.90'/>
                           <FXRate currency='BGN' rate='1.9558'/>
                           <FXRate currency='CZK' rate='25.540'/>
                           <FXRate currency='DKK' rate='7.4676'/>
                           <FXRate currency='GBP' rate='0.89093'/>
                           <FXRate currency='HUF' rate='321.53'/>
                           <FXRate currency='PLN' rate='4.2534'/>
                           <FXRate currency='RON' rate='4.7233'/>
                           <FXRate currency='SEK' rate='10.6390'/>
                           <FXRate currency='CHF' rate='1.1211'/>
                           <FXRate currency='ISK' rate='141.50'/>
                           <FXRate currency='NOK' rate='9.7728'/>
                           <FXRate currency='HRK' rate='7.4105'/>
                           <FXRate currency='RUB' rate='72.3880'/>
                           <FXRate currency='TRY' rate='6.6427'/>
                           <FXRate currency='AUD' rate='1.6324'/>
                           <FXRate currency='BRL' rate='4.3423'/>
                           <FXRate currency='CAD' rate='1.5018'/>
                           <FXRate currency='CNY' rate='7.7997'/>
                           <FXRate currency='HKD' rate='8.8170'/>
                           <FXRate currency='IDR' rate='16128.10'/>
                           <FXRate currency='ILS' rate='4.0518'/>
                           <FXRate currency='INR' rate='78.6080'/>
                           <FXRate currency='KRW' rate='1333.60'/>
                           <FXRate currency='MXN' rate='21.6073'/>
                           <FXRate currency='MYR' rate='4.6981'/>
                           <FXRate currency='NZD' rate='1.7241'/>
                           <FXRate currency='PHP' rate='58.539'/>
                           <FXRate currency='SGD' rate='1.5403'/>
                           <FXRate currency='THB' rate='35.101'/>
                           <FXRate currency='ZAR' rate='16.6529'/>
                   </Cube>
           </Cube>
   </gesmes:Envelope>
)";
