#ifndef httpportal_h
#define httpportal_h

// @USES wifimanager portal

#include <log.h>
#include <WiFiManager.h>

// WiFiManager portal(Logger);
WiFiManager portal;

void begin_httpportal(){
	portal.startWebPortal();
}

void stop_httpportal(){
	portal.stopWebPortal();	
}

void process_httpportal(){
	portal.process();
}

void init_httpportal(String title = "",bool begin = true){
  portal.setDebugOutput(true,"[HTTPP] ");
  // invert theme, dark
  portal.setDarkMode(true);
  std::vector<const char *> menu = {"wifi","param","info","sep","update","restart","exit"};
  portal.setMenu(menu); // custom menu, pass vector
  portal.setTitle(title);
  // portal.setBackButton(true);
  // portal.setHostname(getHostname());
  if(begin) begin_httpportal();
}

void begin_httpportalap(){
  portal.startConfigPortal();
}

#endif
