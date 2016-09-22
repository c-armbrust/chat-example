#include <iostream>
#include <chrono>
#include <thread>

#include "sio_client.h"
#include "sio_message.h"
#include "sio_socket.h"

using namespace sio;
using namespace std;

bool connect_finish = false;

class connection_listener
{
    sio::client &handler;

public:
    
    connection_listener(sio::client& h):
    handler(h)
    {
    }
    

    void on_connected()
    {
		std::cout<<"sio connected "<<std::endl;
		connect_finish = true;
    }
    void on_close(client::close_reason const& reason)
    {
        std::cout<<"sio closed "<<std::endl;
        exit(0);
    }
    
    void on_fail()
    {
        std::cout<<"sio failed "<<std::endl;
        exit(0);
    }
};

int main()
{
	cout << "C++ socket.io client" << endl;
	socket::ptr current_socket;

	sio::client h;
	connection_listener l(h);
    h.set_open_listener(std::bind(&connection_listener::on_connected, &l));
    h.set_close_listener(std::bind(&connection_listener::on_close, &l,std::placeholders::_1));
	h.set_fail_listener(std::bind(&connection_listener::on_fail, &l));
	h.connect("http://127.0.0.1:3000");
	while(!connect_finish) {
		cout << ".";
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	current_socket = h.socket();
	current_socket->on("chat message", [&](sio::event& ev)
	{
		cout << "on chat message handler" << endl;
    	//handle login message
    	//post to UI thread if any UI updating.
	});	

	string msg = "2014_10_15_11_09_25_71.jpg";
	cout << "send message: " << endl;
	current_socket->emit("chat message", msg);

	std::cin.ignore();

return 0;
}
