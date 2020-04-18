#include "rc_sample.h"
#include "../../networking/connection/http_server.h"
#include "../../input/input.h"
//STD includes
#include <chrono>
#include <thread>
#include <iostream>
#include <cstring>

application* application::mf_factory()
{
   return new rc_sample();
}
namespace net
{
   class test_handler : public http_server::handler
   {
   public:
      void mf_get_response(const char* request, http_server::handler::send_callback callback) override
      {
	 if(strcmp(request, "custom") == 0)
	 {
	    callback("you suck", sizeof("you suck"));
	 }
      }
      void mf_ws_response(const char* data, http_server::handler::send_callback callback) override
      {
	 if(strcmp(data, "hello") == 0)
	 {
	    printf("handling: %s\n", data);
	    callback("hi there", sizeof("hi there"));
	 }
      }
      void mf_ws_send(http_server::handler::send_callback callback) override
      {
	 if(mv_counter % 10 == 0) { callback("ten", sizeof("ten")); }
	 if(mv_counter % 100 == 0) { callback("a hundred", sizeof("a hundred")); }
	 if(mv_counter % 1000 == 0) { callback("a thousand", sizeof("a thousand")); }
	 mv_counter++;
      }
      const char* mf_get_response_root_dir() const override
      {
	 return "";
      }
      int mv_counter = 0;
   };
}
void rc_sample::mf_run(void)
{
   input* l_input = input::inputFactory();
   l_input->init();
   net::http_server l_http_server(8000);
   net::test_handler lv_handler;
   l_http_server.mf_set_handler(&lv_handler);
   std::cout << "entering app loop\n";
   bool l_looping = true;
   while(l_looping)
   {
      if(l_input->update()) l_looping = false;
      std::this_thread::sleep_for(std::chrono::milliseconds(30));
   }
   delete l_input;
   std::cout << "ending app loop\n";
}