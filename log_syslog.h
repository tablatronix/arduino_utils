#ifndef log_syslog_h
#define log_syslog_h

#include <log.h>
#include <Syslog.h>
#include <WiFiUdp.h>
#include <creds.h>
#include <wifi_funcs.h>


// @todo add print println stream capability
// add manual flush, ammend to buffer send buffer at once instead of real time, or batch sending.

WiFiUDP udpClient;

Syslog syslog(udpClient, SYSLOG_PROTO_IETF); // SYSLOG_PROTO_BSD
// Syslog syslog(udpClient, SYSLOG_SERVER, SYSLOG_PORT, DEVICE_HOSTNAME, APP_NAME, LOG_KERN);

String logTopic = "ESP";
int iteration = 1;

#define DEVICE_HOSTNAME "my-device"
#define APP_NAME "my-app"

const char* syslog_hostname;
const char* syslog_appname;

void init_syslog(const char* hostname){
  Serial.println("[LOG] syslog init");
  Serial.println("[LOG] syslog devicename: " + (String)hostname);
  Serial.println("[LOG] syslog hostname: " + getHostname());
  Serial.println("[LOG] syslog appname: " + logTopic);

  // syslog_hostname = getHostname().c_str(); // gibberish not working
  syslog_hostname = hostname;
   // : getHostname().c_str();
  syslog_appname  = logTopic.c_str();
  // prepare syslog configuration here (can be anywhere before first call of 
	// log/logf method)
  syslog.server(syslog_server_ip, syslog_server_port);
  syslog.deviceHostname(syslog_hostname);
  syslog.appName(syslog_appname);
  // syslog.deviceHostname(DEVICE_HOSTNAME);
  // syslog.appName(APP_NAME);
  syslog.defaultPriority(LOG_INFO);
}

void sendLogTest(){
  String startmsg = "[BOOT] Device Started";
  syslog.log(LOG_INFO, startmsg);
  return;
  // Log message can be formated like with printf function.
  syslog.logf(LOG_ERR,  "This is error message no. %d", iteration);
  syslog.logf(LOG_INFO, "This is info message no. %d", iteration);

  // You can force set facility in pri parameter for this log message. More 
  // facilities in syslog.h or in Linux syslog documentation.
  syslog.logf(LOG_DAEMON | LOG_INFO, "This is daemon info message no. %d", iteration);

  // You can set default facility and severity level and use it for all log
  // messages (beware defaultPriority is stored permanently!)
  // syslog.defaultPriority(LOG_FTP | LOG_INFO);
  syslog.logf("This is ftp info message no. %d", iteration);

  // Send log messages up to LOG_WARNING severity
  syslog.logMask(LOG_UPTO(LOG_WARNING));
  syslog.log(LOG_ERR, "This is logged.");
  syslog.log(LOG_WARNING, "This is logged.");
  syslog.log(LOG_NOTICE, "This is not logged.");
  syslog.log(LOG_INFO, "This is not logged.");

  // F() macro is supported too
  syslog.log(LOG_INFO, F("End loop"));
  iteration++;
}

void sendSyslog(){
   syslog.log(LOG_INFO, F("End loop")); 
}

// // Syslog protocol format
// #define SYSLOG_PROTO_IETF 0  // RFC 5424
// #define SYSLOG_PROTO_BSD 1   // RFC 3164

// /*
//  * priorities/facilities are encoded into a single 32-bit quantity, where the
//  * bottom 3 bits are the priority (0-7) and the top 28 bits are the facility
//  * (0-big number).  Both the priorities and the facilities map roughly
//  * one-to-one to strings in the syslogd(8) source code.  This mapping is
//  * included in this file.
//  *
//  * priorities (these are ordered)
//  */
// #define LOG_EMERG 0 /* system is unusable */
// #define LOG_ALERT 1 /* action must be taken immediately */
// #define LOG_CRIT  2 /* critical conditions */
// #define LOG_ERR   3 /* error conditions */
// #define LOG_WARNING 4 /* warning conditions */
// #define LOG_NOTICE  5 /* normal but significant condition */
// #define LOG_INFO  6 /* informational */
// #define LOG_DEBUG 7 /* debug-level messages */

// #define LOG_PRIMASK 0x07  /* mask to extract priority part (internal) */
//         /* extract priority */
// #define LOG_PRI(p)  ((p) & LOG_PRIMASK)
// #define LOG_MAKEPRI(fac, pri) (((fac) << 3) | (pri))

// /* facility codes */
// #define LOG_KERN  (0<<3)   kernel messages 
// #define LOG_USER  (1<<3)  /* random user-level messages */
// #define LOG_MAIL  (2<<3)  /* mail system */
// #define LOG_DAEMON  (3<<3)  /* system daemons */
// #define LOG_AUTH  (4<<3)  /* security/authorization messages */
// #define LOG_SYSLOG  (5<<3)  /* messages generated internally by syslogd */
// #define LOG_LPR   (6<<3)  /* line printer subsystem */
// #define LOG_NEWS  (7<<3)  /* network news subsystem */
// #define LOG_UUCP  (8<<3)  /* UUCP subsystem */
// #define LOG_CRON  (9<<3)  /* clock daemon */
// #define LOG_AUTHPRIV  (10<<3) /* security/authorization messages (private) */
// #define LOG_FTP   (11<<3) /* ftp daemon */

// /* other codes through 15 reserved for system use */
// #define LOG_LOCAL0  (16<<3) /* reserved for local use */
// #define LOG_LOCAL1  (17<<3) /* reserved for local use */
// #define LOG_LOCAL2  (18<<3) /* reserved for local use */
// #define LOG_LOCAL3  (19<<3) /* reserved for local use */
// #define LOG_LOCAL4  (20<<3) /* reserved for local use */
// #define LOG_LOCAL5  (21<<3) /* reserved for local use */
// #define LOG_LOCAL6  (22<<3) /* reserved for local use */
// #define LOG_LOCAL7  (23<<3) /* reserved for local use */
// 
// System errors
// Example of a system error message:

// May 11 10:40:48 scrooge disk-health-nurse[26783]: [ID 702911 user.error] m:SY-mon-full-500 c:H : partition health measures for /var did not suffice - still using 96% of partition space

// The message can be split in to the following columns:

// Column 1 = "May 11 10:40:48"               > Timestamp
// Column 2 = "scrooge"                       > Loghost
// Column 3 = "disk-health-nurse[26783]:"     > Application/Process
// Column 4 = "[ID 702911 user.error]"        > Syslog facility.level
// Column 5 = "m:SY-mon-full-500"             > Message ID
// Column 6 = "c:H : partition health..."     > Message [possibly including rid, sid, ip]

#endif