#ifndef test
#define test

#include "../binance_bybit_arb/utils/Binance_Websocket_Exchange.h"

string remove_quotes_around_id_in_json_string(string &s){

    string temp= "";
    int siz= (int)s.size();
    for(int i=0;i<siz;){
        if(s[i]=='"' && i+2<siz && s[i+1]=='1' && s[i+2]=='"'){
            temp.push_back('1');
            i+= 3;
            continue;
        }
        temp.push_back(s[i]);
        i++;
    }

    return temp;
}

string get_json_string_payload_partial_orderbook(string coin){

    boost::property_tree::ptree param,pt;
    boost::property_tree::ptree paramsArray;
    paramsArray.push_back(std::make_pair("", boost::property_tree::ptree(coin + "@depth5")));
    unsigned int id_val= 1;
    pt.put("method", "SUBSCRIBE");
    pt.put("id", id_val);

    pt.put_child("params", paramsArray);

    try{
        std::ostringstream oss;
        boost::property_tree::json_parser::write_json(oss, pt, true);
        std::string jsonString = oss.str();
        return remove_quotes_around_id_in_json_string(jsonString);
    }
    catch(exception const &e){
        return "";
    }
}

void get_bin_partial_orderbook(string coin)
{

    while (true) {
        try {

            string binance_partial_orderbook_1 = "";
            Binance_Websocket_Exchange Binance{"binance", "fstream.binance.com"};

            try{
                Binance.init_webSocket("fstream.binance.com", "443", "/ws");
            }
            catch(std::exception const &e){
                continue;
            }

            if (Binance.is_socket_open()) {
                string req_data= get_json_string_payload_partial_orderbook(coin);
                Binance.write_Socket(req_data.c_str());
            }

            // int port= 0;
            // if(coin=="fdusdusdt") port= binance_partial_orderbook_fdusdusdt_port;
            // if(coin=="usdcusdt") port= binance_partial_orderbook_usdcusdt_port;
            // UDPClient udpClient(port);


            while (true) {

                try{

                    if(!Binance.is_socket_open()) break;
                    Binance.read_Socket();
                    string binance_partial_orderbook_1 = Binance.get_socket_data();
                    Binance.buffer_clear();

                    std::cout<<"Received data: "<<binance_partial_orderbook_1<<std::endl;
                    
                    // udpClient.send_data_async(binance_partial_orderbook_1.c_str(), strlen(binance_partial_orderbook_1.c_str()));

                    /*boost::property_tree::ptree json_data;
                    std::istringstream iss(binance_partial_orderbook_1);
                    boost::property_tree::read_json(iss, json_data);

                    if(json_data.find("lastUpdateId")==json_data.not_found()){
                        
                        continue; 
                    } 
                
                    double bid = stringToDouble(json_data.get_child("bids").front().second.front().second.get_value<std::string>());
                    double ask = stringToDouble(json_data.get_child("asks").front().second.front().second.get_value<std::string>());
                    
                    // a1= 0;
                    // b1= 0;
                    // a2= fdusdusdt_ask;
                    // b2= fdusdusdt_bid;

                    concurrent_vector<double> temp= {0,0,ask,bid};
                    binance_partial_orderbook_setter(temp,coin);*/
                    
                }
                catch (std::exception const& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }

                // cout<<"BINANCE PARTIAL ORDERBOOK PRICES: "<<fixed<<setprecision(6)<<coin<<"_ask: "<<temp[2]<<" "<<coin<<"_bid: "<<temp[3]<<"\n";
            }
        }
        catch (std::exception const& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}


#endif